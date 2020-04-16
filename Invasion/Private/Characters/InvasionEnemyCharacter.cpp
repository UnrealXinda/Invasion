// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionEnemyCharacter.h"
#include "Actors/InvasionParticle.h"
#include "Animation/AnimMontage.h"
#include "AIModule/Classes/BrainComponent.h"
#include "AIModule/Classes/AIController.h"

#include "GameFramework/CharacterMovementComponent.h"

AInvasionEnemyCharacter::AInvasionEnemyCharacter()
{
}

void AInvasionEnemyCharacter::PauseCharacter()
{
	FString Reason;
	PauseBehaviorTreeLogic(Reason);

	GetMesh()->bPauseAnims = true;
	
	StopFire();
}

void AInvasionEnemyCharacter::ResumeCharacter()
{
	FString Reason;
	ResumeBehaviorTreeLogic(Reason);

	GetMesh()->bPauseAnims = false;
}

void AInvasionEnemyCharacter::PauseBehaviorTreeLogic(const FString& Reason)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* BrainComp = AIController->GetBrainComponent())
		{
			BrainComp->PauseLogic(Reason);
		}
	}
}

void AInvasionEnemyCharacter::ResumeBehaviorTreeLogic(const FString& Reason)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* BrainComp = AIController->GetBrainComponent())
		{
			BrainComp->ResumeLogic(Reason);
		}
	}
}

bool AInvasionEnemyCharacter::TryStartAim()
{
	bool bCanAim = CanAim();
	if (bCanAim)
	{
		AimState = EAimState::Aiming;
	}

	return bCanAim;
}

bool AInvasionEnemyCharacter::TryEndAim()
{
	AimState = EAimState::Idle;

	// If taking cover, reorient the character to align with the cover rotation
	if (CoverState == ECoverState::InCover)
	{
		SetActorRotation(CurrentCoverVolume->GetActorRotation());
	}

	return true;
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
					if (UWorld* World = GetWorld())
					{
						AInvasionParticle* EffectActor = World->SpawnActor<AInvasionParticle>(EffectClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
						EffectActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketToAttach);
					}
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

void AInvasionEnemyCharacter::InvasionTick_Implementation(float DeltaTime)
{
	Super::InvasionTick_Implementation(DeltaTime);
	TickCharacterMovement(DeltaTime);
}

void AInvasionEnemyCharacter::TickCharacterMovement(float DeltaTime)
{
	float MaxSpeed = MaxSprintSpeed;

	switch (CoverState)
	{
	case ECoverState::HighIn:
	case ECoverState::HighOut:
	case ECoverState::LowIn:
	case ECoverState::LowOut:
		MaxSpeed = FMath::Min(MaxSpeed, 0.0f);
		break;
	case ECoverState::InCover:
		MaxSpeed = FMath::Min(MaxSpeed, MaxWalkSpeed);
		break;
	}

	switch (MoveState)
	{
	case EMoveState::Sprint:
		MaxSpeed = FMath::Min(MaxSpeed, MaxSprintSpeed);
		break;
	case EMoveState::Walk:
		MaxSpeed = FMath::Min(MaxSpeed, MaxWalkSpeed);
		break;
	case EMoveState::Run:
		MaxSpeed = FMath::Min(MaxSpeed, MaxRunSpeed);
		break;
	}

	switch (AimState)
	{
	case EAimState::Idle:
		MaxSpeed = FMath::Min(MaxSpeed, MaxSprintSpeed);
		break;
	case EAimState::Aiming:
		MaxSpeed = FMath::Min(MaxSpeed, MaxWalkSpeed);
	}

	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}
