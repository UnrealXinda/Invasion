// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InvasionPlayerController.h"
#include "Player/InvasionPlayerState.h"
#include "Player/InvasionPlayerConfiguration.h"
#include "Player/InvasionPlayerCameraManager.h"

#include "Characters/InvasionPlayerCharacter.h"
#include "Characters/InvasionEnemyCharacter.h"
#include "Weapons/InvasionWeapon.h"
#include "Actors/CoverVolume.h"

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

	ViewPitchMin = -90.0f;
	ViewPitchMax = 90.0f;

	CoverViewYawMin = -45.0f;
	CoverViewYawMax = 45.0f;

	DefaultViewYawMin = 0.0f;
	DefaultViewYawMax = 359.999f;

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
	InputComponent->BindAction(InvasionStatics::TakeCover, IE_Pressed, this, &AInvasionPlayerController::OnPressTakeCover);
	InputComponent->BindAction(InvasionStatics::Execute, IE_Pressed, this, &AInvasionPlayerController::OnPressExecute);

	// Left shoulder button actions
	InputComponent->BindAction(InvasionStatics::Sprint, IE_Pressed, this, &AInvasionPlayerController::OnPressSprint);
	InputComponent->BindAction(InvasionStatics::Sprint, IE_Released, this, &AInvasionPlayerController::OnReleaseSprint);

	// Left trigger button actions
	InputComponent->BindAction(InvasionStatics::Aim, IE_Pressed, this, &AInvasionPlayerController::OnPressAim);
	InputComponent->BindAction(InvasionStatics::Aim, IE_Released, this, &AInvasionPlayerController::OnReleaseAim);

	// Right trigger button actions
	InputComponent->BindAction(InvasionStatics::Fire, IE_Pressed, this, &AInvasionPlayerController::OnPressFire);
	InputComponent->BindAction(InvasionStatics::Fire, IE_Released, this, &AInvasionPlayerController::OnReleaseFire);
}

void AInvasionPlayerController::DisableInput(class APlayerController* PlayerController)
{
	Super::DisableInput(PlayerController);

	LastMovementInputVector = FVector::ZeroVector;
}

void AInvasionPlayerController::InvasionTick_Implementation(float DeltaTime)
{
	TickCharacterMovement(DeltaTime);
	TickCharacterRotation(DeltaTime);
	TickCameraManager(DeltaTime);
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
	// We are caching input vector and manually processing character movement. Therefore, we need to account for
	// the case where input is disabled
	if (PlayerCharacter && InputEnabled())
	{
		FVector MoveDirection = FVector::ZeroVector;
		float NormalizedSpeed = 0.0f;
		bool bCanMove = PlayerCharacter->CanMove();
		bool bHasInput = LastMovementInputVector.SizeSquared() > 0.0f;

		if (bCanMove && bHasInput)
		{
			bool bIsAiming = PlayerCharacter->AimState == EAimState::Aiming;
			bool bCanSprint = PlayerCharacter->CanSprint();
			bool bIsSprintKeyDown = IsActionKeyDown(InvasionStatics::Sprint);

			if (bIsAiming)
			{
				PlayerCharacter->MoveState = EMoveState::Walk;
			}

			else if (bCanSprint && bIsSprintKeyDown)
			{
				PlayerCharacter->MoveState = EMoveState::Sprint;
			}

			else
			{
				PlayerCharacter->MoveState = EMoveState::Run;
			}

			MoveDirection = GetWorldInputVector();
			NormalizedSpeed = 1.0f;

			// Cannot move outside of cover
			if (PlayerCharacter->CoverState == ECoverState::InCover)
			{
				bool bMovingLeft = LastMovementInputVector.Y < 0.0f;
				bool bMovingRight = LastMovementInputVector.Y > 0.0f;
				bool bAtCoverLeftEdge = PlayerCharacter->CurrentCoverVolume->HasActorReachedLeftEdge(PlayerCharacter);
				bool bAtCoverRightEdge = PlayerCharacter->CurrentCoverVolume->HasActorReachedRightEdge(PlayerCharacter);

				if ((bMovingLeft && bAtCoverLeftEdge) || (bMovingRight && bAtCoverRightEdge))
				{
					NormalizedSpeed = 0.0f;
				}
			}
		}

		// Reset move state to normal jog
		else
		{
			PlayerCharacter->MoveState = EMoveState::Run;
		}

		PlayerCharacter->MoveCharacter(MoveDirection, NormalizedSpeed);
	}
}

void AInvasionPlayerController::TickCharacterRotation(float DeltaTime)
{
	// We are caching input vector and manually processing character movement. Therefore, we need to account for
	// the case where input is disabled
	if (PlayerCharacter && InputEnabled())
	{
		// Can only control character rotation if it is not driven by root motion, and if it is not controlled by 
		// controller yaw.
		bool bCanControlRot = !(PlayerCharacter->bAllowRootMotionRotation || PlayerCharacter->bUseControllerRotationYaw);

		if (bCanControlRot)
		{
			float InterpSpeed = 0.0f;

			switch (PlayerCharacter->MoveState)
			{
			case EMoveState::Sprint:
			{
				InterpSpeed = PlayerCharacter->SprintRotationInterpSpeed;
				break;
			}
			case EMoveState::Run:
			{
				InterpSpeed = PlayerCharacter->RunRotationInterpSpeed;
				break;
			}
			}

			FRotator CurrentRot = PlayerCharacter->GetActorRotation();
			FRotator TargetRot = GetWorldInputRotation();
			FRotator NewRot = FMath::RInterpConstantTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);
			PlayerCharacter->SetActorRotation(NewRot);
		}
	}
}

void AInvasionPlayerController::TickCameraManager(float DeltaTime)
{
	if (PlayerCharacter)
	{
		// Update view min/max pitch value
		auto TickViewPitchValue = [this]()
		{
			float MinPitch = ViewPitchMin;
			float MaxPitch = ViewPitchMax;

			bool bIsAming = PlayerCharacter->AimState == EAimState::Aiming;
			bool bHasWeapon = !!PlayerCharacter->CurrentWeapon;

			if (bIsAming && bHasWeapon)
			{
				MinPitch = FMath::Max(MinPitch, PlayerCharacter->CurrentWeapon->ZoomInfo.ZoomedPitchMin);
				MaxPitch = FMath::Min(MaxPitch, PlayerCharacter->CurrentWeapon->ZoomInfo.ZoomedPitchMax);
			}

			PlayerCameraManager->ViewPitchMin = MinPitch;
			PlayerCameraManager->ViewPitchMax = MaxPitch;
		};

		// Update view min/max yaw value
		auto TickViewYawValue = [this]()
		{
			if (PlayerCharacter->CoverState == ECoverState::Idle)
			{
				PlayerCameraManager->ViewYawMin = DefaultViewYawMin;
				PlayerCameraManager->ViewYawMax = DefaultViewYawMax;
			}

			// Is taking cover
			else
			{
				ACoverVolume* CurrentCover = PlayerCharacter->CurrentCoverVolume;

				if (CurrentCover)
				{
					float CoverYaw = CurrentCover->GetActorRotation().Yaw;

					if (PlayerCharacter->AimState == EAimState::Idle)
					{
						PlayerCameraManager->ViewYawMin = FRotator::ClampAxis(CoverYaw + CoverViewYawMin);
						PlayerCameraManager->ViewYawMax = FRotator::ClampAxis(CoverYaw + CoverViewYawMax);
					}
					else
					{
						switch (CurrentCover->CoverType)
						{
						case ECoverType::Low:
							PlayerCameraManager->ViewYawMin = FRotator::ClampAxis(CoverYaw + CurrentCover->AimingYawMin);
							PlayerCameraManager->ViewYawMax = FRotator::ClampAxis(CoverYaw + CurrentCover->AimingYawMax);
							break;
						case ECoverType::High:
							if (CurrentCover->HasActorReachedLeftEdge(PlayerCharacter))
							{
								PlayerCameraManager->ViewYawMin = FRotator::ClampAxis(CoverYaw + CurrentCover->AimingYawMin);
								PlayerCameraManager->ViewYawMax = FRotator::ClampAxis(CoverYaw + CurrentCover->AimingYawMax);
							}
							else if (CurrentCover->HasActorReachedRightEdge(PlayerCharacter))
							{
								PlayerCameraManager->ViewYawMin = FRotator::ClampAxis(CoverYaw - CurrentCover->AimingYawMax);
								PlayerCameraManager->ViewYawMax = FRotator::ClampAxis(CoverYaw - CurrentCover->AimingYawMin);
							}
							break;
						}
					}
				}
			}
		};

		// Play camera shake when sprinting
		auto PlaySprintCameraShake = [this]()
		{
			bool bIsSprinting = PlayerCharacter->MoveState == EMoveState::Sprint;

			if (bIsSprinting && SprintCameraShakeClass)
			{
				float MaxSpeed = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed;
				float CurrentSpeed = PlayerCharacter->GetVelocity().Size();
				float Scale = FMath::Clamp(0.0f, 1.0f, CurrentSpeed / MaxSpeed);
				InvasionPlayerCameraManager->PlayCameraShakeSingleInstance(SprintCameraShakeClass, Scale);
			}
		};

		TickViewPitchValue();
		TickViewYawValue();
		PlaySprintCameraShake();
	}
}

void AInvasionPlayerController::TickCameraFOV(float DeltaTime)
{
	if (PlayerCharacter && PlayerCharacter->CurrentWeapon)
	{
		float NewFOV;
		AInvasionWeapon* Weapon = PlayerCharacter->CurrentWeapon;

		if (InputEnabled())
		{
			float TargetFOV;
			float ZoomInterpSpeed = Weapon->ZoomInfo.ZoomInterpSpeed;

			switch (PlayerCharacter->AimState)
			{
			case EAimState::Aiming:
			{
				TargetFOV = Weapon->ZoomInfo.ZoomedFOV;
				break;
			}
			case EAimState::Idle:
			default:
			{
				TargetFOV = Weapon->ZoomInfo.DefaultFOV;
				break;
			}
			}

			float CurrentFOV = PlayerCameraManager->GetFOVAngle();
			NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, ZoomInterpSpeed);
		}

		else
		{
			NewFOV = Weapon->ZoomInfo.DefaultFOV;
		}

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
	if (PlayerCharacter && PlayerCharacter->CanDash())
	{
		FRotator DashDirection;

		// Rotate the character to face dash direction
		if (!LastMovementInputVector.IsNearlyZero())
		{
			FRotator CharacterRot = PlayerCharacter->GetActorRotation();
			FRotator WorldInputRot = GetWorldInputRotation();

			float Roll = CharacterRot.Roll;
			float Pitch = CharacterRot.Pitch;
			float Yaw = WorldInputRot.Yaw;

			DashDirection = FRotator(Pitch, Yaw, Roll);
		}

		else
		{
			DashDirection = PlayerCharacter->GetActorRotation();
		}

		PlayerCharacter->Dash(DashDirection);
	}
}

void AInvasionPlayerController::OnPressTakeCover()
{
	if (PlayerCharacter)
	{
		switch (PlayerCharacter->CoverState)
		{
		case ECoverState::Idle:
			if (PlayerCharacter->CanTakeCover())
			{
				PlayerCharacter->TryTakeCover();
			}
			break;

		case ECoverState::InCover:
			PlayerCharacter->TryUntakeCover();
			break;
		}
	}
}

void AInvasionPlayerController::OnPressExecute()
{
	if (PlayerCharacter && PlayerCharacter->CanExecute())
	{
		TArray<AActor*> ExecutableCharacters = PlayerCharacter->GetExecutableCharacters();

		if (ExecutableCharacters.Num() > 0)
		{
			// TODO: iterate through all characters to find the one closest to camera center
			if (AInvasionEnemyCharacter* EnemyCharacter = Cast<AInvasionEnemyCharacter>(ExecutableCharacters[0]))
			{
				PlayerCharacter->ExecuteCharacter(EnemyCharacter);
			}
		}
	}
}

void AInvasionPlayerController::OnPressSprint()
{

}

void AInvasionPlayerController::OnReleaseSprint()
{

}

void AInvasionPlayerController::OnPressAim()
{
	if (PlayerCharacter && PlayerCharacter->CanAim())
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

		// If taking cover, reorient the character to align with the cover rotation
		if (PlayerCharacter->CoverState == ECoverState::InCover)
		{
			ACoverVolume* CurrentCover = PlayerCharacter->CurrentCoverVolume;
			PlayerCharacter->SetActorRotation(CurrentCover->GetActorRotation());
		}
	}
}

void AInvasionPlayerController::OnPressFire()
{
	if (PlayerCharacter && PlayerCharacter->CanFire())
	{
		PlayerCharacter->StartFire();
	}
}

void AInvasionPlayerController::OnReleaseFire()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->StopFire();
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

bool AInvasionPlayerController::IsActionKeyDown(FName ActionName) const
{
	bool bIsKeyDown = false;
	const TArray<FInputActionKeyMapping>& KeyMapping = PlayerInput->GetKeysForAction(ActionName);

	if (KeyMapping.Num() > 0)
	{
		const FInputActionKeyMapping* Mapping = KeyMapping.FindByPredicate([this](const FInputActionKeyMapping& Mapping)
		{
			return IsInputKeyDown(Mapping.Key);
		});

		bIsKeyDown = !!Mapping;
	}

	return bIsKeyDown;
}