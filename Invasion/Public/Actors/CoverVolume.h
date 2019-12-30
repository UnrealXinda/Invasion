// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvasionEnums.h"
#include "GameFramework/Actor.h"
#include "CoverVolume.generated.h"

UCLASS()
class INVASION_API ACoverVolume : public AActor
{
	GENERATED_BODY()

public:	

	/** The type of this cover mesh actor. Specifying whether it's a low/high cover */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Cover")
	ECoverType CoverType;

	/** The minimum aiming yaw angle offset from the cover center line when taking this cover */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Cover", meta = (ClampMin = "-90.0", ClampMax = "0.0"))
	float AimingYawMin;

	/** The maximum aiming yaw angle offset from the cover center line when taking this cover */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Cover", meta = (ClampMin = "0.0", ClampMax = "90.0"))
	float AimingYawMax;
	
public:	
	// Sets default values for this actor's properties
	ACoverVolume();

	FORCEINLINE class UBoxComponent* GetCoverComponent() const { return CoverComp; }

	FORCEINLINE class UBoxComponent* GetLeftBlockVolumeComp() const { return LeftBlockVolumeComp; }

	FORCEINLINE class UBoxComponent* GetRightBlockVolumeComp() const { return RightBlockVolumeComp; }

	bool HasActorReachedLeftEdge(const AActor* Actor) const;

	bool HasActorReachedRightEdge(const AActor* Actor) const;

	// TODO: add callback to show world space widget to inform the cover is available for taking


protected:

	/** The box component that specifies the area inside which the cover can be taken */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* CoverComp;

	/** The left bounding volume that regulates the player's movement while taking cover */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* LeftBlockVolumeComp;

	/** The left bounding volume that regulates the player's movement while taking cover */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* RightBlockVolumeComp;
};
