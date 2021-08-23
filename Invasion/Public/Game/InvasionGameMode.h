// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/CharacterExecutionScriptData.h"
#include "Data/PlayerHealthConfiguration.h"
#include "Data/PlayerEnergyConfiguration.h"
#include "InvasionGameMode.generated.h"

class ATimeDilationSystem;
class ACoverSystem;
class APostProcessSystem;

UCLASS()
class INVASION_API AInvasionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/** The execution move info */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UCharacterExecutionScriptData* ExecutionData;

	/** The player health config */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (DisplayName = "Player Health Configuration"))
	UPlayerHealthConfiguration* PlayerHealthConfig;

	/** The player energy config */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (DisplayName = "Player Energy Configuration"))
	UPlayerEnergyConfiguration* PlayerEnergyConfig;
	
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
