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

float AInvasionCharacter::GetDefaultHealth() const
{
	return HealthComp->GetDefaultHealth();
}

float AInvasionCharacter::GetCurrentHealth() const
{
	return HealthComp->GetHealth();
}

float AInvasionCharacter::GetCurrentHealthPercentage() const
{
	return GetCurrentHealth() / GetDefaultHealth();
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
	HealthComp->OnCharacterExecuted.AddDynamic(this, &AInvasionCharacter::OnCharacterExecuted_Internal);
}

void AInvasionCharacter::InvasionTick_Implementation(float DeltaTime)
{
}

void AInvasionCharacter::MoveCharacter(FVector WorldDirection, float ScaleValue /*= 1.0F*/)
{
}

bool AInvasionCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor /*= NULL*/) const
{
	FVector HitLoc;
	FVector CharacterLoc = GetActorLocation();
	FVector TraceStart = ObserverLocation;
	FVector TraceEnd = CharacterLoc;
	AActor* ThisActor = const_cast<AInvasionCharacter*>(this);
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(const_cast<AActor*>(IgnoreActor));
	IgnoredActors.Add(ThisActor);

	bool bHit = UInvasionGameplayStatics::RaycastTest(this, TraceStart, TraceEnd, IgnoredActors, HitLoc, true, InvasionDebug::g_DebugDrawAISightTrace);
	++NumberOfLoSChecksPerformed;

	if (!bHit)
	{
		OutSightStrength = 1.0f;
		OutSeenLocation = HitLoc;
		return true;
	}

	const float DistSqr = (TraceStart - TraceEnd).SizeSquared();
	if (DistSqr > FARSIGHTTHRESHOLDSQUARED)
	{
		return false;
	}

	float CylinderRadius, CylinderHeight;
	GetSimpleCollisionCylinder(CylinderRadius, CylinderHeight);

	// only check sides if width of other is significant compared to distance
	if (CylinderRadius * CylinderRadius / DistSqr < 0.0001f)
	{
		return false;
	}
	//try checking sides - look at dist to four side points, and cull furthest and closest
	FVector Points[4];
	Points[0] = CharacterLoc - FVector(CylinderRadius, -1 * CylinderRadius, 0);
	Points[1] = CharacterLoc + FVector(CylinderRadius, CylinderRadius, 0);
	Points[2] = CharacterLoc - FVector(CylinderRadius, CylinderRadius, 0);
	Points[3] = CharacterLoc + FVector(CylinderRadius, -1 * CylinderRadius, 0);
	int32 IndexMin = 0;
	int32 IndexMax = 0;
	float CurrentMax = (Points[0] - TraceStart).SizeSquared();
	float CurrentMin = CurrentMax;
	for (int32 PointIndex = 1; PointIndex < 4; PointIndex++)
	{
		const float NextSize = (Points[PointIndex] - TraceStart).SizeSquared();
		if (NextSize > CurrentMin)
		{
			CurrentMin = NextSize;
			IndexMax = PointIndex;
		}
		else if (NextSize < CurrentMax)
		{
			CurrentMax = NextSize;
			IndexMin = PointIndex;
		}
	}

	int LoSCount = 0;
	int RaycastCount = 0;

	for (int32 PointIndex = 0; PointIndex < 4; PointIndex++)
	{
		if ((PointIndex != IndexMin) && (PointIndex != IndexMax))
		{
			TraceEnd = Points[PointIndex];
			bHit = UInvasionGameplayStatics::RaycastTest(this, TraceStart, TraceEnd, IgnoredActors, HitLoc, true, InvasionDebug::g_DebugDrawAISightTrace);
			++NumberOfLoSChecksPerformed;
			++RaycastCount;

			if (!bHit)
			{
				++LoSCount;
				OutSeenLocation = HitLoc;
			}
		}
	}

	OutSightStrength = static_cast<float>(LoSCount) / RaycastCount;

	return LoSCount > 0;
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

		// Facing direction when taking cover depending on which side is closer to character
		FVector LeftCompLoc = CurrentCoverVolume->GetLeftBlockVolumeLocation();
		FVector RightCompLoc = CurrentCoverVolume->GetRightBlockVolumeLocation();
		FVector CharacterLoc = GetActorLocation();
		float LeftDistSqr = FVector::DistSquared2D(LeftCompLoc, CharacterLoc);
		float RightDistSqr = FVector::DistSquared2D(RightCompLoc, CharacterLoc);
		LastMovementDir = (LeftDistSqr < RightDistSqr) ? EMoveDirection::Left : EMoveDirection::Right;

		return true;
	}

	return false;
}

bool AInvasionCharacter::TryUntakeCover()
{
	bool bIsInCoverState = (CoverState == ECoverState::InCover);
	bool bIsAiming = (AimState == EAimState::Aiming);

	if (bIsInCoverState)
	{
		if (!bIsAiming)
		{
			if (CurrentCoverVolume)
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
			}
			else
			{
				CoverState = ECoverState::HighOut;
			}

			CurrentCoverVolume = nullptr;
		}

		return !bIsAiming;
	}

	return true;
}

void AInvasionCharacter::OnCharacterKilled()
{
	HealthComp->OnCharacterDeath.RemoveDynamic(this, &AInvasionCharacter::OnCharacterDeath_Internal);
	HealthComp->OnHealthChanged.RemoveDynamic(this, &AInvasionCharacter::OnHealthChanged_Internal);

	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DetachFromControllerPendingDestroy();
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

void AInvasionCharacter::OnWeaponFire(AInvasionWeapon* Weapon, AController* InstigatedBy)
{
	if (Weapon && Weapon == CurrentWeapon)
	{
		const FWeaponAnimation* WeaponAnimation = WeaponAnimations.FindByPredicate([Weapon](const FWeaponAnimation& Anim)
		{
			return Anim.WeaponType == Weapon->WeaponType;
		});

		if (WeaponAnimation && WeaponAnimation->FireMontage)
		{
			if (USkeletalMeshComponent* SkeletalMesh = GetMesh())
			{
				if (UAnimInstance* AnimInstance = Cast<UAnimInstance>(SkeletalMesh->GetAnimInstance()))
				{
					AnimInstance->Montage_Play(WeaponAnimation->FireMontage);
				}
			}
		}
	}
}

void AInvasionCharacter::OnWeaponFire_Internal(AInvasionWeapon* Weapon, AController* InstigatedBy)
{
	OnWeaponFire(Weapon, InstigatedBy);
}

void AInvasionCharacter::OnHealthChanged(
	UHealthComponent*        HealthComponent,
	float                    Health,
	float                    HealthDelta,
	const UDamageType*       DamageType,
	class AController*       InstigatedBy,
	AActor*                  DamageCauser
)
{

}

void AInvasionCharacter::OnCharacterDeath(
	UHealthComponent*        HealthComponent,
	float                    LastDamage,
	const UDamageType*       DamageType,
	class AController*       InstigatedBy,
	AActor*                  DamageCauser
)
{
	StopFire();

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
}

void AInvasionCharacter::OnCharacterExecuted(
	class UHealthComponent*  HealthComponent,
	float                    LastDamage,
	class AController*       InstigatedBy,
	AActor*                  DamageCauser
)
{

}

void AInvasionCharacter::OnHealthChanged_Internal(
	UHealthComponent*        HealthComponent,
	float                    Health,
	float                    HealthDelta,
	const UDamageType*       DamageType,
	class AController*       InstigatedBy,
	AActor*                  DamageCauser
)
{
	OnHealthChanged(HealthComponent, Health, HealthDelta, DamageType, InstigatedBy, DamageCauser);
	ReceiveOnHealthChanged(HealthComponent, Health, HealthDelta, DamageType, InstigatedBy, DamageCauser);
}

void AInvasionCharacter::OnCharacterDeath_Internal(
	class UHealthComponent*  HealthComponent,
	float                    LastDamage,
	const class UDamageType* DamageType,
	class AController*       InstigatedBy,
	AActor*                  DamageCauser
)
{
	OnCharacterKilled();
	OnCharacterDeath(HealthComponent, LastDamage, DamageType, InstigatedBy, DamageCauser);
	ReceiveOnCharacterDeath(HealthComponent, LastDamage, DamageType, InstigatedBy, DamageCauser);
}

void AInvasionCharacter::OnCharacterExecuted_Internal(
	class UHealthComponent*  HealthComponent,
	float                    LastDamage,
	class AController*       InstigatedBy,
	AActor*                  DamageCauser
)
{
	OnCharacterKilled();
	OnCharacterExecuted(HealthComponent, LastDamage, InstigatedBy, DamageCauser);
	ReceiveOnCharacterExecuted(HealthComponent, LastDamage, InstigatedBy, DamageCauser);
}
