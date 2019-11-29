// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Every UCLASS inheriting from IInvasionTick interface should place the following macro.
// This macro helps redirecting the UE's Tick() function to the customized InvasionTick() function.
// Automatically, it will adjust the delta time based on the time group it's on. No super call to Tick() is needed from overriden function.
// Note that any class inheriting from IInvasionTick must declare a ETimeGroup member variable to compile.
//
// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TimeGroup)
// ETimeGroup TimeGroup;
// 
// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimeGroup)
// float InvasionTimeDilation = 1.0F;
//
// This is due to the limitation of UInterface not allowing member variables as C++ interfaces.
// Also due to UHT's limitation of not allowing declaration of variables using macros.
#define REDIRECT_TICK_FUNC()                                               \
public:                                                                    \
	virtual void Tick(float DeltaTime) override;                           \
	virtual void InvasionTick_Implementation(float DeltaTime) override;

#define REDIRECT_TICK_FUNC_IMPLEMENTATION(ClassName)                                                            \
void ClassName::Tick(float DeltaTime)                                                                           \
{                                                                                                               \
	Super::Tick(DeltaTime);                                                                                     \
	ATimeDilationSystem* TimeDilationSystem = UInvasionGameplayStatics::GetTimeDilationSystem(GetWorld());      \
	float GroupTimeDilation = (TimeDilationSystem) ? TimeDilationSystem->GetTimeDilation(TimeGroup) : 1.0F;     \
	CustomTimeDilation = InvasionTimeDilation * GroupTimeDilation;                                              \
	Execute_InvasionTick(this, DeltaTime);                                                                      \
}                                                                                                               \