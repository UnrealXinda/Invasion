// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionCharacter.h"

#include "InvasionGameplayStatics.h"

#include "Components/CapsuleComponent.h"
#include "Weapons/InvasionWeapon.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Systems/TimeDilationSystem.h"

REDIRECT_TICK_FUNC_IMPLEMENTATION(AInvasionCharacter)

// Sets default values
AInvasionCharacter::AInvasionCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	WeaponSocketName = "WeaponSocket";

	MoveState = EMoveState::Run;
	AimState = EAimState::Idle;
	CoverState = ECoverState::Idle;
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
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	}
}

void AInvasionCharacter::InvasionTick_Implementation(float DeltaTime)
{
}

void AInvasionCharacter::MoveCharacter(FVector WorldDirection, float ScaleValue /*= 1.0F*/)
{
	//switch (MoveState)
	//{
	//case EMoveState::Sprint:
	//	GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
	//	break;
	//case EMoveState::Walk:
	//	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	//	break;
	//case EMoveState::Run:
	//	GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;
	//	break;
	//}

	//AddMovementInput(WorldDirection, ScaleValue);
}

bool AInvasionCharacter::CanMove() const
{
	return true;
}

bool AInvasionCharacter::CanSprint() const
{
	bool bCanMove = CanMove();
	bool bIsAiming = AimState == EAimState::Aiming;
	return bCanMove && !bIsAiming;
}

bool AInvasionCharacter::CanAim() const
{
	bool bIsMoving = GetVelocity().SizeSquared() > 0.0f;
	bool bIsSprinting = MoveState == EMoveState::Sprint;

	return !(bIsMoving && bIsSprinting);
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
