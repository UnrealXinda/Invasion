// Fill out your copyright notice in the Description page of Project Settings.


#include "InvasionGameplayStatics.h"

#include "Game/InvasionGameMode.h"
#include "Game/InvasionGameState.h"

#include "Player/InvasionPlayerState.h"
#include "Player/InvasionPlayerController.h"
#include "Player/InvasionPlayerCameraManager.h"

#include "Systems/CoverSystem.h"

#include "Characters/InvasionCharacter.h"
#include "Characters/InvasionPlayerCharacter.h"

#include "Weapons/ExecutionDamageType.h"

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
	if (APostProcessSystem* System = GetPostProcessSystem(WorldContextObject))
	{
		System->PlayPostProcessEffect(Type, OnFinishedCallback, PlaybackCurve, HandlerType, PlaybackRate, bAffectedByGlobalTimeDilation);
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
	if (APostProcessSystem* System = GetPostProcessSystem(WorldContextObject))
	{
		System->PlayPostProcessScalarSetting(Type, OnFinishedCallback, PlaybackCurve, HandlerType, PlaybackRate, bAffectedByGlobalTimeDilation);
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
	if (APostProcessSystem* System = GetPostProcessSystem(WorldContextObject))
	{
		System->PlayPostProcessVectorSetting(Type, OnFinishedCallback, PlaybackCurve, HandlerType, PlaybackRate, bAffectedByGlobalTimeDilation);
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
	if (APostProcessSystem* System = GetPostProcessSystem(WorldContextObject))
	{
		System->PlayPostProcessColorSetting(Type, OnFinishedCallback, PlaybackCurve, HandlerType, PlaybackRate, bAffectedByGlobalTimeDilation);
	}
}

void UInvasionGameplayStatics::SetPostProcessEffectControlAmount(
	const UObject*         WorldContextObject,
	EPostProcessEffectType Type,
	float                  ControlAmount,
	EPlaybackInterruptType HandlerType
)
{
	if (APostProcessSystem* System = GetPostProcessSystem(WorldContextObject))
	{
		System->SetPostProcessEffectControlAmount(Type, ControlAmount, HandlerType);
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
	if (APostProcessSystem* System = GetPostProcessSystem(WorldContextObject))
	{
		System->SetPostProcessSettingVectorValue(Type, Value, HandlerType);
	}
}

void UInvasionGameplayStatics::SetPostProcessSettingColorValue(
	const UObject*               WorldContextObject,
	EPostProcessColorSettingType Type,
	FLinearColor                 Value,
	EPlaybackInterruptType       HandlerType
)
{
	if (APostProcessSystem* System = GetPostProcessSystem(WorldContextObject))
	{
		System->SetPostProcessSettingColorValue(Type, Value, HandlerType);
	}
}

void UInvasionGameplayStatics::PlayTimeDilationCurveOnGroup(
	const UObject*                                 WorldContextObject,
	UCurveFloat*                                   PlaybackCurve,
	ETimeGroup                                     AffectedTimeGroup,
	const FOnTimeDilationPlaybackFinishedDelegate& OnFinishedCallback,
	EPlaybackInterruptType                         InterruptType,
	float                                          PlaybackRate,
	bool                                           bAffectLowerGroup
)
{
	if (ATimeDilationSystem* System = GetTimeDilationSystem(WorldContextObject))
	{
		System->PlayTimeDilationCurveOnGroup(PlaybackCurve, AffectedTimeGroup, OnFinishedCallback, InterruptType, PlaybackRate, bAffectLowerGroup);
	}
}

void UInvasionGameplayStatics::SetTimeDilationValueOnGroup(
	const UObject*          WorldContextObject,
	float                   TimeDilation,
	ETimeGroup              AffectedTimeGroup,
	EPlaybackInterruptType  InterruptType,
	bool                    bAffectLowerGroup
)
{
	if (ATimeDilationSystem* System = GetTimeDilationSystem(WorldContextObject))
	{
		System->SetTimeDilationValueOnGroup(TimeDilation, AffectedTimeGroup, InterruptType, bAffectLowerGroup);
	}
}

float UInvasionGameplayStatics::GetTimeDilation(const UObject* WorldContextObject, ETimeGroup TimeGroup)
{
	if (ATimeDilationSystem* System = GetTimeDilationSystem(WorldContextObject))
	{
		return System->GetTimeDilation(TimeGroup);
	}

	return UGameplayStatics::GetGlobalTimeDilation(WorldContextObject);
}

void UInvasionGameplayStatics::ApplyExecutionDamage(AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser)
{
	UGameplayStatics::ApplyDamage(DamagedActor, BaseDamage, EventInstigator, DamageCauser, TSubclassOf<UExecutionDamageType>(UExecutionDamageType::StaticClass()));
}

bool UInvasionGameplayStatics::RaycastTest(
	const UObject*         WorldContextObject,
	const FVector&         Start,
	const FVector&         End,
	const TArray<AActor*>& IgnoredActors,
	FVector&               OutHitLoc,
	bool                   bTraceComplex,
	bool                   bDebugDraw
)
{
	if (!WorldContextObject)
	{
		return false;
	}

	FHitResult HitResult;
	bool bHit;

	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (bDebugDraw)
		{
			bHit = UKismetSystemLibrary::LineTraceSingle(WorldContextObject->GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECC_Visibility),
				bTraceComplex, IgnoredActors, EDrawDebugTrace::ForOneFrame, HitResult, true);
		}
		else
		{
			FCollisionQueryParams CollisionParams;
			CollisionParams.bTraceComplex = bTraceComplex;
			CollisionParams.AddIgnoredActors(IgnoredActors);
			bHit = WorldContextObject->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
		}

		if (bHit)
		{
			OutHitLoc = HitResult.Location;
		}

		return bHit;
	}

	return false;
}
