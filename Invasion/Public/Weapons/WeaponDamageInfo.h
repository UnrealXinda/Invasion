// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDamageInfo.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDamageInfoEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EPhysicalSurface> PhysicalSurface;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveFloat* DamagePerDistanceCurve;
};

UCLASS(BlueprintType)
class INVASION_API UWeaponDamageInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
	TArray<FWeaponDamageInfoEntry> DamageConfig;
};
