// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/InvasionRifle.h"

#include "Invasion.h"
#include "Characters/InvasionCharacter.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Curves/CurveFloat.h"
#include "Components/StaticMeshComponent.h"

AInvasionRifle::AInvasionRifle()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	TracerTargetName = "Target";
	MuzzleSocketName = "MuzzleSocket";

	TraceRangeMax = 10000.0f;

	ZoomInfo.DefaultFOV = 90.0f;
	ZoomInfo.ZoomedFOV = 65.0f;
	ZoomInfo.ZoomInterpSpeed = 20.0f;
	ZoomInfo.ZoomedPitchMin = -50.0f;
	ZoomInfo.ZoomedPitchMax = 50.0f;

	RecoilPitchMin = -0.1f;
	RecoilPitchMax = -0.3f;
	RecoilYawMax = 0.2f;

	RateOfFire = 600;
}

float AInvasionRifle::GetDamageAmountFromSurfaceType(EPhysicalSurface SurfaceType) const
{
	float Damage;

	switch (SurfaceType)
	{
	case SURFACE_FLESH_VULNERABLE:
	case SURFACE_METAL_VULNERABLE:
		Damage = FatalDamage;
		break;
	case SURFACE_FLESH_DEFAULT:
	case SURFACE_METAL_DEFAULT:
	default:
		Damage = BaseDamage;
		break;
	}

	return Damage;
}

void AInvasionRifle::BeginPlay()
{
	Super::BeginPlay();

	RandomStream.GenerateNewSeed();
}

void AInvasionRifle::Fire()
{
	if (AInvasionCharacter* OwnerPawn = Cast<AInvasionCharacter>(GetOwner()))
	{
		if (OwnerPawn->CanFire())
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			OwnerPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector ShotDirection;
			FVector TraceStart, TraceEnd;
			ShotDirection = EyeRotation.Vector();

			// Add recoil to controller
			if (APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController()))
			{
				AddRecoilToController(PlayerController);
			}
			//// Randomize shot
			//else
			//{
			//	// Apply bullet spread to shots
			//	float RandomConeHalfRadian = FMath::DegreesToRadians(RandomConeHalfDegrees);
			//	ShotDirection = FMath::VRandCone(ShotDirection, RandomConeHalfRadian, RandomConeHalfRadian);

			//	TraceEnd = EyeLocation + TraceRangeMax * ShotDirection;
			//}

			// If we trace from the camera position, there's a slight chance of the ray hitting something behind the 
			// character. Hence, we move the trace start position forward to the intersection between the ray and the
			// plane on which the gun muzzle point sits and is orthogonal to the camera forward.
			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			FPlane Plane(MuzzleLocation, ShotDirection);

			TraceStart = FMath::RayPlaneIntersection(EyeLocation, ShotDirection, Plane);
			TraceEnd = TraceStart + TraceRangeMax * ShotDirection;

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(OwnerPawn);
			Params.AddIgnoredActor(this);
			Params.bTraceComplex = true;
			Params.bReturnPhysicalMaterial = true;

			// Blocking hit! Processing damage
			bool bHitTarget = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Weapon, Params);

			if (bHitTarget)
			{
				EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
				float Damage = GetDamageAmountFromSurfaceType(SurfaceType);

				AActor* HitActor = HitResult.GetActor();
				AController* InstigatorController = OwnerPawn->GetInstigatorController();

				UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, InstigatorController, this, DamageType);

				// Play impact effect
				PlayImpactEffect(HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), SurfaceType);

				// Modify the trace end for debug drawing
				TraceEnd = HitResult.ImpactPoint;
			}

			if (InvasionDebug::g_DebugDrawWeaponTrace)
			{
				FColor LineColor = bHitTarget ? FColor::Green : FColor::Red;
				DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 2.0f, 0, 1.0f);
			}

			// Play tracer effect
			PlayTracerEffect(TraceEnd);

			// Play fire effects
			PlayFireEffects();

			// Broadcast a successful fire event
			BroadcastOnWeaponFire();

			LastFireTime = GetWorld()->TimeSeconds;
		}
	}
}

void AInvasionRifle::StartFire()
{
	float TimeBetweenShots = 60.0f / RateOfFire;
	float FirstDelay = FMath::Max(TimeBetweenShots + (LastFireTime - GetWorld()->TimeSeconds), 0.0f);
	GetWorldTimerManager().SetTimer(ShotIntervalTimerHandle, this, &AInvasionRifle::Fire, TimeBetweenShots, true, FirstDelay);
}

void AInvasionRifle::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotIntervalTimerHandle);
}

void AInvasionRifle::SetWeaponVisibility(bool bVisible)
{
	MeshComp->SetVisibility(bVisible);
}

bool AInvasionRifle::IsWeaponVisible() const
{
	return MeshComp->bVisible;
}

void AInvasionRifle::AddRecoilToController(APlayerController* Controller) const
{
	if (Controller)
	{
		float PitchInput = RandomStream.FRandRange(RecoilPitchMin, RecoilPitchMax);
		float YawInput = RandomStream.FRandRange(-RecoilYawMax, RecoilYawMax);
		Controller->AddPitchInput(PitchInput);
		Controller->AddYawInput(YawInput);
	}
}

void AInvasionRifle::PlayFireEffects()
{
	// Play muzzle effect
	PlayMuzzleEffect();

	// Play camera shake effect
	PlayCameraShakeEffect();
}

void AInvasionRifle::PlayMuzzleEffect()
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
}

void AInvasionRifle::PlayImpactEffect(FVector Location, FRotator Rotation, EPhysicalSurface SurfaceType)
{
	auto TryPlayImpactEffect = [this, Location, Rotation](EPhysicalSurface Surface)
	{
		auto FindSurfaceTypePredicate = [Surface](const FImpactEffect& ImpactEffect)
		{
			return ImpactEffect.SurfaceType == Surface;
		};

		if (FImpactEffect* Effect = ImpactEffects.FindByPredicate(FindSurfaceTypePredicate))
		{
			if (UParticleSystem* SelectedEffect = Effect->ImpactParticleEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Location, Rotation);
				return true;
			}
		}

		return false;
	};

	if (!TryPlayImpactEffect(SurfaceType))
	{
		TryPlayImpactEffect(SurfaceType_Default);
	}
}

void AInvasionRifle::PlayTracerEffect(FVector TargetPoint)
{
	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		if (UParticleSystemComponent* ParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation))
		{
			ParticleComp->SetVectorParameter(TracerTargetName, TargetPoint);
		}
	}
}

void AInvasionRifle::PlayCameraShakeEffect()
{
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController()))
		{
			PlayerController->ClientPlayCameraShake(CameraShakeClass);
		}
	}
}

void AInvasionRifle::BroadcastOnWeaponFire()
{
	if (APawn* OwnerActor = Cast<APawn>(GetOwner()))
	{
		if (AController* InstigatedBy = OwnerActor->GetController())
		{
			OnWeaponFire.Broadcast(this, InstigatedBy);
		}
	}
}
