// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/NoExportTypes.h"
#include "Invasion.h"
#include "Systems/PostProcessSystem.h"
#include "Systems/TimeDilationSystem.h"
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

	/** Returns the pointer to the cover system if any */
	UFUNCTION(BlueprintPure, Category = InvasionSystems, meta = (WorldContext = "WorldContextObject"))
	static class ACoverSystem* GetCoverSystem(const UObject* WorldContextObject);

	/** Returns the pointer to the post process system if any */
	UFUNCTION(BlueprintPure, Category = InvasionSystems, meta = (WorldContext = "WorldContextObject"))
	static class APostProcessSystem* GetPostProcessSystem(const UObject* WorldContextObject);

#pragma endregion Systems

#pragma region Post Process System Helper

	// Play an effect means playing it over a period of time. This is in contrast to directly setting
	// an effect's control amount
	UFUNCTION(BlueprintCallable, Category = InvasionPostProcessSystem, meta = (WorldContext = "WorldContextObject"))
	static void PlayPostProcessEffect(
		const UObject*                                      WorldContextObject,
		EPostProcessEffectType                              Type,
		const FOnPostProcessEffectPlaybackFinishedDelegate& OnFinishedCallback,
		class UCurveFloat*                                  PlaybackCurve,
		EPlaybackInterruptType                              HandlerType = EPlaybackInterruptType::DropThis,
		float                                               PlaybackRate = 1.0F,
		bool                                                bAffectedByGlobalTimeDilation = true
	);

	UFUNCTION(BlueprintCallable, Category = InvasionPostProcessSystem, meta = (WorldContext = "WorldContextObject"))
	static void PlayPostProcessScalarSetting(
		const UObject*                                             WorldContextObject,
		EPostProcessScalarSettingType                              Type,
		const FOnPostProcessScalarSettingPlaybackFinishedDelegate& OnFinishedCallback,
		class UCurveFloat*                                         PlaybackCurve,
		EPlaybackInterruptType                                     HandlerType = EPlaybackInterruptType::DropThis,
		float                                                      PlaybackRate = 1.0F,
		bool                                                       bAffectedByGlobalTimeDilation = true
	);

	UFUNCTION(BlueprintCallable, Category = InvasionPostProcessSystem, meta = (WorldContext = "WorldContextObject"))
	static void PlayPostProcessVectorSetting(
		const UObject*                                             WorldContextObject,
		EPostProcessVectorSettingType                              Type,
		const FOnPostProcessVectorSettingPlaybackFinishedDelegate& OnFinishedCallback,
		class UCurveLinearColor*                                   PlaybackCurve,
		EPlaybackInterruptType                                     HandlerType = EPlaybackInterruptType::DropThis,
		float                                                      PlaybackRate = 1.0F,
		bool                                                       bAffectedByGlobalTimeDilation = true
	);

	UFUNCTION(BlueprintCallable, Category = InvasionPostProcessSystem, meta = (WorldContext = "WorldContextObject"))
	static void PlayPostProcessColorSetting(
		const UObject*                                            WorldContextObject,
		EPostProcessColorSettingType                              Type,
		const FOnPostProcessColorSettingPlaybackFinishedDelegate& OnFinishedCallback,
		class UCurveLinearColor*                                  PlaybackCurve,
		EPlaybackInterruptType                                    HandlerType = EPlaybackInterruptType::DropThis,
		float                                                     PlaybackRate = 1.0F,
		bool                                                      bAffectedByGlobalTimeDilation = true
	);

	UFUNCTION(BlueprintCallable, Category = InvasionPostProcessSystem, meta = (WorldContext = "WorldContextObject"))
	static void SetPostProcessEffectControlAmount(
		const UObject*         WorldContextObject,
		EPostProcessEffectType Type,
		float                  ControlAmount,
		EPlaybackInterruptType HandlerType = EPlaybackInterruptType::Override
	);

	UFUNCTION(BlueprintCallable, Category = InvasionPostProcessSystem, meta = (WorldContext = "WorldContextObject"))
	static void SetPostProcessSettingScalarValue(
		const UObject*                WorldContextObject,
		EPostProcessScalarSettingType Type,
		float                         Value,
		EPlaybackInterruptType        HandlerType = EPlaybackInterruptType::Override
	);

	UFUNCTION(BlueprintCallable, Category = InvasionPostProcessSystem, meta = (WorldContext = "WorldContextObject"))
	static void SetPostProcessSettingVectorValue(
		const UObject*                WorldContextObject,
		EPostProcessVectorSettingType Type,
		FVector4                      Value,
		EPlaybackInterruptType        HandlerType = EPlaybackInterruptType::Override
	);

	UFUNCTION(BlueprintCallable, Category = InvasionPostProcessSystem, meta = (WorldContext = "WorldContextObject"))
	static void SetPostProcessSettingColorValue(
		const UObject*               WorldContextObject,
		EPostProcessColorSettingType Type,
		FLinearColor                 Value,
		EPlaybackInterruptType       HandlerType = EPlaybackInterruptType::Override
	);

#pragma endregion Post Process System Helper

#pragma region Time Dilation System Helper

	UFUNCTION(BlueprintCallable, Category = TimeDilationSystem, meta = (WorldContext = "WorldContextObject"))
	static void PlayTimeDilationCurveOnGroup(
		const UObject*                                 WorldContextObject,
		UCurveFloat*                                   PlaybackCurve,
		ETimeGroup                                     AffectedTimeGroup,
		const FOnTimeDilationPlaybackFinishedDelegate& OnFinishedCallback,
		EPlaybackInterruptType                         InterruptType = EPlaybackInterruptType::DropThis,
		float                                          PlaybackRate = 1.0F,
		bool                                           bAffectLowerGroup = true
	);

	UFUNCTION(BlueprintCallable, Category = TimeDilationSystem, meta = (WorldContext = "WorldContextObject"))
	static void SetTimeDilationValueOnGroup(
		const UObject*          WorldContextObject,
		float                   TimeDilation,
		ETimeGroup              AffectedTimeGroup,
		EPlaybackInterruptType  InterruptType = EPlaybackInterruptType::Override,
		bool                    bAffectLowerGroup = true
	);

	UFUNCTION(BlueprintPure, Category = TimeDilationSystem, meta = (WorldContext = "WorldContextObject"))
	static float GetTimeDilation(const UObject* WorldContextObject, ETimeGroup TimeGroup);

#pragma endregion Time Dilation System Helper

#pragma region Gameplay

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	static void ApplyExecutionDamage(AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser);

#pragma endregion Gameplay

	UFUNCTION(BlueprintCallable, Category = Utils, meta = (WorldContext = "WorldContextObject"))
	static bool RaycastTest(
		const UObject*         WorldContextObject,
		const FVector&         Start,
		const FVector&         End,
		const TArray<AActor*>& IgnoredActors,
		FVector&               OutHitLoc,
		bool                   bTraceComplex = true,
		bool                   bDebugDraw = false
	);

};
