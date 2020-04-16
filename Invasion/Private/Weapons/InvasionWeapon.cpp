// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/InvasionWeapon.h"
#include "Weapons/WeaponDamageInfo.h"
#include "Characters/InvasionPlayerCharacter.h"
#include "Player/InvasionPlayerController.h"

// Sets default values
AInvasionWeapon::AInvasionWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AInvasionWeapon::EquipWeapon(AActor* OwnerActor, USceneComponent* ComponentToAttach, FName SocketToAttach /*= NAME_None*/)
{
	check(ComponentToAttach);
	check(OwnerActor);
	check(ComponentToAttach->GetOwner() == OwnerActor);

	SetOwner(OwnerActor);
	AttachToComponent(ComponentToAttach, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketToAttach);

	if (!CrosshairWidget && CrosshairWidgetClass)
	{
		if (AInvasionPlayerCharacter* Player = Cast<AInvasionPlayerCharacter>(OwnerActor))
		{
			if (AInvasionPlayerController* Controller = Cast<AInvasionPlayerController>(Player->GetController()))
			{
				CrosshairWidget = Cast<UCrosshairWidget>(CreateWidget(Controller, CrosshairWidgetClass, FName("Crosshair")));

				if (CrosshairWidget)
				{
					CrosshairWidget->OnEquipWeapon(this);
				}
			}
		}
	}
}

void AInvasionWeapon::UnequipWeapon()
{
	if (GetOwner())
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (CrosshairWidget)
	{
		CrosshairWidget->OnUnequipWeapon(this);
	}
}

void AInvasionWeapon::Fire()
{

}

void AInvasionWeapon::StartFire()
{

}

void AInvasionWeapon::StopFire()
{

}

void AInvasionWeapon::SetWeaponVisibility(bool bVisible)
{

}

bool AInvasionWeapon::IsWeaponVisible() const
{
	return true;
}
