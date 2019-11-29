// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FSimpleTimeline
{
	float Position;
	float Length;
	float PlayRate;
	uint8 bIsPlaying : 1;

	FSimpleTimeline() :
		Position(0.0F),
		Length(0.0F),
		PlayRate(1.0F),
		bIsPlaying(false)
	{}

	FORCEINLINE void SetTimelineLength(float L)
	{ 
		Length = L; 
	}

	FORCEINLINE void SetPlayRate(float Rate)
	{ 
		PlayRate = Rate; 
	}

	FORCEINLINE void TickTimeline(float DeltaTime)
	{ 
		Position += DeltaTime * PlayRate * bIsPlaying; 
	}

	FORCEINLINE void PlayFromStart()
	{ 
		Position = 0.0F; 
		bIsPlaying = true; 
	}

	FORCEINLINE void Stop()
	{ 
		bIsPlaying = false; 
	}

	FORCEINLINE float GetPlaybackPosition()
	{ 
		return Position; 
	}

	FORCEINLINE float GetTimelineLength() 
	{ 
		return Length; 
	}
};