// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/InvasionWeapon.h"
#include "InvasionRifle.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API AInvasionRifle : public AInvasionWeapon
{
	GENERATED_BODY()

public:

	AInvasionRifle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Fire() override;

	virtual void StartFire() override;

	virtual void StopFire() override;

	void AddRecoilToController(class APlayerController* Controller) const;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<class UCameraShake> CameraShakeClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	/** Used for simulating recoil for AI characters when using this rifle. 
	/** Bullet spread in degrees */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UCurveFloat* RandomConeHalfDegreesPerDistance;

	/** The last game time when fired */
	float LastFireTime;

	/** Timer handle for scheduling automatic fire */
	FTimerHandle ShotIntervalTimerHandle;

	/** Random stream for RNG */
	FRandomStream RandomStream;

protected:

	void PlayFireEffects();
	void PlayMuzzleEffect();
	void PlayImpactEffect(FVector Location, FRotator Rotation, EPhysicalSurface SurfaceType);
	void PlayTracerEffect(FVector TargetPoint);
	void PlayCameraShakeEffect();
};
