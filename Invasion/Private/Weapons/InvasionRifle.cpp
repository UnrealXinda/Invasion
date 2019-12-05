// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/InvasionRifle.h"

#include "Invasion.h"

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

void AInvasionRifle::BeginPlay()
{
	Super::BeginPlay();

	RandomStream.GenerateNewSeed();
}

void AInvasionRifle::Fire()
{
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		OwnerPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection;
		FVector TraceEnd;
		ShotDirection = EyeRotation.Vector();

		// Add recoil to controller
		if (APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController()))
		{
			AddRecoilToController(PlayerController);
		}
		//else
		//{
		//	// Apply bullet spread to shots
		//	float RandomConeHalfRadian = FMath::DegreesToRadians(RandomConeHalfDegrees);
		//	ShotDirection = FMath::VRandCone(ShotDirection, RandomConeHalfRadian, RandomConeHalfRadian);

		//	TraceEnd = EyeLocation + TraceRangeMax * ShotDirection;
		//}
		TraceEnd = EyeLocation + TraceRangeMax * ShotDirection;

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(OwnerPawn);
		Params.AddIgnoredActor(this);
		Params.bTraceComplex = true;
		Params.bReturnPhysicalMaterial = true;

		// Blocking hit! Processing damage
		bool bHitTarget = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_WEAPON, Params);

		if (bHitTarget)
		{
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

			// Play impact effect
			PlayImpactEffect(HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), SurfaceType);

			TraceEnd = HitResult.ImpactPoint;
		}

		if (InvasionDebug::g_DebugDrawWeaponTrace)
		{
			FColor LineColor = bHitTarget ? FColor::Green : FColor::Red;
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, LineColor, false, 1.0f, 0, 1.0f);
		}

		// Play tracer effect
		PlayTracerEffect(TraceEnd);

		// Play fire effects
		PlayFireEffects();

		LastFireTime = GetWorld()->TimeSeconds;
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
	UParticleSystem* SelectedEffect;

	switch (SurfaceType)
	{
	case SURFACE_FLESH_DEFAULT:
	case SURFACE_FLESH_VULNERABLE:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}

	if (SelectedEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Location, Rotation);
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
