// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnergyComponent.generated.h"

/** Delegate type for callbacks when energy value changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnEnergyChangedSignature,
	/* The energy component that is reporting this event */
	UEnergyComponent*, EnergyComp,
	/* The delta value of energy */
	float, DeltaValue
);

UCLASS(ClassGroup=(Energy), meta=(BlueprintSpawnableComponent))
class INVASION_API UEnergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnergyChangedSignature OnEnergyChanged;

public:

	UEnergyComponent();

	UFUNCTION(BlueprintCallable)
	void CostEnergy(float CostAmount);

	UFUNCTION(BlueprintCallable)
	void GainEnergy(float GainAmount);

	UFUNCTION(BlueprintPure)
	float GetEnergy() const;

	UFUNCTION(BlueprintPure)
	float GetDefaultEnergy() const;

	UFUNCTION(BlueprintPure)
	float GetMaxEnergy() const;

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Energy")
	float Energy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Energy")
	float DefaultEnergy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Energy")
	float MaxEnergy;

protected:

	virtual void BeginPlay() override;
};
