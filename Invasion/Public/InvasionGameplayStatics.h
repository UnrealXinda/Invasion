// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/NoExportTypes.h"
#include "Invasion.h"
#include "InvasionGameplayStatics.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API UInvasionGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()	

public:

	/** Returns true if this is being run from an editor preview */
	UFUNCTION(BlueprintPure, Category = InvasionGameMode)
	static bool IsInEditor();

	/** Returns the pointer to the Invasion game mode */
	UFUNCTION(BlueprintPure, Category = InvasionGameMode, meta = (WorldContext = "WorldContextObject"))
	static class AInvasionGameMode* GetInvasionGameMode(const UObject* WorldContextObject);

	/** Returns the pointer to the Invasion game state */
	UFUNCTION(BlueprintPure, Category = InvasionGameMode, meta = (WorldContext = "WorldContextObject"))
	static class AInvasionGameState* GetInvasionGameState(const UObject* WorldContextObject);

	/** Returns the pointer to the Invasion player controller */
	UFUNCTION(BlueprintPure, Category = InvasionGameMode, meta = (WorldContext = "WorldContextObject"))
	static class AInvasionPlayerController* GetInvasionPlayerController(const UObject* WorldContextObject);

	/** Returns the pointer to the Invasion player character */
	UFUNCTION(BlueprintPure, Category = InvasionGameMode, meta = (WorldContext = "WorldContextObject"))
	static class AInvasionPlayerCharacter* GetInvasionPlayerCharacter(const UObject* WorldContextObject);

	/** Returns the pointer to the Invasion player camera manager */
	UFUNCTION(BlueprintPure, Category = InvasionGameMode, meta = (WorldContext = "WorldContextObject"))
	static class AInvasionPlayerCameraManager* GetInvasionPlayerCameraManager(const UObject* WorldContextObject);

#pragma region Systems

	/** Returns the pointer to the time dilation system if any */
	UFUNCTION(BlueprintPure, Category = InvasionSystems, meta = (WorldContext = "WorldContextObject"))
	static class ATimeDilationSystem* GetTimeDilationSystem(const UObject* WorldContextObject);

	/** Returns the pointer to the time dilation system if any */
	UFUNCTION(BlueprintPure, Category = InvasionSystems, meta = (WorldContext = "WorldContextObject"))
	static class ACoverSystem* GetCoverSystem(const UObject* WorldContextObject);

#pragma endregion Systems

#pragma region Miscellaneous

	/** Check to see if a given angle is inside the range of the smaller angle formed by two angles */
	UFUNCTION(BlueprintPure, Category = Miscellaneous)
	static bool IsAngleInsideRange(float Angle, float Min, float Max);

#pragma endregion Miscellaneous

};
