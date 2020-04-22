// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvasionEnums.h"
#include "Engine/DataAsset.h"
#include "PlayerEnergyConfiguration.generated.h"

USTRUCT(BlueprintType)
struct FWeaponEnergyRecoverEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EPhysicalSurface> PhysicalSurface;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RecoverAmount;
};

USTRUCT(BlueprintType)
struct FWeaponEnergyRecover
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWeaponEnergyRecoverEntry> Entries;
};

UCLASS()
class INVASION_API UPlayerEnergyConfiguration : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Energy cost per dash */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0), Category = "Skill Cost")
	float DashCost;

	/** Energy cost per scan */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0), Category = "Skill Cost")
	float ScanCost;

	/** Energy recovery */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0), Category = "Energy Recover")
	TArray<FWeaponEnergyRecover> WeaponRecover;

public:

	UFUNCTION(BlueprintPure)
	float GetEnergyRecoverFromWeapon(EWeaponType WeaponType, EPhysicalSurface PhysicalSurface) const;
};
