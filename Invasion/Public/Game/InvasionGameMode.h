// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/CharacterExecutionScriptData.h"
#include "InvasionGameMode.generated.h"

class ATimeDilationSystem;
class ACoverSystem;
class APostProcessSystem;

UCLASS()
class INVASION_API AInvasionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/** Maximum distance between player and victim when trying to melee execute */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0), Category = "Game Settings")
	float MaximumExecutionDistance;

	/** Time before player can self recover health when taking damage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0), Category = "Player Health Settings")
	float TimeBeforeSelfRecover;

	/** Health points recovered per second */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0), Category = "Player Health Settings")
	float SelfRecoverPerSecond;
	
public:

	AInvasionGameMode();

	ATimeDilationSystem* GetTimeDilationSystem() const;

	ACoverSystem* GetCoverSystem() const;

	APostProcessSystem* GetPostProcessSystem() const;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Systems)
	TSubclassOf<ATimeDilationSystem> TimeDilationSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Systems)
	TSubclassOf<ACoverSystem> CoverSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Systems)
	TSubclassOf<APostProcessSystem> PostProcessSystemClass;

	/** The execution move info */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	UCharacterExecutionScriptData* ExecutionData;

#pragma region System Runtime Instances

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Systems)
	ATimeDilationSystem* TimeDilationSystem;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Systems)
	ACoverSystem* CoverSystem;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Systems)
	APostProcessSystem* PostProcessSystem;

#pragma endregion System Runtime Instances

protected:

	UFUNCTION(BlueprintPure)
	const FExecutionScript& GetRandomExecutionScript() const;
};
