// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyComponent.h"

// Sets default values for this component's properties
UEnergyComponent::UEnergyComponent()
{
	DefaultEnergy = 0.0f;
	MaxEnergy = 100.0f;
}

void UEnergyComponent::CostEnergy(float CostAmount)
{
	if (GetOwner() == UInvasionGameplayStatics::GetInvasionPlayerCharacter(GetWorld()))
	{
		if (InvasionDebug::g_PlayerInfiniteEnergy)
		{
			return;
		}
	}

	if (CostAmount >= 0.0f)
	{
		float ActualCost = FMath::Min(CostAmount, Energy);
		Energy -= ActualCost;

		OnEnergyChanged.Broadcast(this, -ActualCost);
	}
}

void UEnergyComponent::GainEnergy(float GainAmount)
{
	if (GainAmount > 0.0f)
	{
		float ActualGain = FMath::Min(GainAmount, MaxEnergy - Energy);
		Energy += ActualGain;

		OnEnergyChanged.Broadcast(this, ActualGain);
	}
}

float UEnergyComponent::GetEnergy() const
{
	return Energy;
}

float UEnergyComponent::GetDefaultEnergy() const
{
	return DefaultEnergy;
}

float UEnergyComponent::GetMaxEnergy() const
{
	return MaxEnergy;
}

// Called when the game starts
void UEnergyComponent::BeginPlay()
{
	Super::BeginPlay();

	Energy = DefaultEnergy;
}
