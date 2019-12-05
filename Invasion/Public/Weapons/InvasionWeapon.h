// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvasionWeapon.generated.h"

USTRUCT(BlueprintType)
struct FZoomInfo
{
	GENERATED_BODY()

	/** FOV by default */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 0.0, ClampMax = 90.0))
	float DefaultFOV;

	/** FOV when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 0.0, ClampMax = 90.0))
	float ZoomedFOV;

	/** Minimum pitch when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = -90.0, ClampMax = 90.0))
	float ZoomedPitchMin;

	/** Maximum pitch when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = -90.0, ClampMax = 90.0))
	float ZoomedPitchMax;

	/** Interpolation speed while aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 0.0, ClampMax = 100.0))
	float ZoomInterpSpeed;
};

UCLASS()
class INVASION_API AInvasionWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInvasionWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual void StartFire();

	UFUNCTION(BlueprintCallable)
	virtual void StopFire();

public:

	/** FOV when aiming */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FZoomInfo ZoomInfo;

	/** The maximum range of this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float TraceRangeMax;

	/** The maximum range of this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RangeMax;

	/** The min pitch input for recoil of this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RecoilPitchMin;

	/** The max pitch input for recoil of this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RecoilPitchMax;

	/** The yaw input for recoil of this weapon, the range is [-RecoilYawMax, RecoilYawMax] */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RecoilYawMax;

	/** The base damage of the weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float BaseDamage;

	/** The fatal damage of the weapon, such as committing a headshot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float FatalDamage;

	/** RPM - Bullets per minute fired */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RateOfFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class UDamageType> DamageType;

};
