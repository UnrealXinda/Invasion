// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionPlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
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

	if (UWorld* World = GetWorld())
	{
		if (AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(GetWorld()))
		{
			SphereComp->SetSphereRadius(GameMode->MaximumExecutionDistance);
			SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AInvasionPlayerCharacter::OnSphereBeginOverlap);
		}
	}
	
	OnTakeAnyDamage.AddDynamic(this, &AInvasionPlayerCharacter::OnPlayerTakeAnyDamage);
}

void AInvasionPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AInvasionPlayerCharacter::RecoverHealth(float DeltaSeconds)
{
	if (UWorld* World = GetWorld())
	{
		if (AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(World))
		{
			float HealthRecovered = GameMode->SelfRecoverPerSecond * DeltaSeconds;
			Heal(HealthRecovered);
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

	if (bCanSelfRecover)
	{
		RecoverHealth(DeltaTime);
	}
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
	return Super::CanTakeCover()
		&& AimState == EAimState::Idle
		&& DashState == EDashState::Idle;
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
	PlayAnimMontage(DashMontage);
}

void AInvasionPlayerCharacter::Heal(float RecoverAmount)
{
	HealthComp->Heal(RecoverAmount);
}

void AInvasionPlayerCharacter::OnWeaponFire(AInvasionWeapon* Weapon, AController* InstigatedBy)
{
	Super::OnWeaponFire(Weapon, InstigatedBy);
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

void AInvasionPlayerCharacter::EnableSelfRecover()
{
	bCanSelfRecover = true;
}

void AInvasionPlayerCharacter::OnPlayerTakeAnyDamage(
	AActor*                  DamagedActor,
	float                    Damage,
	const class UDamageType* DamageType,
	class AController*       InstigatedBy,
	AActor*                  DamageCauser)
{
	if (UWorld* World = GetWorld())
	{
		if (AInvasionGameMode* GameMode = UInvasionGameplayStatics::GetInvasionGameMode(World))
		{
			bCanSelfRecover = false;

			float TimeBeforeSelfRecover = GameMode->TimeBeforeSelfRecover;
			GetWorldTimerManager().ClearTimer(ResetSelfRecoverTimerHandle);
			GetWorldTimerManager().SetTimer(ResetSelfRecoverTimerHandle, this, &AInvasionPlayerCharacter::EnableSelfRecover, TimeBeforeSelfRecover, false);
		}
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
}

void AInvasionPlayerCharacter::ExecuteCharacter_Implementation(AInvasionCharacter* Victim)
{

}

