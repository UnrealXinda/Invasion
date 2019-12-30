// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionCharacter.h"

#include "InvasionGameplayStatics.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/IKComponent.h"

#include "Weapons/InvasionWeapon.h"
#include "Actors/CoverVolume.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Systems/TimeDilationSystem.h"

REDIRECT_TICK_FUNC_IMPLEMENTATION(AInvasionCharacter)

// Sets default values
AInvasionCharacter::AInvasionCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	IKComp = CreateDefaultSubobject<UIKComponent>(TEXT("IK Component"));

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Weapon, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_CoverObject, ECR_Overlap);

	WeaponSocketName = "WeaponSocket";

	MoveState = EMoveState::Run;
	AimState = EAimState::Idle;
	CoverState = ECoverState::Idle;

	CurrentCoverVolume = nullptr;
}

// Called when the game starts or when spawned
void AInvasionCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Spawn a weapon
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AInvasionWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (CurrentWeapon)
	{
		CurrentWeapon->EquipWeapon(this, GetMesh(), WeaponSocketName);
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInvasionCharacter::OnCapsuleBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AInvasionCharacter::OnCapsuleEndOverlap);
}

void AInvasionCharacter::InvasionTick_Implementation(float DeltaTime)
{
}

void AInvasionCharacter::MoveCharacter(FVector WorldDirection, float ScaleValue /*= 1.0F*/)
{
}

bool AInvasionCharacter::TryTakeCover()
{
	if (AvailableCoverVolumes.Num() > 0)
	{
		float MinDistSqr = FLT_MAX;
		ACoverVolume* ClosestCover = nullptr;
		FVector ActorLoc = GetActorLocation();

		// Find the closest cover
		for (ACoverVolume* Cover : AvailableCoverVolumes)
		{
			FVector CoverLoc = Cover->GetActorLocation();
			float Distance = (ActorLoc - CoverLoc).SizeSquared();
			if (Distance < MinDistSqr)
			{
				ClosestCover = Cover;
			}

			MinDistSqr = FMath::Min(MinDistSqr, Distance);
		}

		switch (ClosestCover->CoverType)
		{
		case ECoverType::Low:
			CoverState = ECoverState::LowIn;
			break;
		case ECoverType::High:
			CoverState = ECoverState::HighIn;
			break;
		}

		CurrentCoverVolume = ClosestCover;

		// Reorient the character so that its movement follows along the cover volume
		SetActorRotation(CurrentCoverVolume->GetActorRotation());
	}

	return false;
}

bool AInvasionCharacter::TryUntakeCover()
{
	bool bIsTakingCover = !!CurrentCoverVolume;
	bool bIsInCoverState = (CoverState == ECoverState::InCover);
	bool bIsAiming = (AimState == EAimState::Aiming);

	bool bCanUntakeCover = bIsTakingCover && bIsInCoverState && !bIsAiming;

	if (bCanUntakeCover)
	{
		switch (CurrentCoverVolume->CoverType)
		{
		case ECoverType::Low:
			CoverState = ECoverState::LowOut;
			break;
		case ECoverType::High:
			CoverState = ECoverState::HighOut;
			break;
		}

		CurrentCoverVolume = nullptr;

		return true;
	}

	return false;
}

bool AInvasionCharacter::CanMove() const
{
	return true;
}

bool AInvasionCharacter::CanSprint() const
{
	bool bCanMove = CanMove();
	bool bIsAiming = AimState == EAimState::Aiming;
	bool bIsInCover = CoverState != ECoverState::Idle;
	return bCanMove && !bIsAiming && !bIsInCover;
}

bool AInvasionCharacter::CanAim() const
{
	bool bIsMoving = GetVelocity().SizeSquared() > 0.0f;
	bool bIsSprinting = MoveState == EMoveState::Sprint;
	bool bIsReadyForAiming = CoverState == ECoverState::Idle || CoverState == ECoverState::InCover;

	return !(bIsMoving && bIsSprinting) && bIsReadyForAiming;
}

bool AInvasionCharacter::CanFire() const
{
	return AimState == EAimState::Aiming;
}

bool AInvasionCharacter::CanTakeCover() const
{
	return CoverState == ECoverState::Idle;
}

void AInvasionCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void AInvasionCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AInvasionCharacter::OnCapsuleBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor*              OtherActor,
	UPrimitiveComponent* OtherComp,
	int32                OtherBodyIndex,
	bool                 bFromSweep,
	const FHitResult&    SweepResult
)
{
	if (ACoverVolume* CoverVolume = Cast<ACoverVolume>(OtherActor))
	{
		if (OtherComp == CoverVolume->GetCoverComponent())
		{
			AvailableCoverVolumes.AddUnique(CoverVolume);
			// TODO: register callback on cover volume to show world space widget
		}
	}
}

void AInvasionCharacter::OnCapsuleEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor*              OtherActor,
	UPrimitiveComponent* OtherComp,
	int32                OtherBodyIndex
)
{
	if (ACoverVolume* CoverVolume = Cast<ACoverVolume>(OtherActor))
	{
		if (OtherComp == CoverVolume->GetCoverComponent())
		{
			AvailableCoverVolumes.RemoveSwap(CoverVolume);
		}
	}
}