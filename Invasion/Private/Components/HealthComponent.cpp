// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Weapons/ExecutionDamageType.h"

UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100.0f;
	MaxHealth = 100.0f;
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount > 0.0f)
	{
		float ActualHeal = FMath::Min(HealAmount, MaxHealth - Health);
		Health += ActualHeal;

		OnHealthChanged.Broadcast(this, Health, ActualHeal, nullptr, nullptr, nullptr);
	}
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

float UHealthComponent::GetDefaultHealth() const
{
	return DefaultHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage >= 0.0f)
	{
		float ActualDamage = FMath::Min(Damage, Health);
		Health -= ActualDamage;

		OnHealthChanged.Broadcast(this, Health, -ActualDamage, DamageType, InstigatedBy, DamageCauser);

		if (Health == 0.0f)
		{
			if (DamageType->IsA<UExecutionDamageType>())
			{
				OnCharacterExecuted.Broadcast(this, ActualDamage, InstigatedBy, DamageCauser);
			}

			else
			{
				OnCharacterDeath.Broadcast(this, ActualDamage, DamageType, InstigatedBy, DamageCauser);
			}
		}
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
	}

	Health = DefaultHealth;
}


