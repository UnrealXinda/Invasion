// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RifleCrosshairWidget.h"
#include "Weapons/InvasionWeapon.h"
#include "Weapons/InvasionRifle.h"
#include "GameFramework/Controller.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"

void URifleCrosshairWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URifleCrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Rifle)
	{
		float SpreadRate = Rifle->CrosshairZoomOutPerSecond;
		CrosshairZoom = FMath::Clamp(CrosshairZoom - InDeltaTime * SpreadRate, 0.0f, 1.0f);
	}
}

void URifleCrosshairWidget::OnEquipWeapon(AInvasionWeapon* Weapon)
{
	Super::OnEquipWeapon(Weapon);

	CrosshairZoom = 0;

	if (Weapon)
	{
		Rifle = Weapon;
		Weapon->OnWeaponFire.AddDynamic(this, &URifleCrosshairWidget::OnWeaponFire);
	}
}

void URifleCrosshairWidget::OnUnequipWeapon(AInvasionWeapon* Weapon)
{
	Super::OnUnequipWeapon(Weapon);

	if (Weapon)
	{
		check(Weapon == Rifle);

		Rifle = nullptr;
		Weapon->OnWeaponFire.RemoveDynamic(this, &URifleCrosshairWidget::OnWeaponFire);
	}
}

void URifleCrosshairWidget::OnWeaponFire(class AInvasionWeapon* Weapon, class AController* InstigatedBy)
{
	check(Weapon == Rifle);

	float SpreadRate = Rifle->CrosshairZoomInPerFire;
	CrosshairZoom = FMath::Clamp(CrosshairZoom + SpreadRate, 0.0f, 1.0f);
}
