// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvasionEnums.h"
#include "InvasionMacros.h"
#include "Interfaces/InvasionTick.h"
#include "Utilities/SimpleTimeline.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveLinearColor.h"
#include "PostProcessSystem.generated.h"

/** Delegate type for callbacks for finished playing post process effect */
DECLARE_DYNAMIC_DELEGATE_TwoParams(
	FOnPostProcessEffectPlaybackFinishedDelegate,
	/* The type of the post process effect */
	EPostProcessEffectType, EffectType,
	/* The finish type of this playback */
	EPlaybackFinishType, FinishType);

/** Delegate type for callbacks for finished playback of post process scalar setting values */
DECLARE_DYNAMIC_DELEGATE_TwoParams(
	FOnPostProcessScalarSettingPlaybackFinishedDelegate,
	/* The type of the post process setting */
	EPostProcessScalarSettingType, SettingType,
	/* The finish type of this playback */
	EPlaybackFinishType, FinishType);

/** Delegate type for callbacks for finished playback of post process vector setting values */
DECLARE_DYNAMIC_DELEGATE_TwoParams(
	FOnPostProcessVectorSettingPlaybackFinishedDelegate,
	/* The type of the post process setting */
	EPostProcessVectorSettingType, SettingType,
	/* The finish type of this playback */
	EPlaybackFinishType, FinishType);

/** Delegate type for callbacks for finished playback of post process color setting values */
DECLARE_DYNAMIC_DELEGATE_TwoParams(
	FOnPostProcessColorSettingPlaybackFinishedDelegate,
	/* The type of the post process setting */
	EPostProcessColorSettingType, SettingType,
	/* The finish type of this playback */
	EPlaybackFinishType, FinishType);


USTRUCT(BlueprintType)
struct FPostProcessEffectDef
{
	GENERATED_USTRUCT_BODY()

	/* The effect type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPostProcessEffectType EffectType;

	/* The post process material */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* Material;
};

// The struct used for maintaining the runtime state of each effect
struct FPostProcessEffect
{
	EPostProcessEffectType                         EffectType;
	int                                            BlendableIndex;
	FSimpleTimeline                                CurrentTimeline;
	TWeakObjectPtr<UCurveFloat>                    CurrentPlaybackCurve;
	FOnPostProcessEffectPlaybackFinishedDelegate   OnEffectFinishedCallback;
	uint8                                          bAffectedByGlobalTimeDilation : 1;
};

// The struct used for maintaining the runtime state of each setting
template <typename SettingType, typename ValueType, typename CurveType, typename DelegateType>
struct TPostProcessSettingBase
{
	SettingType                                    SettingType;
	ValueType*                                     Target;
	TWeakObjectPtr<CurveType>                      CurrentPlaybackCurve;
	FSimpleTimeline                                CurrentTimeline;
	DelegateType                                   OnSettingFinishedCallback;
	uint8                                          bAffectedByGlobalTimeDilation : 1;

#define GetCurveValueSafe(GetFunc, DefaultValue)   \
	((CurrentPlaybackCurve.IsValid()) ? CurrentPlaybackCurve->GetFunc(CurrentTimeline.GetPlaybackPosition()) : DefaultValue)

	template <typename DesiredType, typename InputType>
	using ReturnType = typename TEnableIf<TAreTypesEqual<DesiredType, InputType>::Value, DesiredType>::Type;

	template <typename InputType = ValueType>
	FORCEINLINE ReturnType<float, InputType> GetCurveValue()
	{ return GetCurveValueSafe(GetFloatValue, 0.0F); }

	template <typename InputType = ValueType>
	FORCEINLINE ReturnType<FVector4, InputType> GetCurveValue()
	{ return GetCurveValueSafe(GetUnadjustedLinearColorValue, FVector4()); }

	template <typename InputType = ValueType>
	FORCEINLINE ReturnType<FLinearColor, InputType> GetCurveValue()
	{ return GetCurveValueSafe(GetLinearColorValue, FLinearColor()); }

#undef GetCurveValueSafe
};

using FPostProcessScalarSetting = TPostProcessSettingBase<EPostProcessScalarSettingType, float,        UCurveFloat,       FOnPostProcessScalarSettingPlaybackFinishedDelegate>;
using FPostProcessVectorSetting = TPostProcessSettingBase<EPostProcessVectorSettingType, FVector4,     UCurveLinearColor, FOnPostProcessVectorSettingPlaybackFinishedDelegate>;
using FPostProcessColorSetting  = TPostProcessSettingBase<EPostProcessColorSettingType,  FLinearColor, UCurveLinearColor, FOnPostProcessColorSettingPlaybackFinishedDelegate>;

UCLASS()
class INVASION_API APostProcessSystem : public AActor, public IInvasionTick
{
	GENERATED_BODY()

	REDIRECT_TICK_FUNC()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeGroup)
	ETimeGroup TimeGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimeGroup)
	float InvasionTimeDilation = 1.0f;

public:

	APostProcessSystem();

	// Play an effect means playing it over a period of time. This is in contrast to directly setting
	// an effect's control amount
	UFUNCTION(BlueprintCallable)
	void PlayPostProcessEffect(
		EPostProcessEffectType                              Type,
		const FOnPostProcessEffectPlaybackFinishedDelegate& OnFinishedCallback,
		UCurveFloat*                                        PlaybackCurve,
		EPlaybackInterruptType                              HandlerType = EPlaybackInterruptType::DropThis,
		float                                               PlaybackRate = 1.0f,
		bool                                                bAffectedByGlobalTimeDilation = true
	);

	UFUNCTION(BlueprintCallable)
	void PlayPostProcessScalarSetting(
		EPostProcessScalarSettingType                              Type,
		const FOnPostProcessScalarSettingPlaybackFinishedDelegate& OnFinishedCallback,
		UCurveFloat*                                               PlaybackCurve,
		EPlaybackInterruptType                                     HandlerType = EPlaybackInterruptType::DropThis,
		float                                                      PlaybackRate = 1.0f,
		bool                                                       bAffectedByGlobalTimeDilation = true
	);

	UFUNCTION(BlueprintCallable)
	void PlayPostProcessVectorSetting(
		EPostProcessVectorSettingType                              Type,
		const FOnPostProcessVectorSettingPlaybackFinishedDelegate& OnFinishedCallback,
		UCurveLinearColor*                                         PlaybackCurve,
		EPlaybackInterruptType                                     HandlerType = EPlaybackInterruptType::DropThis,
		float                                                      PlaybackRate = 1.0f,
		bool                                                       bAffectedByGlobalTimeDilation = true
	);

	UFUNCTION(BlueprintCallable)
	void PlayPostProcessColorSetting(
		EPostProcessColorSettingType                              Type,
		const FOnPostProcessColorSettingPlaybackFinishedDelegate& OnFinishedCallback,
		UCurveLinearColor*                                        PlaybackCurve,
		EPlaybackInterruptType                                    HandlerType = EPlaybackInterruptType::DropThis,
		float                                                     PlaybackRate = 1.0f,
		bool                                                      bAffectedByGlobalTimeDilation = true
	);

	UFUNCTION(BlueprintCallable)
	void SetPostProcessEffectControlAmount(
		EPostProcessEffectType Type,
		float                  ControlAmount,
		EPlaybackInterruptType HandlerType = EPlaybackInterruptType::Override
	);

	UFUNCTION(BlueprintCallable)
	void SetPostProcessSettingScalarValue(
		EPostProcessScalarSettingType Type,
		float                         Value,
		EPlaybackInterruptType        HandlerType = EPlaybackInterruptType::Override
	);

	UFUNCTION(BlueprintCallable)
	void SetPostProcessSettingVectorValue(
		EPostProcessVectorSettingType Type,
		FVector4                      Value,
		EPlaybackInterruptType        HandlerType = EPlaybackInterruptType::Override
	);

	UFUNCTION(BlueprintCallable)
	void SetPostProcessSettingColorValue(
		EPostProcessColorSettingType Type,
		FLinearColor                 Value,
		EPlaybackInterruptType       HandlerType = EPlaybackInterruptType::Override
	);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPostProcessEffectDef>      PostProcessEffectDefs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPostProcessComponent*       PostProcessComp;

	TArray<FPostProcessEffect>         PostProcessEffects;
	TArray<FPostProcessEffect*>        ActiveEffects;

	TArray<FPostProcessScalarSetting>  PostProcessScalarSettings;
	TArray<FPostProcessVectorSetting>  PostProcessVectorSettings;
	TArray<FPostProcessColorSetting>   PostProcessColorSettings;

	TArray<FPostProcessScalarSetting*> ActiveScalarSettings;
	TArray<FPostProcessVectorSetting*> ActiveVectorSettings;
	TArray<FPostProcessColorSetting*>  ActiveColorSettings;

protected:

	virtual void BeginPlay() override;

	void InstantiatePostProcessEffects();
	void SetControlAmountForEffect(const FPostProcessEffect& Effect, float ControlAmount);
	void GetControlAmountForEffect(const FPostProcessEffect& Effect, float& ControlAmount);
	void UpdatePostProcessEffects(TArray<FPostProcessEffect*>& Effects, float DeltaTime);
	void FinishPostProcessEffectPlayback(FPostProcessEffect& Effect, EPlaybackFinishType FinishType);

	void InstantiatePostProcessSettings();

#define PostProcessSettingTemplateFunc      template <typename _St, typename _Vt, typename _Ct, typename _Dt>
#define PostProcessSettingTemplateBase      TPostProcessSettingBase<_St, _Vt, _Ct, _Dt>

	PostProcessSettingTemplateFunc
	void TemplatePlayPostProcessSetting(
		_St                                      Type,
		const _Dt&                               OnFinishedCallback,
		_Ct*                                     PlaybackCurve,
		EPlaybackInterruptType                   HandlerType,
		float                                    PlaybackRate,
		bool                                     bAffectedByGlobalTimeDilation,
		TArray<PostProcessSettingTemplateBase>&  AllSettings,
		TArray<PostProcessSettingTemplateBase*>& ActiveSettings
	);

	PostProcessSettingTemplateFunc
	void TemplateSetPostProcessSettingValue(
		_St                                      Type,
		_Vt                                      Value,
		EPlaybackInterruptType                   HandlerType, 
		TArray<PostProcessSettingTemplateBase>&  AllSettings,
		TArray<PostProcessSettingTemplateBase*>& ActiveSettings
	);

	PostProcessSettingTemplateFunc
	void UpdatePostProcessSettings(TArray<PostProcessSettingTemplateBase*>& Settings, float DeltaTime);

	PostProcessSettingTemplateFunc
	void FinishPostProcessSettingPlayback(PostProcessSettingTemplateBase& Setting, EPlaybackFinishType FinishType);
};