// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100.0f;
}


void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage >= 0.0f)
	{
		float ActualDamage = FMath::Min(Damage, Health);
		Health -= ActualDamage;

		OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
	}
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
	}

	Health = DefaultHealth;
}


