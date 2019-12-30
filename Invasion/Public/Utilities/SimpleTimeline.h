// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FSimpleTimeline
{
	float Position;
	float Length;
	float PlayRate;
	uint8 bIsPlaying : 1;

	FSimpleTimeline();

	void SetTimelineLength(float L);
	void SetPlayRate(float Rate);
	void TickTimeline(float DeltaTime);
	void PlayFromStart();
	void Stop();
	float GetPlaybackPosition();	
	float GetTimelineLength();
};