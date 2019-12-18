// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionPlayerCharacter.h"

#include "Components/CapsuleComponent.h"

#include "Animation/AnimMontage.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "Weapons/InvasionWeapon.h"

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
	bool bCanMove = CanMove();

	return !bIsAiming && !bIsDashing && bCanMove;
}

bool AInvasionPlayerCharacter::CanMove() const
{
	return Super::CanMove();
}

bool AInvasionPlayerCharacter::CanSprint() const
{
	return Super::CanSprint();
}

bool AInvasionPlayerCharacter::CanAim() const
{
	return Super::CanAim();
}

bool AInvasionPlayerCharacter::CanFire() const
{
	return Super::CanFire();
}

bool AInvasionPlayerCharacter::CanTakeCover() const
{
	return Super::CanTakeCover();
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
}

void AInvasionPlayerCharacter::Dash(FRotator Direction)
{
	SetActorRotation(Direction);

	if (DashMontage)
	{
		PlayAnimMontage(DashMontage);
	}
}
