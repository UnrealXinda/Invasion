// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/InvasionCharacter.h"
#include "InvasionEnemyCharacter.generated.h"

USTRUCT(BlueprintType)
struct FBreakableBoneEffect
{
	GENERATED_BODY()

	/** The name of the bone that's breakable */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName BoneName;

	/** The name of the socket that's attached to the main body */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName PrincipalSocketName;

	/** The name of the socket that's attached to the broken bone */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SubsidiarySocketName;

	/** The particle effect that's attached to the principal socket */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> PrincipalEffect;

	/** The particle effect that's attached to the subsidiary socket */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> SubsidiaryEffect;
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects)
	TArray<FBreakableBoneEffect> BreakableBoneEffects;
	
public:

	AInvasionEnemyCharacter();

	UFUNCTION(BlueprintCallable)
	bool TryBreakBone(FName InBoneName);

	UFUNCTION(BlueprintPure)
	bool IsBoneBroken(FName InBoneName) const;

	float GetMaxWalkSpeed() const;

	virtual void MoveCharacter(FVector WorldDirection, float ScaleValue = 1.0F) override;

};
