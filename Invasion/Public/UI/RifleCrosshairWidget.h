// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CrosshairWidget.h"
#include "RifleCrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API URifleCrosshairWidget : public UCrosshairWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponFire(class AInvasionWeapon* Weapon, class AController* InstigatedBy);

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnEquipWeapon(class AInvasionWeapon* Weapon) override;

	virtual void OnUnequipWeapon(class AInvasionWeapon* Weapon) override;

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UCanvasPanel* CanvasPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* UpperLeftEdge;

};
