// Fill out your copyright notice in the Description page of Project Settings.

#include "Utilities/SimpleTimeline.h"

FSimpleTimeline::FSimpleTimeline() :
	Position(0.0F),
	Length(0.0F),
	PlayRate(1.0F),
	bIsPlaying(false)
{}

void FSimpleTimeline::SetTimelineLength(float L)
{
	Length = L;
}

void FSimpleTimeline::SetPlayRate(float Rate)
{
	PlayRate = Rate;
}

void FSimpleTimeline::TickTimeline(float DeltaTime)
{
	Position += DeltaTime * PlayRate * bIsPlaying;
}

void FSimpleTimeline::PlayFromStart()
{
	Position = 0.0F;
	bIsPlaying = true;
}

void FSimpleTimeline::Stop()
{
	bIsPlaying = false;
}

float FSimpleTimeline::GetPlaybackPosition()
{
	return Position;
}

float FSimpleTimeline::GetTimelineLength()
{
	return Length;
}