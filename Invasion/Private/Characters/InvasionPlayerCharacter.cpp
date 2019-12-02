// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionPlayerCharacter.h"

#include "Components/CapsuleComponent.h"

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

	MaxWalkSpeed = 200;
	MaxRunSpeed = 600;
	MaxSprintSpeed = 800;
}

FVector AInvasionPlayerCharacter::GetPawnViewLocation() const
{
	return FollowCamera->GetComponentLocation();
}

void AInvasionPlayerCharacter::InvasionTick_Implementation(float DeltaTime)
{
	Super::InvasionTick_Implementation(DeltaTime);

	TickMovementSpeed();
}

void AInvasionPlayerCharacter::TickMovementSpeed()
{
	float CurrentMaxSpeed = 0.0f;

	switch (MoveState)
	{
	case EMoveState::Walk:
		CurrentMaxSpeed = MaxWalkSpeed;
		break;
	case EMoveState::Run:
		CurrentMaxSpeed = MaxRunSpeed;
		break;
	case EMoveState::Sprint:
		CurrentMaxSpeed = MaxSprintSpeed;
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = CurrentMaxSpeed;
}
