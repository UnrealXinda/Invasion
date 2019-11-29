// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeDilationSystem.generated.h"

UCLASS()
class INVASION_API ATimeDilationSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeDilationSystem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	float GetTimeDilation(ETimeGroup TimeGroup);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
