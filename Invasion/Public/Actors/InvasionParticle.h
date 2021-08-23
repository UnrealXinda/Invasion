// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Invasion.h"
#include "InvasionEnums.h"
#include "Interfaces/InvasionTick.h"
#include "GameFramework/Actor.h"
#include "InvasionParticle.generated.h"

UCLASS()
class INVASION_API AInvasionParticle : public AActor, public IInvasionTick
{
	GENERATED_BODY()

	REDIRECT_TICK_FUNC()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeGroup)
	ETimeGroup TimeGroup;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimeGroup)
	float InvasionTimeDilation = 1.0f;

public:	

	AInvasionParticle();

};
