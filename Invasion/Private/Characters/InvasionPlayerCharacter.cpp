// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionPlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/EnergyComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Animation/AnimMontage.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "Weapons/InvasionWeapon.h"
#include "Actors/CoverVolume.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraAnim.h"

#include "Curves/CurveFloat.h"

#include "InvasionGameplayStatics.h"
#include "Game/InvasionGameMode.h"

AInvasionPlayerCharacter::AInvasionPlayerCharacter()
{
	TimeGroup = ETimeGroup::Player;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetCollisionObjectType(ECC_Pawn);
	SphereComp->SetCanEverAffectNavigation(false);
	SphereComp->SetupAttachment(RootComponent);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.F, 96.0F);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0F, 540.0F, 0.0F); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.F;
	GetCharacterMovement()->AirControl = 0.2F;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0F; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	EnergyComp = CreateDefaultSubobject<UEnergyComponent>(TEXT("Energy Component"));

	DashState = EDashState::Idle;
	ExecuteState = EExecuteState::Idle;

	TargetMovementDir = FVector::ZeroVector;
	NormalizedSpeed = 0.0f;

	RunRotationInterpSpeed = 300.0f;
	SprintRotationInterpSpeed = 90.0f;
	bAllowRootMotionRotation = true;
}

void AInvasionPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(GetWorld());

	if (GameMode && GameMode->ExecutionData)
	{
		SphereComp->SetSphereRadius(GameMode->ExecutionData->MaximumExecutionDistance);
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AInvasionPlayerCharacter::OnSphereBeginOverlap);
	}
	
	OnTakeAnyDamage.AddDynamic(this, &AInvasionPlayerCharacter::OnPlayerTakeAnyDamage);
}

bool AInvasionPlayerCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor /*= NULL*/) const
{
	if (DashState == EDashState::Dashing)
	{
		return false;
	}

	return Super::CanBeSeenFrom(ObserverLocation, OutSeenLocation, NumberOfLoSChecksPerformed, OutSightStrength, IgnoreActor);
}

void AInvasionPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AInvasionPlayerCharacter::TickHealth(float DeltaTime)
{
	if (bCanSelfRecover)
	{
		AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(GetWorld());

		if (GameMode && GameMode->PlayerHealthConfig)
		{
			float HealthRecovered = GameMode->PlayerHealthConfig->SelfRecoverPerSecond *  DeltaTime;
			Heal(HealthRecovered);
		}
	}
}

void AInvasionPlayerCharacter::TickEnergy(float DeltaTime)
{
	if (ScanState == EScanState::Scanning)
	{
		AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(GetWorld());

		if (GameMode && GameMode->PlayerEnergyConfig)
		{
			float EnergyCost = GameMode->PlayerEnergyConfig->ScanCostPerSecond *  DeltaTime;
			EnergyComp->CostEnergy(EnergyCost);

			if (EnergyComp->GetEnergy() == 0.0f)
			{
				EndScan();
			}
		}		
	}
}

FVector AInvasionPlayerCharacter::GetPawnViewLocation() const
{
	return FollowCamera->GetComponentLocation();
}

void AInvasionPlayerCharacter::InvasionTick_Implementation(float DeltaTime)
{
	Super::InvasionTick_Implementation(DeltaTime);

	TickHealth(DeltaTime);
	TickEnergy(DeltaTime);
}

void AInvasionPlayerCharacter::BeginScan_Implementation()
{

}

void AInvasionPlayerCharacter::EndScan_Implementation()
{

}

TArray<AActor*> AInvasionPlayerCharacter::GetExecutableCharacters() const
{
	TArray<AActor*> OverlappingActors;
	SphereComp->GetOverlappingActors(OverlappingActors, TSubclassOf<AInvasionCharacter>());
	OverlappingActors.RemoveAll([this](AActor* OverlappingActor) { return OverlappingActor == this; });
	return OverlappingActors;
}

bool AInvasionPlayerCharacter::CanDash() const
{
	const AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(GetWorld());

	if (GameMode && GameMode->PlayerEnergyConfig)
	{
		bool bIsAiming = AimState == EAimState::Aiming;
		bool bIsDashing = DashState == EDashState::Dashing;
		bool bIsInCover = CoverState != ECoverState::Idle;
		bool bCanMove = CanMove();
		bool bHasEnoughEnergy = GetCurrentEnergy() >= GameMode->PlayerEnergyConfig->DashCost;

		return !bIsAiming && !bIsDashing && !bIsInCover && bCanMove && bHasEnoughEnergy;
	}

	return false;
}

bool AInvasionPlayerCharacter::CanBeginScan() const
{
	const AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(GetWorld());

	if (GameMode && GameMode->PlayerEnergyConfig)
	{
		bool bIsIdle = ScanState == EScanState::Idle;
		bool bHasEnoughEnergy = GetCurrentEnergy() >= GameMode->PlayerEnergyConfig->ScanCostPerSecond;

		return bIsIdle && bHasEnoughEnergy;
	}

	return false;	
}

bool AInvasionPlayerCharacter::CanEndScan() const
{
	return ScanState == EScanState::Scanning;
}

bool AInvasionPlayerCharacter::CanExecute() const
{
	return ExecuteState == EExecuteState::Idle;
}

float AInvasionPlayerCharacter::GetDefaultEnergy() const
{
	return EnergyComp->GetDefaultEnergy();
}

float AInvasionPlayerCharacter::GetCurrentEnergy() const
{
	return EnergyComp->GetEnergy();
}

float AInvasionPlayerCharacter::GetMaxEnergy() const
{
	return EnergyComp->GetMaxEnergy();
}

float AInvasionPlayerCharacter::GetCurrentEnergyPercentage() const
{
	return GetCurrentEnergy() / GetMaxEnergy();
}

bool AInvasionPlayerCharacter::CanMove() const
{
	bool bCanMove;

	switch (CoverState)
	{
	case ECoverState::HighIn:
	case ECoverState::HighOut:
	case ECoverState::LowIn:
	case ECoverState::LowOut:
		bCanMove = false;
		break;
	case ECoverState::InCover:
		bCanMove = AimState == EAimState::Idle;
		break;
	case ECoverState::Idle:
	default:
		bCanMove = Super::CanMove();
		break;
	}

	return bCanMove;
}

bool AInvasionPlayerCharacter::CanSprint() const
{
	return Super::CanSprint();
}

bool AInvasionPlayerCharacter::CanAim() const
{
	bool bCanAim = Super::CanAim();

	if (CoverState == ECoverState::InCover && CurrentCoverVolume)
	{
		if (CurrentCoverVolume->CoverType == ECoverType::High)
		{
			bCanAim &= CurrentCoverVolume->HasActorReachedLeftEdge(this) || CurrentCoverVolume->HasActorReachedRightEdge(this);
		}
	}

	bCanAim &= ExecuteState != EExecuteState::Executing;

	return bCanAim;
}

bool AInvasionPlayerCharacter::CanFire() const
{
	return Super::CanFire();
}

bool AInvasionPlayerCharacter::CanTakeCover() const
{
	return Super::CanTakeCover()
		&& AimState == EAimState::Idle
		&& DashState == EDashState::Idle;
}

void AInvasionPlayerCharacter::StartAim()
{
	Super::StartAim();

	AimState = EAimState::Aiming;
	bUseControllerRotationYaw = true;
	UnCrouch();
}

void AInvasionPlayerCharacter::StopAim()
{
	Super::StopAim();

	AimState = EAimState::Idle;
	bUseControllerRotationYaw = false;

	// If taking cover, reorient the character to align with the cover rotation
	if (CoverState == ECoverState::InCover && CurrentCoverVolume)
	{
		SetActorRotation(CurrentCoverVolume->GetActorRotation());

		// Get back to crouch position if taking low cover
		if (CurrentCoverVolume && CurrentCoverVolume->CoverType == ECoverType::Low)
		{
			Crouch();
		}
	}
}

void AInvasionPlayerCharacter::StartFire()
{
	Super::StartFire();
}

void AInvasionPlayerCharacter::StopFire()
{
	Super::StopFire();
}

void AInvasionPlayerCharacter::MoveCharacter(FVector WorldDirection, float ScaleValue /* = 1.0F */)
{
	NormalizedSpeed = ScaleValue;
	TargetMovementDir = WorldDirection;

	if (NormalizedSpeed > 0.0F)
	{
		FVector ForwardDir = GetActorForwardVector();
		float Z = (WorldDirection ^ ForwardDir).Z;

		LastMovementDir = (Z > 0.0f) ? EMoveDirection::Left : EMoveDirection::Right;
	}
}

bool AInvasionPlayerCharacter::EquipWeapon(AInvasionWeapon* Weapon)
{
	bool bEquipped = Super::EquipWeapon(Weapon);

	if (bEquipped)
	{
		CurrentWeapon->OnWeaponFire.AddDynamic(this, &AInvasionPlayerCharacter::OnWeaponFire);
		CurrentWeapon->OnWeaponHit.AddDynamic(this, &AInvasionPlayerCharacter::OnWeaponHit);
	}

	return bEquipped;
}

bool AInvasionPlayerCharacter::UnequipWeapon(AInvasionWeapon* Weapon)
{
	bool bUnequipped = Super::UnequipWeapon(Weapon);

	if (bUnequipped)
	{
		Weapon->OnWeaponFire.RemoveDynamic(this, &AInvasionPlayerCharacter::OnWeaponFire);
		Weapon->OnWeaponHit.RemoveDynamic(this, &AInvasionPlayerCharacter::OnWeaponHit);
	}

	return bUnequipped;
}

void AInvasionPlayerCharacter::Dash(FRotator Direction)
{
	SetActorRotation(Direction);
	PlayAnimMontage(DashMontage);

	const AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(GetWorld());

	if (GameMode && GameMode->PlayerEnergyConfig)
	{
		EnergyComp->CostEnergy(GameMode->PlayerEnergyConfig->DashCost);
	}
}

void AInvasionPlayerCharacter::Heal(float RecoverAmount)
{
	HealthComp->Heal(RecoverAmount);
}

void AInvasionPlayerCharacter::OnWeaponFire(AInvasionWeapon* Weapon, AController* InstigatedBy)
{
	Super::OnWeaponFire(Weapon, InstigatedBy);
}

void AInvasionPlayerCharacter::OnWeaponHit(AInvasionWeapon* Weapon, AController* InstigatedBy, AActor* HitActor, EPhysicalSurface PhysicalSurface)
{
	if (const AInvasionCharacter* HitCharacter = Cast<AInvasionCharacter>(HitActor))
	{
		// Only gain energy when hitting alive characters
		if (HitCharacter->GetCurrentHealth() > 0.0F)
		{
			const AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(GetWorld());

			if (GameMode && GameMode->PlayerEnergyConfig)
			{
				float EnergyRecover = GameMode->PlayerEnergyConfig->GetEnergyRecoverFromWeapon(Weapon->WeaponType, PhysicalSurface);
				EnergyComp->GainEnergy(EnergyRecover);
			}
		}
	}
}

void AInvasionPlayerCharacter::OnSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor*              OtherActor,
	UPrimitiveComponent* OtherComp,
	int32                OtherBodyIndex,
	bool                 bFromSweep,
	const FHitResult&    SweepResult)
{
	// TODO: show on screen widget to imply availablity for execution
}

void AInvasionPlayerCharacter::OnPlayerTakeAnyDamage(
	AActor*                  DamagedActor,
	float                    Damage,
	const class UDamageType* DamageType,
	class AController*       InstigatedBy,
	AActor*                  DamageCauser)
{
	AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(GetWorld());

	if (GameMode && GameMode->PlayerHealthConfig)
	{
		bCanSelfRecover = false;

		float TimeBeforeSelfRecover = GameMode->PlayerHealthConfig->TimeBeforeSelfRecover;
		FTimerDelegate TimerCallback;

		TimerCallback.BindLambda([this]{ bCanSelfRecover = true; });

		GetWorldTimerManager().ClearTimer(ResetSelfRecoverTimerHandle);
		GetWorldTimerManager().SetTimer(ResetSelfRecoverTimerHandle, TimerCallback, TimeBeforeSelfRecover, false);
	}
}

void AInvasionPlayerCharacter::OnCharacterDeath(
	UHealthComponent*  HealthComponent,
	float              LastDamage,
	const UDamageType* DamageType,
	AController*       InstigatedBy,
	AActor*            DamageCauser
)
{
	Super::OnCharacterDeath(HealthComponent, LastDamage, DamageType, InstigatedBy, DamageCauser);

	OnTakeAnyDamage.RemoveDynamic(this, &AInvasionPlayerCharacter::OnPlayerTakeAnyDamage);
}

void AInvasionPlayerCharacter::ExecuteCharacter_Implementation(AInvasionCharacter* Victim)
{

}

