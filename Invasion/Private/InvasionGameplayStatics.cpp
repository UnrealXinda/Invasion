// Fill out your copyright notice in the Description page of Project Settings.


#include "InvasionGameplayStatics.h"

#include "Game/InvasionGameMode.h"
#include "Game/InvasionGameState.h"

#include "Player/InvasionPlayerState.h"
#include "Player/InvasionPlayerController.h"
#include "Player/InvasionPlayerCameraManager.h"

#include "Systems/TimeDilationSystem.h"
#include "Systems/CoverSystem.h"
#include "Systems/PostProcessSystem.h"

#include "Characters/InvasionCharacter.h"
#include "Characters/InvasionPlayerCharacter.h"

#include "Kismet/GameplayStatics.h"

bool UInvasionGameplayStatics::IsInEditor()
{
	return GIsEditor;
}

AInvasionGameMode* UInvasionGameplayStatics::GetInvasionGameMode(const UObject* WorldContextObject)
{
	return Cast<AInvasionGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
}

AInvasionGameState* UInvasionGameplayStatics::GetInvasionGameState(const UObject* WorldContextObject)
{
	return Cast<AInvasionGameState>(UGameplayStatics::GetGameState(WorldContextObject));
}

AInvasionPlayerController* UInvasionGameplayStatics::GetInvasionPlayerController(const UObject* WorldContextObject)
{
	return Cast<AInvasionPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

AInvasionPlayerCharacter* UInvasionGameplayStatics::GetInvasionPlayerCharacter(const UObject* WorldContextObject)
{
	return Cast<AInvasionPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
}

AInvasionPlayerCameraManager* UInvasionGameplayStatics::GetInvasionPlayerCameraManager(const UObject* WorldContextObject)
{
	AInvasionPlayerController* Controller = GetInvasionPlayerController(WorldContextObject);
	return Controller ? Cast<AInvasionPlayerCameraManager>(Controller->PlayerCameraManager) : nullptr;
}

ATimeDilationSystem* UInvasionGameplayStatics::GetTimeDilationSystem(const UObject* WorldContextObject)
{
	AInvasionGameMode* CurrentGameMode = GetInvasionGameMode(WorldContextObject);
	return CurrentGameMode ? CurrentGameMode->GetTimeDilationSystem() : nullptr;
}

ACoverSystem* UInvasionGameplayStatics::GetCoverSystem(const UObject* WorldContextObject)
{
	AInvasionGameMode* CurrentGameMode = GetInvasionGameMode(WorldContextObject);
	return CurrentGameMode ? CurrentGameMode->GetCoverSystem() : nullptr;
}

APostProcessSystem* UInvasionGameplayStatics::GetPostProcessSystem(const UObject* WorldContextObject)
{
	AInvasionGameMode* CurrentGameMode = GetInvasionGameMode(WorldContextObject);
	return CurrentGameMode ? CurrentGameMode->GetPostProcessSystem() : nullptr;
}

void UInvasionGameplayStatics::PlayPostProcessEffect(
	const UObject*                                      WorldContextObject,
	EPostProcessEffectType                              Type,
	const FOnPostProcessEffectPlaybackFinishedDelegate& OnFinishedCallback,
	UCurveFloat*                                        PlaybackCurve,
	EPlaybackInterruptType                              HandlerType,
	float                                               PlaybackRate,
	bool                                                bAffectedByGlobalTimeDilation
)
{
	if (APostProcessSystem* PostProcessSystem = GetPostProcessSystem(WorldContextObject))
	{
		PostProcessSystem->PlayPostProcessEffect(Type, OnFinishedCallback, PlaybackCurve, HandlerType, PlaybackRate, bAffectedByGlobalTimeDilation);
	}
}

void UInvasionGameplayStatics::PlayPostProcessScalarSetting(
	const UObject*                                             WorldContextObject,
	EPostProcessScalarSettingType                              Type,
	const FOnPostProcessScalarSettingPlaybackFinishedDelegate& OnFinishedCallback,
	UCurveFloat*                                               PlaybackCurve,
	EPlaybackInterruptType                                     HandlerType,
	float                                                      PlaybackRate,
	bool                                                       bAffectedByGlobalTimeDilation
)
{
	if (APostProcessSystem* PostProcessSystem = GetPostProcessSystem(WorldContextObject))
	{
		PostProcessSystem->PlayPostProcessScalarSetting(Type, OnFinishedCallback, PlaybackCurve, HandlerType, PlaybackRate, bAffectedByGlobalTimeDilation);
	}
}

void UInvasionGameplayStatics::PlayPostProcessVectorSetting(
	const UObject*                                             WorldContextObject,
	EPostProcessVectorSettingType                              Type,
	const FOnPostProcessVectorSettingPlaybackFinishedDelegate& OnFinishedCallback,
	UCurveLinearColor*                                         PlaybackCurve,
	EPlaybackInterruptType                                     HandlerType,
	float                                                      PlaybackRate,
	bool                                                       bAffectedByGlobalTimeDilation
)
{
	if (APostProcessSystem* PostProcessSystem = GetPostProcessSystem(WorldContextObject))
	{
		PostProcessSystem->PlayPostProcessVectorSetting(Type, OnFinishedCallback, PlaybackCurve, HandlerType, PlaybackRate, bAffectedByGlobalTimeDilation);
	}
}

void UInvasionGameplayStatics::PlayPostProcessColorSetting(
	const UObject*                                            WorldContextObject,
	EPostProcessColorSettingType                              Type,
	const FOnPostProcessColorSettingPlaybackFinishedDelegate& OnFinishedCallback,
	UCurveLinearColor*                                        PlaybackCurve,
	EPlaybackInterruptType                                    HandlerType,
	float                                                     PlaybackRate,
	bool                                                      bAffectedByGlobalTimeDilation
)
{
	if (APostProcessSystem* PostProcessSystem = GetPostProcessSystem(WorldContextObject))
	{
		PostProcessSystem->PlayPostProcessColorSetting(Type, OnFinishedCallback, PlaybackCurve, HandlerType, PlaybackRate, bAffectedByGlobalTimeDilation);
	}
}

void UInvasionGameplayStatics::SetPostProcessEffectControlAmount(
	const UObject*         WorldContextObject,
	EPostProcessEffectType Type,
	float                  ControlAmount,
	EPlaybackInterruptType HandlerType
)
{
	if (APostProcessSystem* PostProcessSystem = GetPostProcessSystem(WorldContextObject))
	{
		PostProcessSystem->SetPostProcessEffectControlAmount(Type, ControlAmount, HandlerType);
	}
}

void UInvasionGameplayStatics::SetPostProcessSettingScalarValue(
	const UObject*                WorldContextObject,
	EPostProcessScalarSettingType Type,
	float                         Value,
	EPlaybackInterruptType        HandlerType
)
{
	if (APostProcessSystem* PostProcessSystem = GetPostProcessSystem(WorldContextObject))
	{
		PostProcessSystem->SetPostProcessSettingScalarValue(Type, Value, HandlerType);
	}
}

void UInvasionGameplayStatics::SetPostProcessSettingVectorValue(
	const UObject*                WorldContextObject,
	EPostProcessVectorSettingType Type,
	FVector4                      Value,
	EPlaybackInterruptType        HandlerType
)
{
	if (APostProcessSystem* PostProcessSystem = GetPostProcessSystem(WorldContextObject))
	{
		PostProcessSystem->SetPostProcessSettingVectorValue(Type, Value, HandlerType);
	}
}

void UInvasionGameplayStatics::SetPostProcessSettingColorValue(
	const UObject*               WorldContextObject,
	EPostProcessColorSettingType Type,
	FLinearColor                 Value,
	EPlaybackInterruptType       HandlerType
)
{
	if (APostProcessSystem* PostProcessSystem = GetPostProcessSystem(WorldContextObject))
	{
		PostProcessSystem->SetPostProcessSettingColorValue(Type, Value, HandlerType);
	}
}

bool UInvasionGameplayStatics::IsAngleInsideRange(float Angle, float Min, float Max)
{
	FVector MinVec = FRotator(0.0f, Min, 0.0f).Vector();
	FVector MaxVec = FRotator(0.0f, Max, 0.0f).Vector();
	FVector CurrentVec = FRotator(0.0f, Angle, 0.0f).Vector();

	bool bOnSameSideOfMin = ((MinVec ^ CurrentVec).Z * (MinVec ^ MaxVec).Z > 0);
	bool bOnSameSideOfMax = ((MaxVec ^ CurrentVec).Z * (MaxVec ^ MinVec).Z > 0);

	return bOnSameSideOfMin && bOnSameSideOfMax;
}
