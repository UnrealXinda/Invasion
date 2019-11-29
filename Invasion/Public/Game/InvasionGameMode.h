// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InvasionGameMode.generated.h"

class ATimeDilationSystem;

UCLASS()
class INVASION_API AInvasionGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AInvasionGameMode();

	ATimeDilationSystem* GetTimeDilationSystem() const;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Systems)
	TSubclassOf<ATimeDilationSystem> TimeDilationSystemClass;

#pragma region System Runtime Instances

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Systems)
	ATimeDilationSystem* TimeDilationSystem;

#pragma endregion System Runtime Instances

};
