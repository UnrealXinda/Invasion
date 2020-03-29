// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionEnemyCharacter.h"
#include "Actors/InvasionParticle.h"
#include "Animation/AnimMontage.h"

#include "GameFramework/CharacterMovementComponent.h"

AInvasionEnemyCharacter::AInvasionEnemyCharacter()
{

}

bool AInvasionEnemyCharacter::TryBreakBone(FName InBoneName, FVector Inpulse, FVector HitLocation)
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
			GetMesh()->BreakConstraint(Inpulse, HitLocation, InBoneName);

			auto SpawnEffect = [this](TSubclassOf<AInvasionParticle> EffectClass, FName SocketToAttach)
			{
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				if (EffectClass)
				{
					AInvasionParticle* EffectActor = GetWorld()->SpawnActor<AInvasionParticle>(EffectClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
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

void AInvasionEnemyCharacter::OnGettingExecuted_Implementation()
{
	// TODO: this is a bit redundant. Integrate execution kill halding into normal kill handling
	OnCharacterKilled();
}

bool AInvasionEnemyCharacter::IsBoneBroken(FName InBoneName) const
{
	if (USkeletalMeshComponent* SkeletalMesh = GetMesh())
	{
		int32 ConstraintIndex = SkeletalMesh->FindConstraintIndex(InBoneName);

		if (ConstraintIndex != INDEX_NONE)
		{
			FConstraintInstance* Constraint = SkeletalMesh->Constraints[ConstraintIndex];
			return Constraint->IsTerminated();
		}
	}

	// By default, a non-existing constraint is broken
	return true;
}

FExecutedAnimDef AInvasionEnemyCharacter::GetExecutedAnimDef(FName ExecutionName) const
{
	FExecutedAnimDef Result;
	const FExecutedAnimDef* Def = ExecutedAnimDefs.FindByPredicate([ExecutionName](const FExecutedAnimDef& Def) { return Def.ExecutionName == ExecutionName; });

	if (Def)
	{
		Result = *Def;
	}
	else if (ExecutedAnimDefs.Num() > 0)
	{
		Result = ExecutedAnimDefs[0];
	}

	return Result;
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