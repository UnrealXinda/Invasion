// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvasionEnums.h"
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

USTRUCT(BlueprintType)
struct FImpactEffect
{
	GENERATED_BODY()

	/** The surface type for this impact effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	/** The particle effect for this impact effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UParticleSystem* ImpactParticleEffect;

	/** The sound to play when hitting the surface */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* ImpactSound;

	/** The volume multiplier of impact sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float VolumeMultiplier;

	/** The pitch multiplier of impact sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float PitchMultiplier;
};

/** Delegate type for callbacks when weapon fires */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnWeaponFireSignature,
	/* The weapon that is firing */
	class AInvasionWeapon*, Weapon,
	/* The controller that instigates this attack */
	class AController*, InstigatedBy
);

/** Delegate type for callbacks when weapon hits something */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnWeaponHitSignature,
	/* The weapon that is firing */
	class AInvasionWeapon*, Weapon,
	/* The controller that instigates this attack */
	class AController*, InstigatedBy,		
	/* The actor that this weapon hit */
	AActor*, HitActor,
	/* The physical surface this weapon hit */
	EPhysicalSurface, PhysicalSurface
);

UCLASS()
class INVASION_API AInvasionWeapon : public AActor
{
	GENERATED_BODY()		

public:

	/** Type of the weapon */
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	EWeaponType WeaponType;

	/** FOV when aiming */
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FZoomInfo ZoomInfo;

	/** The maximum range of this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float TraceRangeMax;

	/** The maximum range of this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float RangeMax;

	/** The min pitch input for recoil of this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float RecoilPitchMin;

	/** The max pitch input for recoil of this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float RecoilPitchMax;

	/** The yaw input for recoil of this weapon, the range is [-RecoilYawMax, RecoilYawMax] */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float RecoilYawMax;

	/** The crosshair zoom in value increased per shot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0, ClampMax = 1.0), Category = Weapon)
	float CrosshairZoomInPerFire;

	/** The crosshair spread recover rate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0), Category = Weapon)
	float CrosshairZoomOutPerSecond;

	/** The damage info of the weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0), Category = Weapon)
	class UWeaponDamageInfo* DamageInfo;

	/** RPM - Bullets per minute fired */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float RateOfFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects)
	TArray<FImpactEffect> ImpactEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<class UCrosshairWidget> CrosshairWidgetClass;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponFireSignature OnWeaponFire;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponHitSignature OnWeaponHit;

public:

	// Sets default values for this actor's properties
	AInvasionWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void EquipWeapon(AActor* OwnerActor, class USceneComponent* ComponentToAttach, FName SocketToAttach = NAME_None);

	UFUNCTION(BlueprintCallable)
	virtual void UnequipWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual void StartFire();

	UFUNCTION(BlueprintCallable)
	virtual void StopFire();

	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponVisibility(bool bVisible);

	UFUNCTION(BlueprintPure)
	virtual bool IsWeaponVisible() const;

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = UI)
	class UCrosshairWidget* CrosshairWidget;

};
