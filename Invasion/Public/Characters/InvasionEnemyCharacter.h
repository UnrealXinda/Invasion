// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/InvasionCharacter.h"
#include "InvasionEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API AInvasionEnemyCharacter : public AInvasionCharacter
{
	GENERATED_BODY()

public:

	/** The movement speed for walking */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float MaxWalkSpeed;

	/** The movement speed for running */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float MaxRunSpeed;

	/** The movement speed for sprinting */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float MaxSprintSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Physics)
	TArray<FName> BreakableBones;
	
public:

	AInvasionEnemyCharacter();

	float GetMaxWalkSpeed() const;

	virtual void MoveCharacter(FVector WorldDirection, float ScaleValue = 1.0F) override;

};
