// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionCharacter.h"

#include "InvasionGameplayStatics.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/IKComponent.h"
#include "Components/HealthComponent.h"

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
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

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
	AInvasionWeapon* Weapon = GetWorld()->SpawnActor<AInvasionWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);

	EquipWeapon(Weapon);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInvasionCharacter::OnCapsuleBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AInvasionCharacter::OnCapsuleEndOverlap);

	HealthComp->OnCharacterDeath.AddDynamic(this, &AInvasionCharacter::OnCharacterDeath_Internal);
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

bool AInvasionCharacter::EquipWeapon(class AInvasionWeapon* Weapon)
{
	if (Weapon && Weapon != CurrentWeapon)
	{
		Weapon->EquipWeapon(this, GetMesh(), WeaponSocketName);
		Weapon->OnWeaponFire.AddDynamic(this, &AInvasionCharacter::OnWeaponFire_Internal);
		CurrentWeapon = Weapon;

		return true;
	}

	return false;
}

bool AInvasionCharacter::UnequipWeapon(class AInvasionWeapon* Weapon)
{
	if (Weapon && Weapon == CurrentWeapon)
	{
		Weapon->UnequipWeapon();
		Weapon->OnWeaponFire.RemoveDynamic(this, &AInvasionCharacter::OnWeaponFire_Internal);
		CurrentWeapon = nullptr;

		return true;
	}

	return false;
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

void AInvasionCharacter::OnWeaponFire_Internal(AInvasionWeapon* Weapon, AController* InstigatedBy)
{
	if (Weapon && Weapon == CurrentWeapon)
	{
		const FWeaponAnimation* WeaponAnimation = WeaponAnimations.FindByPredicate([Weapon](FWeaponAnimation Anim)
		{
			return Anim.WeaponType == Weapon->WeaponType;
		});

		if (WeaponAnimation && WeaponAnimation->FireMontage)
		{
			if (USkeletalMeshComponent* Mesh = GetMesh())
			{
				if (UAnimInstance* AnimInstance = Cast<UAnimInstance>(Mesh->GetAnimInstance()))
				{
					AnimInstance->Montage_Play(WeaponAnimation->FireMontage);
				}
			}
		}
	}
}
void AInvasionCharacter::OnCharacterDeath(
	class UHealthComponent*  HealthComponent,
	float                    LastDamage,
	const class UDamageType* DamageType,
	class AController*       InstigatedBy,
	AActor*                  DamageCauser
)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}

	UAnimMontage* MontageToPlay;

	switch (CoverState)
	{
	case ECoverState::Idle:
		MontageToPlay = DeathMontage;
		break;
	default:
		MontageToPlay = CoverDeathMontage;
		break;
	}

	GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);

	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DetachFromControllerPendingDestroy();
}

void AInvasionCharacter::OnCharacterDeath_Internal(
	class UHealthComponent*  HealthComponent,
	float                    LastDamage,
	const class UDamageType* DamageType,
	class AController*       InstigatedBy,
	AActor*                  DamageCauser
)
{
	OnCharacterDeath(HealthComponent, LastDamage, DamageType, InstigatedBy, DamageCauser);
	HealthComp->OnCharacterDeath.RemoveDynamic(this, &AInvasionCharacter::OnCharacterDeath_Internal);
}
