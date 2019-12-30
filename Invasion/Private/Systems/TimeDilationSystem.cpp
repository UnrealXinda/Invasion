// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/TimeDilationSystem.h"

// Sets default values
ATimeDilationSystem::ATimeDilationSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}

// Called when the game starts or when spawned
void ATimeDilationSystem::BeginPlay()
{
	Super::BeginPlay();
	
	for (uint8 Index = 0; Index < static_cast<uint8>(ETimeGroup::Max); ++Index)
	{
		FTimeDilationState State;
		State.TimeGroup = static_cast<ETimeGroup>(Index);
		State.TimeDilation = 1.0F;

		TimeDilationStates.Add(State);
	}
}

// Called every frame
void ATimeDilationSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (uint8 Index = 0; Index < static_cast<uint8>(ETimeGroup::Max); ++Index)
	{
		FTimeDilationState& State = TimeDilationStates[Index];
		FSimpleTimeline& Timeline = State.CurrentTimeline;

		if (Timeline.bIsPlaying)
		{
			Timeline.TickTimeline(DeltaTime);

			if (!State.CurrentPlaybackCurve.IsValid())
			{
				FinishCurrentPlayback(State, EPlaybackFinishType::FailedToPlay);
			}

			else
			{
				float TimelineValue = Timeline.GetPlaybackPosition();
				float TimelineLength = Timeline.GetTimelineLength();

				State.TimeDilation = State.CurrentPlaybackCurve->GetFloatValue(TimelineValue);

				// Check if the setting playback is finished
				if (TimelineValue >= TimelineLength)
				{
					FinishCurrentPlayback(State, EPlaybackFinishType::Finished);
				}
			}
		}
	}

	// Always make sure this system is immune to global time dilation!
	float GlobalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
	CustomTimeDilation = 1.0F / GlobalTimeDilation;
}

void ATimeDilationSystem::PlayTimeDilationCurveOnGroup(
	UCurveFloat*                                   PlaybackCurve,
	ETimeGroup                                     AffectedTimeGroup,
	const FOnTimeDilationPlaybackFinishedDelegate& OnFinishedCallback,
	EPlaybackInterruptType                         InterruptType /* = EPlaybackInterruptType::DropThis */,
	float                                          PlaybackRate /* = 1.0F */,
	bool                                           bAffectLowerGroup /* = true */
)
{
	uint8 GroupIndex = static_cast<uint8>(AffectedTimeGroup);
	uint8 StartIndex = bAffectLowerGroup ? 0 : GroupIndex;
	uint8 EndIndex = GroupIndex;

	for (uint8 Index = StartIndex; Index <= EndIndex; ++Index)
	{
		FTimeDilationState& State = TimeDilationStates[Index];

		if (!PlaybackCurve)
		{
			OnFinishedCallback.ExecuteIfBound(State.TimeGroup, EPlaybackFinishType::FailedToPlay);
		}

		else
		{
			if (State.CurrentTimeline.bIsPlaying)
			{
				switch (InterruptType)
				{
				case EPlaybackInterruptType::DropThis:
				{
					return;
				}
				break;
				case EPlaybackInterruptType::Override:
				{
					FinishCurrentPlayback(State, EPlaybackFinishType::Interrupted);
				}
				break;
				}
			}

			float Min, Max;
			PlaybackCurve->GetTimeRange(Min, Max);

			State.OnPlaybackFinishedCallback = OnFinishedCallback;
			State.CurrentPlaybackCurve = PlaybackCurve;
			State.CurrentTimeline.SetPlayRate(PlaybackRate);
			State.CurrentTimeline.SetTimelineLength(Max);
			State.CurrentTimeline.PlayFromStart();
		}
	}
}

void ATimeDilationSystem::SetTimeDilationValueOnGroup(
	float                  TimeDilation,
	ETimeGroup             AffectedTimeGroup,
	EPlaybackInterruptType InterruptType /* = EPlaybackInterruptType::Override */,
	bool                   bAffectLowerGroup /* = true */
)
{
	uint8 GroupIndex = static_cast<uint8>(AffectedTimeGroup);
	uint8 StartIndex = bAffectLowerGroup ? 0 : GroupIndex;
	uint8 EndIndex = GroupIndex;

	for (uint8 Index = StartIndex; Index <= EndIndex; ++Index)
	{
		FTimeDilationState& State = TimeDilationStates[Index];

		if (State.CurrentTimeline.bIsPlaying)
		{
			switch (InterruptType)
			{
			case EPlaybackInterruptType::DropThis:
			{
				return;
			}
			break;
			case EPlaybackInterruptType::Override:
			{
				FinishCurrentPlayback(State, EPlaybackFinishType::Interrupted);
			}
			break;
			}
		}

		State.TimeDilation = TimeDilation;
	}
}

float ATimeDilationSystem::GetTimeDilation(ETimeGroup TimeGroup) const
{
	uint8 Index = static_cast<uint8>(TimeGroup);
	check(Index >= 0 && Index < static_cast<uint8>(ETimeGroup::Max));

	return TimeDilationStates[Index].TimeDilation;
}

void ATimeDilationSystem::FinishCurrentPlayback(FTimeDilationState& State, EPlaybackFinishType FinishType)
{
	State.CurrentTimeline.Stop();
	State.OnPlaybackFinishedCallback.ExecuteIfBound(State.TimeGroup, FinishType);
	State.OnPlaybackFinishedCallback.Unbind();
	State.CurrentPlaybackCurve = nullptr;
}
