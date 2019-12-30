// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/PostProcessSystem.h"
#include "Systems/TimeDilationSystem.h"

#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"

REDIRECT_TICK_FUNC_IMPLEMENTATION(APostProcessSystem)

// Sets default values
APostProcessSystem::APostProcessSystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;

	PostProcessComp = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	RootComponent = PostProcessComp;

	TimeGroup = ETimeGroup::System;
}

// Called when the game starts or when spawned
void APostProcessSystem::BeginPlay()
{
	Super::BeginPlay();

	InstantiatePostProcessEffects();
	InstantiatePostProcessSettings();
}

void APostProcessSystem::InvasionTick_Implementation(float DeltaTime)
{
	UpdatePostProcessEffects(ActiveEffects, DeltaTime);

	UpdatePostProcessSettings(ActiveScalarSettings, DeltaTime);
	UpdatePostProcessSettings(ActiveVectorSettings, DeltaTime);
	UpdatePostProcessSettings(ActiveColorSettings, DeltaTime);
}


void APostProcessSystem::PlayPostProcessEffect(
	EPostProcessEffectType                              Type,
	const FOnPostProcessEffectPlaybackFinishedDelegate& OnFinishedCallback,
	UCurveFloat*                                        PlaybackCurve,
	EPlaybackInterruptType                              HandlerType, /*= DropThis */
	float                                               PlaybackRate, /*= 1.0F */
	bool                                                bAffectedByGlobalTimeDilation /*= true */
)
{
	FPostProcessEffect* Effect = PostProcessEffects.FindByPredicate([Type](const FPostProcessEffect& E) { return E.EffectType == Type; });

	if (Effect && PlaybackCurve)
	{
		switch (HandlerType)
		{
		case EPlaybackInterruptType::DropThis:
			if (ActiveEffects.Contains(Effect))
			{
				return;
			}
			break;

		case EPlaybackInterruptType::Override:
		default:
			if (ActiveEffects.Contains(Effect))
			{
				FinishPostProcessEffectPlayback(*Effect, EPlaybackFinishType::Interrupted);
				ActiveEffects.Remove(Effect);
			}
		}

		float Min, Max;
		PlaybackCurve->GetTimeRange(Min, Max);

		Effect->OnEffectFinishedCallback = OnFinishedCallback;
		Effect->CurrentPlaybackCurve = PlaybackCurve;
		Effect->CurrentTimeline.SetTimelineLength(Max);
		Effect->CurrentTimeline.SetPlayRate(PlaybackRate);
		Effect->CurrentTimeline.PlayFromStart();
		Effect->bAffectedByGlobalTimeDilation = bAffectedByGlobalTimeDilation;
		ActiveEffects.Add(Effect);
	}
}

void APostProcessSystem::PlayPostProcessScalarSetting(
	EPostProcessScalarSettingType                              Type,
	const FOnPostProcessScalarSettingPlaybackFinishedDelegate& OnFinishedCallback,
	UCurveFloat*                                               PlaybackCurve,
	EPlaybackInterruptType                                     HandlerType /*= EPlaybackInterruptType::DropThis*/,
	float                                                      PlaybackRate /*= 1.0F*/,
	bool                                                       bAffectedByGlobalTimeDilation /*= true */
)
{
	TemplatePlayPostProcessSetting(
		Type,
		OnFinishedCallback,
		PlaybackCurve,
		HandlerType,
		PlaybackRate, 
		bAffectedByGlobalTimeDilation, 
		PostProcessScalarSettings, 
		ActiveScalarSettings
	);
}

void APostProcessSystem::PlayPostProcessVectorSetting(
	EPostProcessVectorSettingType                              Type,
	const FOnPostProcessVectorSettingPlaybackFinishedDelegate& OnFinishedCallback,
	UCurveLinearColor*                                         PlaybackCurve,
	EPlaybackInterruptType                                     HandlerType /*= EPlaybackInterruptType::DropThis*/,
	float                                                      PlaybackRate /*= 1.0F*/,
	bool                                                       bAffectedByGlobalTimeDilation /*= true */
)
{
	TemplatePlayPostProcessSetting(
		Type,
		OnFinishedCallback,
		PlaybackCurve,
		HandlerType,
		PlaybackRate,
		bAffectedByGlobalTimeDilation,
		PostProcessVectorSettings,
		ActiveVectorSettings
	);
}

void APostProcessSystem::PlayPostProcessColorSetting(
	EPostProcessColorSettingType                              Type,
	const FOnPostProcessColorSettingPlaybackFinishedDelegate& OnFinishedCallback,
	UCurveLinearColor*                                        PlaybackCurve,
	EPlaybackInterruptType                                    HandlerType /*= EPlaybackInterruptType::DropThis*/,
	float                                                     PlaybackRate /*= 1.0F*/,
	bool                                                      bAffectedByGlobalTimeDilation /*= true */
)
{
	TemplatePlayPostProcessSetting(
		Type,
		OnFinishedCallback,
		PlaybackCurve,
		HandlerType,
		PlaybackRate,
		bAffectedByGlobalTimeDilation,
		PostProcessColorSettings,
		ActiveColorSettings
	);
}

void APostProcessSystem::SetPostProcessEffectControlAmount(
	EPostProcessEffectType Type,
	float                  ControlAmount,
	EPlaybackInterruptType HandlerType /*= Override */
)
{
	FPostProcessEffect* Effect = PostProcessEffects.FindByPredicate([Type](const FPostProcessEffect& E) { return E.EffectType == Type; });

	if (Effect)
	{
		float ClampedControlAmount = FMath::Clamp(ControlAmount, 0.0F, 1.0F);

		switch (HandlerType)
		{
		case EPlaybackInterruptType::DropThis:
			if (ActiveEffects.Contains(Effect))
			{
				return;
			}
			break;

		case EPlaybackInterruptType::Override:
			if (ActiveEffects.Contains(Effect))
			{
				FinishPostProcessEffectPlayback(*Effect, EPlaybackFinishType::Interrupted);
			}
			break;
		}

		SetControlAmountForEffect(*Effect, ClampedControlAmount);
	}
}

void APostProcessSystem::SetPostProcessSettingScalarValue(
	EPostProcessScalarSettingType Type,
	float                         Value,
	EPlaybackInterruptType        HandlerType /*= Override */
)
{
	TemplateSetPostProcessSettingValue(Type, Value, HandlerType, PostProcessScalarSettings, ActiveScalarSettings);
}

void APostProcessSystem::SetPostProcessSettingVectorValue(
	EPostProcessVectorSettingType Type,
	FVector4                      Value,
	EPlaybackInterruptType        HandlerType /*= Override */
)
{
	TemplateSetPostProcessSettingValue(Type, Value, HandlerType, PostProcessVectorSettings, ActiveVectorSettings);
}

void APostProcessSystem::SetPostProcessSettingColorValue(
	EPostProcessColorSettingType Type,
	FLinearColor Value,
	EPlaybackInterruptType HandlerType /*= Override */
)
{
	TemplateSetPostProcessSettingValue(Type, Value, HandlerType, PostProcessColorSettings, ActiveColorSettings);
}

void APostProcessSystem::UpdatePostProcessEffects(TArray<FPostProcessEffect*>& Effects, float DeltaTime)
{
	float GlobalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());

	for (int32 Index = Effects.Num() - 1; Index >= 0; --Index)
	{
		FPostProcessEffect& Effect = *Effects[Index];

		if (!Effect.CurrentPlaybackCurve.IsValid())
		{
			FinishPostProcessEffectPlayback(Effect, EPlaybackFinishType::FailedToPlay);
			Effects.RemoveAtSwap(Index, 1, false);
		}

		else
		{
			float TimelineDeltaTime = DeltaTime * (Effect.bAffectedByGlobalTimeDilation ? GlobalTimeDilation : 1.0F);
			Effect.CurrentTimeline.TickTimeline(TimelineDeltaTime);

			float TimelineValue = Effect.CurrentTimeline.GetPlaybackPosition();
			float TimelineLength = Effect.CurrentTimeline.GetTimelineLength();
			float CurveFloatValue = Effect.CurrentPlaybackCurve->GetFloatValue(TimelineValue);
			float ClampedControlAmount = FMath::Clamp(CurveFloatValue, 0.0F, 1.0F);

			SetControlAmountForEffect(Effect, ClampedControlAmount);

			// Check if the effect playback is finished
			if (TimelineValue >= TimelineLength)
			{
				FinishPostProcessEffectPlayback(Effect, EPlaybackFinishType::Finished);
				Effects.RemoveAtSwap(Index, 1, false);
			}
		}
	}
}

void APostProcessSystem::FinishPostProcessEffectPlayback(FPostProcessEffect& Effect, EPlaybackFinishType FinishType)
{
	Effect.CurrentTimeline.Stop();
	Effect.OnEffectFinishedCallback.ExecuteIfBound(Effect.EffectType, FinishType);
	Effect.OnEffectFinishedCallback.Unbind();
	Effect.CurrentPlaybackCurve = nullptr;
}

void APostProcessSystem::SetControlAmountForEffect(const FPostProcessEffect& Effect, float ControlAmount)
{
	FWeightedBlendable& Blendable = PostProcessComp->Settings.WeightedBlendables.Array[Effect.BlendableIndex];
	UMaterialInstanceDynamic* MaterialInstance = (UMaterialInstanceDynamic*)Blendable.Object;
	MaterialInstance->SetScalarParameterValue(FName("ControlAmount"), ControlAmount);
	Blendable.Weight = (ControlAmount > 0.0F) ? 1.0F : 0.0F;
}

void APostProcessSystem::GetControlAmountForEffect(const FPostProcessEffect& Effect, float& ControlAmount)
{
	FWeightedBlendable& Blendable = PostProcessComp->Settings.WeightedBlendables.Array[Effect.BlendableIndex];
	UMaterialInstanceDynamic* MaterialInstance = (UMaterialInstanceDynamic*)Blendable.Object;
	MaterialInstance->GetScalarParameterValue(FName("ControlAmount"), ControlAmount);
}

void APostProcessSystem::InstantiatePostProcessEffects()
{
	// Initialize all effect declarations
	for (const FPostProcessEffectDef& EffectDef : PostProcessEffectDefs)
	{
		if (EffectDef.Material)
		{
			// Create an effect struct for this material def
			UMaterialInstanceDynamic* MaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), EffectDef.Material);
			FWeightedBlendable Blendable(0.0F, MaterialInstance);

			FPostProcessEffect Effect;
			Effect.EffectType = EffectDef.EffectType;

			// Add the instanced dynamic material to the post process settings
			Effect.BlendableIndex = PostProcessComp->Settings.WeightedBlendables.Array.Add(Blendable);

			PostProcessEffects.Add(Effect);
		}
	}
}

void APostProcessSystem::InstantiatePostProcessSettings()
{
#define InitializePostProcessSetting(ValueType, Type)                     \
	do {														          \
		FPostProcess##ValueType##Setting Setting;                         \
		Setting.SettingType = EPostProcess##ValueType##SettingType::Type; \
		Setting.Target = &PostProcessComp->Settings.Type;                 \
		PostProcessComp->Settings.bOverride_##Type = true;                \
		PostProcess##ValueType##Settings.Add(Setting);                    \
	} while (0);

	InitializePostProcessSetting(Scalar, BloomIntensity);
	InitializePostProcessSetting(Scalar, AutoExposureBias);
	InitializePostProcessSetting(Scalar, SceneFringeIntensity);
	InitializePostProcessSetting(Scalar, LensFlareIntensity);
	InitializePostProcessSetting(Scalar, VignetteIntensity);
	InitializePostProcessSetting(Scalar, GrainIntensity);
	InitializePostProcessSetting(Scalar, FilmSlope);
	InitializePostProcessSetting(Scalar, FilmToe);
	InitializePostProcessSetting(Scalar, FilmShoulder);
	InitializePostProcessSetting(Scalar, FilmBlackClip);
	InitializePostProcessSetting(Scalar, FilmWhiteClip);

	InitializePostProcessSetting(Vector, ColorSaturation);
	InitializePostProcessSetting(Vector, ColorContrast);
	InitializePostProcessSetting(Vector, ColorGamma);
	InitializePostProcessSetting(Vector, ColorGain);
	InitializePostProcessSetting(Vector, ColorOffset);

#undef InitializePostProcessSetting
}

PostProcessSettingTemplateFunc
void APostProcessSystem::TemplatePlayPostProcessSetting(
	_St                                      Type,
	const _Dt&                               OnFinishedCallback,
	_Ct*                                     PlaybackCurve,
	EPlaybackInterruptType                   HandlerType,
	float                                    PlaybackRate,
	bool                                     bAffectedByGlobalTimeDilation,
	TArray<PostProcessSettingTemplateBase>&  AllSettings,
	TArray<PostProcessSettingTemplateBase*>& ActiveSettings
)
{
	PostProcessSettingTemplateBase* Setting = AllSettings.FindByPredicate([Type](const PostProcessSettingTemplateBase& S) { return S.SettingType == Type; });

	if (Setting && PlaybackCurve)
	{
		switch (HandlerType)
		{
		case EPlaybackInterruptType::DropThis:
			if (ActiveSettings.Contains(Setting))
			{
				return;
			}
			break;

		case EPlaybackInterruptType::Override:
		default:
			if (ActiveSettings.Contains(Setting))
			{
				FinishPostProcessSettingPlayback(*Setting, EPlaybackFinishType::Interrupted);
				ActiveSettings.Remove(Setting);
			}
		}

		float Min, Max;
		PlaybackCurve->GetTimeRange(Min, Max);

		Setting->OnSettingFinishedCallback = OnFinishedCallback;
		Setting->CurrentPlaybackCurve = PlaybackCurve;
		Setting->CurrentTimeline.SetTimelineLength(Max);
		Setting->CurrentTimeline.SetPlayRate(PlaybackRate);
		Setting->CurrentTimeline.PlayFromStart();
		Setting->bAffectedByGlobalTimeDilation = bAffectedByGlobalTimeDilation;
		ActiveSettings.Add(Setting);
	}
}

PostProcessSettingTemplateFunc
void APostProcessSystem::TemplateSetPostProcessSettingValue(
	_St                                      Type,
	_Vt                                      Value,
	EPlaybackInterruptType                   HandlerType,
	TArray<PostProcessSettingTemplateBase>&  AllSettings,
	TArray<PostProcessSettingTemplateBase*>& ActiveSettings
)
{
	PostProcessSettingTemplateBase* Setting = AllSettings.FindByPredicate([Type](const PostProcessSettingTemplateBase& S) { return S.SettingType == Type; });

	if (Setting)
	{
		_Vt CurrentSettingValue = *Setting->Target;

		switch (HandlerType)
		{
		case EPlaybackInterruptType::DropThis:
			if (ActiveSettings.Contains(Setting))
			{
				return;
			}
			break;

		case EPlaybackInterruptType::Override:
			if (ActiveSettings.Contains(Setting))
			{
				FinishPostProcessSettingPlayback(*Setting, EPlaybackFinishType::Interrupted);
			}
			break;
		}

		*Setting->Target = Value;
	}
}

PostProcessSettingTemplateFunc
void APostProcessSystem::UpdatePostProcessSettings(TArray<PostProcessSettingTemplateBase*>& Settings, float DeltaTime)
{
	float GlobalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());

	for (int32 Index = Settings.Num() - 1; Index >= 0; --Index)
	{
		PostProcessSettingTemplateBase& Setting = *Settings[Index];

		float TimelineDeltaTime = DeltaTime * (Setting.bAffectedByGlobalTimeDilation ? GlobalTimeDilation : 1.0F);
		Setting.CurrentTimeline.TickTimeline(TimelineDeltaTime);

		if (!Setting.CurrentPlaybackCurve.IsValid())
		{
			FinishPostProcessSettingPlayback(Setting, EPlaybackFinishType::FailedToPlay);
			Settings.RemoveAtSwap(Index, 1, false);
		}

		else
		{
			float TimelineValue = Setting.CurrentTimeline.GetPlaybackPosition();
			float TimelineLength = Setting.CurrentTimeline.GetTimelineLength();
			*Setting.Target = Setting.GetCurveValue();

			// Check if the setting playback is finished
			if (TimelineValue >= TimelineLength)
			{
				FinishPostProcessSettingPlayback(Setting, EPlaybackFinishType::Finished);
				Settings.RemoveAtSwap(Index, 1, false);
			}
		}
	}
}

PostProcessSettingTemplateFunc
void APostProcessSystem::FinishPostProcessSettingPlayback(PostProcessSettingTemplateBase& Setting, EPlaybackFinishType FinishType)
{
	Setting.CurrentTimeline.Stop();
	Setting.OnSettingFinishedCallback.ExecuteIfBound(Setting.SettingType, FinishType);
	Setting.OnSettingFinishedCallback.Unbind();
	Setting.CurrentPlaybackCurve = nullptr;
}