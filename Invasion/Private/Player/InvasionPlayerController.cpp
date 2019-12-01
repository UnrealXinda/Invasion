// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InvasionPlayerController.h"
#include "Player/InvasionPlayerState.h"
#include "Player/InvasionPlayerConfiguration.h"
#include "Player/InvasionPlayerCameraManager.h"

#include "Characters/InvasionPlayerCharacter.h"
#include "Weapons/InvasionWeapon.h"

#include "InvasionGameplayStatics.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerInput.h"

#include "Systems/TimeDilationSystem.h"

REDIRECT_TICK_FUNC_IMPLEMENTATION(AInvasionPlayerController)

AInvasionPlayerController::AInvasionPlayerController()
{
	TimeGroup = ETimeGroup::System;
	InvasionTimeDilation = 1.0f;

	PlayerCameraManagerClass = AInvasionPlayerCameraManager::StaticClass();
}

void AInvasionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Cache local variables to eliminate the necessity of casting every time we try to use it
	PlayerCharacter = Cast<AInvasionPlayerCharacter>(GetPawn());
	InvasionPlayerState = Cast<AInvasionPlayerState>(PlayerState);
	InvasionPlayerCameraManager = Cast<AInvasionPlayerCameraManager>(PlayerCameraManager);

	check(PlayerCharacter);
	check(InvasionPlayerState);
}

void AInvasionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis(InvasionStatics::MoveForward, this, &AInvasionPlayerController::MoveForward);
	InputComponent->BindAxis(InvasionStatics::MoveRight, this, &AInvasionPlayerController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis(InvasionStatics::Turn, this, &AInvasionPlayerController::Turn);
	InputComponent->BindAxis(InvasionStatics::TurnRate, this, &AInvasionPlayerController::TurnAtRate);
	InputComponent->BindAxis(InvasionStatics::LookUp, this, &AInvasionPlayerController::LookUp);
	InputComponent->BindAxis(InvasionStatics::LookUpRate, this, &AInvasionPlayerController::LookUpAtRate);

	// Face button actions
	InputComponent->BindAction(InvasionStatics::Dash, IE_Pressed, this, &AInvasionPlayerController::OnPressDash);
	InputComponent->BindAction(InvasionStatics::Sprint, IE_Pressed, this, &AInvasionPlayerController::OnPressSprint);
	InputComponent->BindAction(InvasionStatics::Sprint, IE_Released, this, &AInvasionPlayerController::OnReleaseSprint);

	// Left trigger button actions
	InputComponent->BindAction(InvasionStatics::Aim, IE_Pressed, this, &AInvasionPlayerController::OnPressAim);
	InputComponent->BindAction(InvasionStatics::Aim, IE_Released, this, &AInvasionPlayerController::OnReleaseAim);

	// Right trigger button actions
	InputComponent->BindAction(InvasionStatics::Fire, IE_Pressed, this, &AInvasionPlayerController::OnPressFire);
	InputComponent->BindAction(InvasionStatics::Fire, IE_Released, this, &AInvasionPlayerController::OnReleaseFire);
}

void AInvasionPlayerController::InvasionTick_Implementation(float DeltaTime)
{
	TickCharacterMovement(DeltaTime);
	TickCameraFOV(DeltaTime);
}

void AInvasionPlayerController::MoveForward(float Value)
{
	LastMovementInputVector.X = Value;
}

void AInvasionPlayerController::MoveRight(float Value)
{
	LastMovementInputVector.Y = Value;
}

void AInvasionPlayerController::TickCharacterMovement(float DeltaTime)
{
	float Magnitude = LastMovementInputVector.Size();

	if (Magnitude > 0.0F)
	{
		bool bInputAboveWalkThreshold = Magnitude > InvasionPlayerState->PlayerConfiguration->InputSetting.WalkInputThreshold;
		bool bIsSprintKeyDown = IsActionKeyDown(InvasionStatics::Sprint);

		if (!bInputAboveWalkThreshold)
		{
			PlayerCharacter->MoveState = EMoveState::Walk;
		}

		else if (bIsSprintKeyDown)
		{
			PlayerCharacter->MoveState = EMoveState::Sprint;
		}

		else
		{
			PlayerCharacter->MoveState = EMoveState::Run;
		}

		FVector WorldInputVector = GetWorldInputVector();
		PlayerCharacter->MoveCharacter(WorldInputVector, 1.0F);
	}
}

void AInvasionPlayerController::TickCameraFOV(float DeltaTime)
{
	if (PlayerCharacter && PlayerCharacter->CurrentWeapon)
	{
		AInvasionWeapon* Weapon = PlayerCharacter->CurrentWeapon;

		float TargetFOV;
		float ZoomInterpSpeed = Weapon->ZoomInterpSpeed;

		switch (PlayerCharacter->AimState)
		{
		case EAimState::Aiming:
		{
			TargetFOV = Weapon->ZoomedFOV;
			break;
		}
		case EAimState::Idle:
		{
			TargetFOV = PlayerCameraManager->DefaultFOV;
			break;
		}
		}

		float CurrentFOV = PlayerCameraManager->GetFOVAngle();
		float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, ZoomInterpSpeed);
		PlayerCameraManager->SetFOV(NewFOV);
	}
}

void AInvasionPlayerController::Turn(float Rate)
{
	AddYawInput(Rate);
}

void AInvasionPlayerController::LookUp(float Rate)
{
	AddPitchInput(Rate);
}

void AInvasionPlayerController::TurnAtRate(float Rate)
{
	float BaseTurnRate = InvasionPlayerState->PlayerConfiguration->InputSetting.BaseTurnRate;
	float GlobalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());

	// calculate delta for this frame from the rate information
	AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() / GlobalTimeDilation);
}

void AInvasionPlayerController::LookUpAtRate(float Rate)
{
	float BaseLookUpRate = InvasionPlayerState->PlayerConfiguration->InputSetting.BaseLookUpRate;
	float GlobalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());

	// calculate delta for this frame from the rate information
	AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() / GlobalTimeDilation);
}

void AInvasionPlayerController::OnPressDash()
{
}

void AInvasionPlayerController::OnPressSprint()
{

}

void AInvasionPlayerController::OnReleaseSprint()
{

}

void AInvasionPlayerController::OnPressAim()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->AimState = EAimState::Aiming;
		PlayerCharacter->bUseControllerRotationYaw = true;
	}
}

void AInvasionPlayerController::OnReleaseAim()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->AimState = EAimState::Idle;
		PlayerCharacter->bUseControllerRotationYaw = false;
	}
}

void AInvasionPlayerController::OnPressFire()
{
	if (PlayerCharacter && PlayerCharacter->CurrentWeapon)
	{
		PlayerCharacter->CurrentWeapon->StartFire();
	}
}

void AInvasionPlayerController::OnReleaseFire()
{
	if (PlayerCharacter && PlayerCharacter->CurrentWeapon)
	{
		PlayerCharacter->CurrentWeapon->StopFire();
	}
}

FRotator AInvasionPlayerController::GetWorldInputRotation() const
{
	FRotator CameraRot = PlayerCameraManager->GetCameraRotation();
	FRotator InputRot = LastMovementInputVector.Rotation();
	FRotator WorldInputRot = CameraRot + InputRot;
	WorldInputRot.Roll = 0.0F;
	WorldInputRot.Pitch = 0.0F;

	return WorldInputRot;
}

FVector AInvasionPlayerController::GetWorldInputVector() const
{
	return UKismetMathLibrary::GetForwardVector(GetWorldInputRotation());
}

bool AInvasionPlayerController::IsActionKeyDown(FName ActionName)
{
	bool bIsKeyDown = false;
	const TArray<FInputActionKeyMapping>& KeyMapping = PlayerInput->GetKeysForAction(ActionName);

	if (KeyMapping.Num() > 0)
	{
		const FInputActionKeyMapping* Mapping = KeyMapping.FindByPredicate([this](const FInputActionKeyMapping& Mapping)
		{
			return IsInputKeyDown(Mapping.Key);
		});

		bIsKeyDown = (Mapping != nullptr);
	}

	return bIsKeyDown;
}
