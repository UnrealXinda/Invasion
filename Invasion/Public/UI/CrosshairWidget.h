// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnEquipWeapon(class AInvasionWeapon* Weapon);

	virtual void OnUnequipWeapon(class AInvasionWeapon* Weapon);
	
};
