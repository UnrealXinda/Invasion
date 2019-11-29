// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionCharacter.h"

#include "InvasionGameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Systems/TimeDilationSystem.h"

REDIRECT_TICK_FUNC_IMPLEMENTATION(AInvasionCharacter)

// Sets default values
AInvasionCharacter::AInvasionCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInvasionCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AInvasionCharacter::InvasionTick_Implementation(float DeltaTime)
{
}

void AInvasionCharacter::MoveCharacter(FVector WorldDirection, float ScaleValue /*= 1.0F*/)
{
	switch (MoveState)
	{
	case EMoveState::Sprint:
		GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
		break;
	case EMoveState::Walk:
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		break;
	case EMoveState::Run:
		GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;
		break;
	}

	AddMovementInput(WorldDirection, ScaleValue);
}

