// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utilities/SimpleTimeline.h"
#include "InvasionEnums.h"
#include "TimeDilationSystem.generated.h"

/** Delegate type for callbacks for finished playback of time dilation curves */
DECLARE_DYNAMIC_DELEGATE_TwoParams(
	FOnTimeDilationPlaybackFinishedDelegate,
	/* The time group affected by the playback */
	ETimeGroup, TimeGroup,
	/* The finish type of this playback */
	EPlaybackFinishType, FinishType);

// The struct used for maintaining the runtime state of each time dilation group
struct FTimeDilationState
{
	FSimpleTimeline                          CurrentTimeline;
	TWeakObjectPtr<UCurveFloat>              CurrentPlaybackCurve;
	FOnTimeDilationPlaybackFinishedDelegate  OnPlaybackFinishedCallback;
	ETimeGroup                               TimeGroup;
	float                                    TimeDilation;
};

UCLASS()
class INVASION_API ATimeDilationSystem : public AActor
{
	GENERATED_BODY()
	
public:	

	ATimeDilationSystem();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void PlayTimeDilationCurveOnGroup(
		UCurveFloat*                                   PlaybackCurve,
		ETimeGroup                                     AffectedTimeGroup,
		const FOnTimeDilationPlaybackFinishedDelegate& OnFinishedCallback,
		EPlaybackInterruptType                         InterruptType = EPlaybackInterruptType::DropThis,
		float                                          PlaybackRate = 1.0F,
		bool                                           bAffectLowerGroup = true
	);

	UFUNCTION(BlueprintCallable)
	void SetTimeDilationValueOnGroup(
		float                   TimeDilation,
		ETimeGroup              AffectedTimeGroup,
		EPlaybackInterruptType  InterruptType = EPlaybackInterruptType::Override,
		bool                    bAffectLowerGroup = true
	);

	UFUNCTION(BlueprintPure)
	float GetTimeDilation(ETimeGroup TimeGroup) const;

protected:

	virtual void BeginPlay() override;

	void FinishCurrentPlayback(FTimeDilationState& State, EPlaybackFinishType FinishType);

	TArray<FTimeDilationState> TimeDilationStates;

};
