// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RifleCrosshairWidget.h"
#include "Weapons/InvasionWeapon.h"
#include "Weapons/InvasionRifle.h"
#include "GameFramework/Controller.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"


void URifleCrosshairWidget::OnWeaponFire_Implementation(class AInvasionWeapon* Weapon, class AController* InstigatedBy)
{

}

void URifleCrosshairWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URifleCrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URifleCrosshairWidget::OnEquipWeapon(AInvasionWeapon* Weapon)
{
	Super::OnEquipWeapon(Weapon);

	if (Weapon)
	{
		Weapon->OnWeaponFire.AddDynamic(this, &URifleCrosshairWidget::OnWeaponFire);
	}
}

void URifleCrosshairWidget::OnUnequipWeapon(AInvasionWeapon* Weapon)
{
	Super::OnUnequipWeapon(Weapon);
}
