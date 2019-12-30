// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETimeGroup : uint8
{
	World  = 0   UMETA(DisplayName = "World"),
	Enemy  = 1   UMETA(DisplayName = "Enemy"),
	Player = 2   UMETA(DisplayName = "Player"),
	UI     = 3   UMETA(DisplayName = "UI"),
	System = 4   UMETA(DisplayName = "System"),
	Max    = 5,
};

#pragma region Character States

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Walk   = 0   UMETA(DisplayName = "Walk"),
	Run    = 1   UMETA(DisplayName = "Run"),
	Sprint = 2   UMETA(DisplayName = "Sprint"),
	Max    = 3,
};

UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Left     = 0   UMETA(DisplayName = "Left"),
	Right    = 1   UMETA(DisplayName = "Right"),
	Max      = 2,
};

UENUM(BlueprintType)
enum class EAimState : uint8
{
	Idle   = 0   UMETA(DisplayName = "Idle"),
	Aiming = 1   UMETA(DisplayName = "Aimimg"),
	Max    = 2,
};

UENUM(BlueprintType)
enum class EExecuteState : uint8
{
	Idle      = 0   UMETA(DisplayName = "Idle"),
	Executing = 1   UMETA(DisplayName = "Executing"),
	Max       = 2,
};

UENUM(BlueprintType)
enum class ECoverState : uint8
{
	Idle    = 0   UMETA(DisplayName = "Idle"),
	HighIn  = 1   UMETA(DisplayName = "HighIn"),
	HighOut = 2   UMETA(DisplayName = "HighOut"),
	LowIn   = 3   UMETA(DisplayName = "LowIn"),
	LowOut  = 4   UMETA(DisplayName = "LowOut"),
	InCover = 5   UMETA(DisplayName = "InCover"),
	Max     = 6,
};

UENUM(BlueprintType)
enum class EDashState : uint8
{
	Idle    = 0   UMETA(DisplayName = "Idle"),
	Dashing = 1   UMETA(DisplayName = "Dashing"),
	Max     = 2,
};

#pragma endregion Character States

UENUM(BlueprintType)
enum class ECoverType : uint8
{
	Low  = 0  UMETA(DisplayName = "Low"),
	High = 1  UMETA(DisplayName = "High"),
	Max  = 2,
};

#pragma region Playback

UENUM(BlueprintType)
enum class EPlaybackFinishType : uint8
{
	Finished     = 0   UMETA(DisplayName = "Finished"),
	Interrupted  = 1   UMETA(DisplayName = "Interrupted"),
	FailedToPlay = 2   UMETA(DisplayName = "Failed To Play"),
	Max          = 3,
};

UENUM(BlueprintType)
enum class EPlaybackInterruptType : uint8
{
	DropThis = 0     UMETA(DisplayName = "Drop This"),
	Override = 1     UMETA(DisplayName = "Override"),
	Max      = 2,
};

#pragma endregion Playback

#pragma region Post Process System

UENUM(BlueprintType)
enum class EPostProcessEffectType : uint8
{
	RadialBlur           = 0  UMETA(DisplayName = "Radial Blur"),
	BrokenLens           = 1  UMETA(DisplayName = "Broken Lens"),
	Glitch               = 2  UMETA(DisplayName = "Glitch"),
	Scan                 = 3  UMETA(DisplayName = "Scan"),
	Max                  = 4,
};

UENUM(BlueprintType)
enum class EPostProcessScalarSettingType : uint8
{
	BloomIntensity       = 0   UMETA(DisplayName = "Bloom Intensity"),
	AutoExposureBias     = 1   UMETA(DisplayName = "Exposure Compensation"),
	SceneFringeIntensity = 2   UMETA(DisplayName = "Chromatic Aberration Intensity"),
	LensFlareIntensity   = 3   UMETA(DisplayName = "Lens Flares Intensity"),
	VignetteIntensity    = 4   UMETA(DisplayName = "Vignette Intensity"),
	GrainIntensity       = 5   UMETA(DisplayName = "Grain Intensity"),
	FilmSlope            = 6   UMETA(DisplayName = "Film Slope"),
	FilmToe              = 7   UMETA(DisplayName = "Film Toe"),
	FilmShoulder         = 8   UMETA(DisplayName = "Film Shoulder"),
	FilmBlackClip        = 9   UMETA(DisplayName = "Film Black Clip"),
	FilmWhiteClip        = 10  UMETA(DisplayName = "Film White Clip"),
	Max                  = 11,
};

UENUM(BlueprintType)
enum class EPostProcessVectorSettingType : uint8
{
	ColorSaturation      = 0   UMETA(DisplayName = "Color Saturation"),
	ColorContrast        = 1   UMETA(DisplayName = "Color Contrast"),
	ColorGamma           = 2   UMETA(DisplayName = "Color Gamma"),
	ColorGain            = 3   UMETA(DisplayName = "Color Gain"),
	ColorOffset          = 4   UMETA(DisplayName = "Color Offset"),
	Max                  = 5,
};

UENUM(BlueprintType)
enum class EPostProcessColorSettingType : uint8
{
	Placeholder          = 0   UMETA(DisplayName = "Placeholder"),
	Max                  = 1,
};

#pragma endregion Post Process System