// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

/** Delegate type for callbacks when health value changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(
	FOnHealthChangedSignature,
	/* The health component that is reporting this event */
	UHealthComponent*, HealthComp,
	/* The current health value */
	float, Health,
	/* The delta health value. Positive for taking damage. Negative for healing. */
	float, HealthDelta,
	/* the damage type when taking damage. NULL for healing. */
	const class UDamageType*, DamageType,
	/* The instigator of this damage. NULL for healing. */
	class AController*, InstigatedBy,
	/* The causer of this damage. NULL for healing. */
	AActor*, DamageCauser
);

/** Delegate type for callbacks when health is equal to or below zero */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FOnCharacterDeathSignature,
	/* The health component that is reporting this event */
	UHealthComponent*, HealthComp,
	/* The last damage that causes this death */
	float, LastDamage,
	/* the damage type when taking damage. NULL for healing. */
	const class UDamageType*, DamageType,
	/* The instigator of this damage. NULL for healing. */
	class AController*, InstigatedBy,
	/* The causer of this damage. NULL for healing. */
	AActor*, DamageCauser
);

UCLASS(ClassGroup=(Health), meta=(BlueprintSpawnableComponent))
class INVASION_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCharacterDeathSignature OnCharacterDeath;	

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float DefaultHealth;

public:	

	UHealthComponent();

	UFUNCTION(BlueprintCallable)
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable)
	void DealExecutionDamage(float DamageAmount);

protected:

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
			class AController* InstigatedBy, AActor* DamageCauser);

	virtual void BeginPlay() override;
		
};
