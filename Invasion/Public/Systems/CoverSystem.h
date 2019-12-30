// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverSystem.generated.h"

UCLASS()
class INVASION_API ACoverSystem : public AActor
{
	GENERATED_BODY()
	
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
