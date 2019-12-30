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

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Covers)
	TArray<class ACoverVolume*> AllCoverVolumes;
};
