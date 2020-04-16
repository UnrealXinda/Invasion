// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/InvasionRifle.h"
#include "Weapons/WeaponDamageInfo.h"

#include "Invasion.h"
#include "Characters/InvasionCharacter.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Animation/AnimationAsset.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Curves/CurveFloat.h"
#include "Components/SkeletalMeshComponent.h"

#include "InvasionGameplayStatics.h"

AInvasionRifle::AInvasionRifle()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	RootComponent = WeaponSkeletalMesh;

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

void AInvasionRifle::BeginPlay()
{
	Super::BeginPlay();

	RandomStream.GenerateNewSeed();
}

float AInvasionRifle::GetDamage(EPhysicalSurface SurfaceType, float Distance) const
{
	float Damage = 0;
	const FWeaponDamageInfoEntry* InfoEntry = DamageInfo->DamageConfig.FindByPredicate([SurfaceType](const FWeaponDamageInfoEntry& Entry)
	{
		return Entry.PhysicalSurface == SurfaceType;
	});

	if (InfoEntry && InfoEntry->DamagePerDistanceCurve)
	{
		Damage = InfoEntry->DamagePerDistanceCurve->GetFloatValue(Distance);
	}

	return Damage;
}


void AInvasionRifle::Fire()
{
	if (AInvasionCharacter* OwnerPawn = Cast<AInvasionCharacter>(GetOwner()))
	{
		if (UWorld* World = GetWorld())
		{
			if (OwnerPawn->CanFire())
			{
				FVector TraceStart, TraceEnd;

				// Get weapon trace for player controller
				if (APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController()))
				{
					GetWeaponTraceForPlayer(TraceStart, TraceEnd, OwnerPawn);
					AddRecoilToController(PlayerController);
				}

				// Get weapon trace for AI controller
				else
				{
					GetWeaponTraceForAI(TraceStart, TraceEnd, OwnerPawn);
				}

				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(OwnerPawn);
				Params.AddIgnoredActor(this);
				Params.bTraceComplex = true;
				Params.bReturnPhysicalMaterial = true;

				// Blocking hit! Processing damage
				bool bHitTarget = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Weapon, Params);

				if (bHitTarget)
				{
					EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
					float Distance = (HitResult.Location - TraceStart).Size();
					float Damage = GetDamage(SurfaceType, Distance);

					AActor* HitActor = HitResult.GetActor();
					AController* InstigatorController = OwnerPawn->GetInstigatorController();
					FVector ShotDirection = TraceEnd - TraceStart;

					UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, InstigatorController, this, DamageType);

					// Play impact effect
					PlayImpactEffect(HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), SurfaceType);

					// Modify the trace end for debug drawing
					if (InvasionDebug::g_DebugDrawWeaponTrace)
					{
						TraceEnd = HitResult.ImpactPoint;
					}
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

				LastFireTime = World->TimeSeconds;
			}
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
	WeaponSkeletalMesh->SetVisibility(bVisible);
}

bool AInvasionRifle::IsWeaponVisible() const
{
	return WeaponSkeletalMesh->IsVisible();
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
	PlayMuzzleEffect();
	PlayCameraShakeEffect();
	PlayFireAnimation();
}

void AInvasionRifle::PlayFireAnimation()
{
	if (FireAnimation)
	{
		WeaponSkeletalMesh->PlayAnimation(FireAnimation, false);
	}
}

void AInvasionRifle::PlayMuzzleEffect()
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponSkeletalMesh, MuzzleSocketName);
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

		if (UWorld* World = GetWorld())
		{
			if (FImpactEffect* Effect = ImpactEffects.FindByPredicate(FindSurfaceTypePredicate))
			{
				if (UParticleSystem* SelectedEffect = Effect->ImpactParticleEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(World, SelectedEffect, Location, Rotation);

					if (USoundBase* ImpactSound = Effect->ImpactSound)
					{
						UGameplayStatics::PlaySoundAtLocation(World, ImpactSound, Location, Effect->VolumeMultiplier, Effect->PitchMultiplier);
					}

					return true;
				}
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
		FVector MuzzleLocation = WeaponSkeletalMesh->GetSocketLocation(MuzzleSocketName);

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

void AInvasionRifle::GetWeaponTraceForPlayer(FVector& TraceStart, FVector& TraceEnd, APawn* OwningPawn) const
{
	check(OwningPawn != nullptr);

	FVector EyeLocation;
	FRotator EyeRotation;
	OwningPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector ShotDirection = EyeRotation.Vector();
	FVector MuzzleLocation = WeaponSkeletalMesh->GetSocketLocation(MuzzleSocketName);

	// If we trace from the camera position, there's a slight chance of the ray hitting something behind the 
	// character. Hence, we move the trace start position forward to the intersection between the ray and the
	// plane on which the gun muzzle point sits and is orthogonal to the camera forward.
	FPlane Plane(MuzzleLocation, ShotDirection);

	TraceStart = FMath::RayPlaneIntersection(EyeLocation, ShotDirection, Plane);
	TraceEnd = TraceStart + TraceRangeMax * ShotDirection;
}

void AInvasionRifle::GetWeaponTraceForAI(FVector& TraceStart, FVector& TraceEnd, APawn* OwningPawn) const
{
	check(OwningPawn != nullptr);

	if (UWorld* World = GetWorld())
	{
		if (AActor* TargetActor = UInvasionGameplayStatics::GetInvasionPlayerCharacter(GetWorld()))
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			OwningPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector ShotDirection = EyeRotation.Vector();
			FVector MuzzleLocation = WeaponSkeletalMesh->GetSocketLocation(MuzzleSocketName);

			FVector OwingPawnLoc = OwningPawn->GetActorLocation();
			FVector TargetActorLoc = TargetActor->GetActorLocation();

			// Apply bullet spread to shots
			float Distance = (OwingPawnLoc - TargetActorLoc).Size();
			float RandomConeHalfDegrees = RandomConeHalfDegreesPerDistance->GetFloatValue(Distance);
			float RandomConeHalfRadian = FMath::DegreesToRadians(RandomConeHalfDegrees);
			ShotDirection = FMath::VRandCone(ShotDirection, RandomConeHalfRadian, RandomConeHalfRadian);

			TraceStart = EyeLocation;
			TraceEnd = TraceStart + TraceRangeMax * ShotDirection;
		}
	}
}
