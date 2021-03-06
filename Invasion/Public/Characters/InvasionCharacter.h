// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Invasion.h"
#include "InvasionEnums.h"
#include "Interfaces/InvasionTick.h"
#include "GameFramework/Character.h"
#include "InvasionCharacter.generated.h"

USTRUCT(BlueprintType)
struct INVASION_API FWeaponAnimation
{
	GENERATED_BODY()

	/** The type of the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType WeaponType;

	/** Animation montage to play when firing the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* FireMontage;
};

UCLASS()
class INVASION_API AInvasionCharacter : public ACharacter, public IInvasionTick
{
	GENERATED_BODY()

	REDIRECT_TICK_FUNC()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeGroup)
	ETimeGroup TimeGroup;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimeGroup)
	float InvasionTimeDilation = 1.0f;

public:

	/** The movement state of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = States)
	EMoveState MoveState;

	/** The aim state of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = States)
	EAimState AimState;

	/** The cover state of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = States)
	ECoverState CoverState;

	/** The last movement direction of the character. Used for animation blending */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = States)
	EMoveDirection LastMovementDir;

	/** The current weapon used by the character */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Weapon)
	class AInvasionWeapon* CurrentWeapon;

	/** Initial class that the player owns when begin play */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<class AInvasionWeapon> StarterWeaponClass;

	/** Socket name that the weapon is attached to the character mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName WeaponSocketName;

	/** The current cover volume that the character is taking */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Cover)
	class ACoverVolume* CurrentCoverVolume;

public:

	// Sets default values for this character's properties
	AInvasionCharacter();

	UFUNCTION(BlueprintPure)
	virtual bool CanMove() const;

	UFUNCTION(BlueprintPure)
	virtual bool CanSprint() const;

	UFUNCTION(BlueprintPure)
	virtual bool CanAim() const;

	UFUNCTION(BlueprintPure)
	virtual bool CanFire() const;

	UFUNCTION(BlueprintPure)
	virtual bool CanTakeCover() const;

	UFUNCTION(BlueprintCallable)
	virtual void StartFire();

	UFUNCTION(BlueprintCallable)
	virtual void StopFire();

	UFUNCTION(BlueprintCallable)
	virtual bool EquipWeapon(class AInvasionWeapon* Weapon);

	UFUNCTION(BlueprintCallable)
	virtual bool UnequipWeapon(class AInvasionWeapon* Weapon);

	virtual void MoveCharacter(FVector WorldDirection, float ScaleValue = 1.0F);

	virtual bool TryTakeCover();

	virtual bool TryUntakeCover();

	FORCEINLINE class UIKComponent* GetIKComponent() const { return IKComp; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TArray<FWeaponAnimation> WeaponAnimations;

	/** The current cover volume that the character is taking */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Cover)
	TArray<class ACoverVolume*> AvailableCoverVolumes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Components)
	class UIKComponent* IKComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Components)
	class UHealthComponent* HealthComp;

	/** The animation used for death (stance) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	class UAnimMontage* DeathMontage;

	/** The animation used for death (cover) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	class UAnimMontage* CoverDeathMontage;

protected:

	virtual void BeginPlay() override;	

	UFUNCTION()
	virtual void OnCapsuleBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor*              OtherActor,
		UPrimitiveComponent* OtherComp,
		int32                OtherBodyIndex,
		bool                 bFromSweep,
		const FHitResult&    SweepResult
	);

	UFUNCTION()
	virtual void OnCapsuleEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor*              OtherActor,
		UPrimitiveComponent* OtherComp,
		int32                OtherBodyIndex
	);

	virtual void OnWeaponFire(class AInvasionWeapon* Weapon, class AController* InstigatedBy);

	virtual void OnHealthChanged(
		class UHealthComponent*  HealthComponent,
		float                    Health,
		float                    HealthDelta,
		const class UDamageType* DamageType,
		class AController*       InstigatedBy,
		AActor*                  DamageCauser
	);

	virtual void OnCharacterDeath(
		class UHealthComponent*  HealthComponent,
		float                    LastDamage,
		const class UDamageType* DamageType,
		class AController*       InstigatedBy,
		AActor*                  DamageCauser
	);

private:

	UFUNCTION()
	void OnWeaponFire_Internal(class AInvasionWeapon* Weapon, class AController* InstigatedBy);

	UFUNCTION()
	void OnHealthChanged_Internal(
		class UHealthComponent*  HealthComponent,
		float                    Health,
		float                    HealthDelta,
		const class UDamageType* DamageType,
		class AController*       InstigatedBy,
		AActor*                  DamageCauser
	);

	UFUNCTION()
	void OnCharacterDeath_Internal(
		class UHealthComponent*  HealthComponent,
		float                    LastDamage,
		const class UDamageType* DamageType,
		class AController*       InstigatedBy,
		AActor*                  DamageCauser
	);
};
