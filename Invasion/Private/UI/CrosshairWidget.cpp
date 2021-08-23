// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CrosshairWidget.h"

#include "Weapons/InvasionWeapon.h"

void UCrosshairWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCrosshairWidget::OnEquipWeapon(AInvasionWeapon* Weapon)
{
	check(Weapon);

	OwnerWeapon = Weapon;
	AddToViewport();
}

void UCrosshairWidget::OnUnequipWeapon(AInvasionWeapon* Weapon)
{
	check(Weapon && (OwnerWeapon == nullptr || Weapon == OwnerWeapon));

	OwnerWeapon = nullptr;
	RemoveFromParent();
}
