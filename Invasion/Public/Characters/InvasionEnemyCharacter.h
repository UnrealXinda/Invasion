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
	TSubclassOf<class AInvasionParticle> PrincipalEffect;

	/** The particle effect that's attached to the subsidiary socket */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AInvasionParticle> SubsidiaryEffect;
};

USTRUCT(BlueprintType)
struct FExecutedAnimDef
{
	GENERATED_BODY()

	/** The name of the execution. Used as key to pair with player's execution montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ExecutionName;

	/** The montage to play when getting executed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* ExecutedMontage;

	/** Time delay to play executed montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Delay;
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

	/** Effect definition for bone breaking effects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects)
	TArray<FBreakableBoneEffect> BreakableBoneEffects;

	/** Serialized mapping between execution name and animation montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects)
	TArray<FExecutedAnimDef> ExecutedAnimDefs;
	
public:

	AInvasionEnemyCharacter();

	UFUNCTION(BlueprintCallable)
	bool TryBreakBone(FName InBoneName, FVector Inpulse = FVector::ZeroVector, FVector HitLocation = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGettingExecuted();

	UFUNCTION(BlueprintPure)
	bool IsBoneBroken(FName InBoneName) const;

	UFUNCTION(BlueprintPure)
	FExecutedAnimDef GetExecutedAnimDef(FName ExecutionName) const;

	float GetMaxWalkSpeed() const;

	virtual void MoveCharacter(FVector WorldDirection, float ScaleValue = 1.0F) override;

};
