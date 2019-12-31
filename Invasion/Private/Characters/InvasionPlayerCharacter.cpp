// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionPlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Animation/AnimMontage.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "Weapons/InvasionWeapon.h"
#include "Actors/CoverVolume.h"

#include "Camera/CameraComponent.h"

AInvasionPlayerCharacter::AInvasionPlayerCharacter()
{
	TimeGroup = ETimeGroup::Player;

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

	DashState = EDashState::Idle;
	ExecuteState = EExecuteState::Idle;

	TargetMovementDir = FVector::ZeroVector;
	NormalizedSpeed = 0.0f;

	RunRotationInterpSpeed = 300.0f;
	SprintRotationInterpSpeed = 90.0f;
	bAllowRootMotionRotation = true;
}

FVector AInvasionPlayerCharacter::GetPawnViewLocation() const
{
	return FollowCamera->GetComponentLocation();
}

void AInvasionPlayerCharacter::InvasionTick_Implementation(float DeltaTime)
{
	Super::InvasionTick_Implementation(DeltaTime);
}

bool AInvasionPlayerCharacter::CanDash() const
{
	bool bIsAiming = AimState == EAimState::Aiming;
	bool bIsDashing = DashState == EDashState::Dashing;
	bool bIsInCover = CoverState != ECoverState::Idle;
	bool bCanMove = CanMove();

	return !bIsAiming && !bIsDashing && !bIsInCover && bCanMove;
}

bool AInvasionPlayerCharacter::CanExecute() const
{
	return ExecuteState == EExecuteState::Idle;
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

	if (CoverState == ECoverState::InCover)
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
	bool bCanTakeCover = Super::CanTakeCover();
	bCanTakeCover &= AimState == EAimState::Idle;
	bCanTakeCover &= DashState == EDashState::Idle;

	return bCanTakeCover;
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
	// TODO: check if taking cover to decide whether can move to given direction

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
	}

	return bEquipped;
}



bool AInvasionPlayerCharacter::UnequipWeapon(AInvasionWeapon* Weapon)
{
	bool bUnequipped = Super::UnequipWeapon(Weapon);

	if (bUnequipped)
	{
		Weapon->OnWeaponFire.RemoveDynamic(this, &AInvasionPlayerCharacter::OnWeaponFire);
	}

	return bUnequipped;
}

void AInvasionPlayerCharacter::Dash(FRotator Direction)
{
	SetActorRotation(Direction);

	if (DashMontage)
	{
		PlayAnimMontage(DashMontage);
	}
}

void AInvasionPlayerCharacter::OnWeaponFire(AInvasionWeapon* Weapon, AController* InstigatedBy)
{

}

void AInvasionPlayerCharacter::ExecuteCharacter_Implementation(AInvasionCharacter* Victim)
{

}

