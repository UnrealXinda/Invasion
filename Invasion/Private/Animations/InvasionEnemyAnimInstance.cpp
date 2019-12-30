// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/InvasionEnemyAnimInstance.h"

#include "Characters/InvasionEnemyCharacter.h"

UInvasionEnemyAnimInstance::UInvasionEnemyAnimInstance()
{

}

void UInvasionEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UInvasionEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (AInvasionEnemyCharacter* EnemyCharacter = Cast<AInvasionEnemyCharacter>(OwningPawn))
	{
		FVector Velocity = OwningPawn->GetVelocity();
		FVector ForwardDir = OwningPawn->GetActorForwardVector();
		FRotator PawnRot = OwningPawn->GetActorRotation();

		float Speed = Velocity.Size();
		float MaxSpeed = EnemyCharacter->GetMaxWalkSpeed();
		float Z = (Velocity ^ ForwardDir).Z;

		Direction = CalculateDirection(Velocity, PawnRot);
		NormalizedSpeed = Speed / MaxSpeed;
		LastMovementDir = (Z > 0.0f) ? EMoveDirection::Left : EMoveDirection::Right;
	}

}
