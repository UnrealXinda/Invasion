// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionEnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AInvasionEnemyCharacter::AInvasionEnemyCharacter()
{

}

float AInvasionEnemyCharacter::GetMaxWalkSpeed() const
{
	return GetCharacterMovement()->MaxWalkSpeed;
}

void AInvasionEnemyCharacter::MoveCharacter(FVector WorldDirection, float ScaleValue /*= 1.0F*/)
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
