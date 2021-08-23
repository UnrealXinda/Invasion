// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvasionEnums.h"
#include "InvasionMacros.h"
#include "Interfaces/InvasionTick.h"
#include "CoverSystem.generated.h"

UCLASS()
class INVASION_API ACoverSystem : public AActor, public IInvasionTick
{
	GENERATED_BODY()

	REDIRECT_TICK_FUNC()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeGroup)
	ETimeGroup TimeGroup;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimeGroup)
	float InvasionTimeDilation = 1.0F;
	
public:	

	ACoverSystem();

	UFUNCTION(BlueprintPure)
	const TArray<class ACoverVolume*>& GetAllCoverVolumes() const;

	UFUNCTION(BlueprintPure)
	const TArray<class ACoverVolume*>& GetAllLowCoverVolumes() const;

	UFUNCTION(BlueprintPure)
	const TArray<class ACoverVolume*>& GetAllHighCoverVolumes() const;

	UFUNCTION(BlueprintPure)
	TArray<class ACoverVolume*> GetAvailableLowCoverVolumes(FVector TargetLoc) const;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Covers)
	TArray<class ACoverVolume*> AllCoverVolumes;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Covers)
	TArray<class ACoverVolume*> AllLowCoverVolumes;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Covers)
	TArray<class ACoverVolume*> AllHighCoverVolumes;
};
