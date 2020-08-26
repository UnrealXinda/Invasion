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

	if (OwnerWeapon)
	{
		float SpreadRate = OwnerWeapon->CrosshairZoomOutPerSecond;
		CrosshairZoom = FMath::Clamp(CrosshairZoom - InDeltaTime * SpreadRate, 0.0f, 1.0f);
	}
}

void URifleCrosshairWidget::OnEquipWeapon(AInvasionWeapon* Weapon)
{
	Super::OnEquipWeapon(Weapon);

	CrosshairZoom = 0;
	Weapon->OnWeaponFire.AddDynamic(this, &URifleCrosshairWidget::OnWeaponFire);
}

void URifleCrosshairWidget::OnUnequipWeapon(AInvasionWeapon* Weapon)
{
	Super::OnUnequipWeapon(Weapon);

	Weapon->OnWeaponFire.RemoveDynamic(this, &URifleCrosshairWidget::OnWeaponFire);
}

void URifleCrosshairWidget::OnWeaponFire(class AInvasionWeapon* Weapon, class AController* InstigatedBy)
{
	check(Weapon && Weapon == OwnerWeapon);

	float SpreadRate = OwnerWeapon->CrosshairZoomInPerFire;
	CrosshairZoom = FMath::Clamp(CrosshairZoom + SpreadRate, 0.0f, 1.0f);
}
