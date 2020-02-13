// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionEnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AInvasionEnemyCharacter::AInvasionEnemyCharacter()
{

}

bool AInvasionEnemyCharacter::TryBreakBone(FName InBoneName)
{
	if (!IsBoneBroken(InBoneName))
	{
		FBreakableBoneEffect* Effect = BreakableBoneEffects.FindByPredicate([InBoneName](const FBreakableBoneEffect& BreakableEffect)
		{
			return BreakableEffect.BoneName == InBoneName;
		});

		// Found valid breakable bone
		if (Effect)
		{
			GetMesh()->BreakConstraint(FVector::ZeroVector, FVector::ZeroVector, InBoneName);

			auto SpawnEffect = [this](TSubclassOf<AActor> EffectClass, FName SocketToAttach)
			{
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				if (EffectClass)
				{
					AActor* EffectActor = GetWorld()->SpawnActor<AActor>(EffectClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
					EffectActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketToAttach);
				}
			};

			SpawnEffect(Effect->PrincipalEffect, Effect->PrincipalSocketName);
			SpawnEffect(Effect->SubsidiaryEffect, Effect->SubsidiarySocketName);

			return true;
		}
	}

	return false;
}

bool AInvasionEnemyCharacter::IsBoneBroken(FName InBoneName) const
{
	if (USkeletalMeshComponent* Mesh = GetMesh())
	{
		int32 ConstraintIndex = Mesh->FindConstraintIndex(InBoneName);

		if (ConstraintIndex != INDEX_NONE)
		{
			FConstraintInstance* Constraint = Mesh->Constraints[ConstraintIndex];
			return Constraint->IsTerminated();
		}
	}

	// By default, a non-existing constraint is broken
	return true;
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
}