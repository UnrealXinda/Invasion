// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEnergyConfiguration.h"

float UPlayerEnergyConfiguration::GetEnergyRecoverFromWeapon(EWeaponType WeaponType, EPhysicalSurface PhysicalSurface) const
{
	const FWeaponEnergyRecover* Recover = WeaponRecover.FindByPredicate([WeaponType](const FWeaponEnergyRecover& WeaponEnergyRecover)
	{
		return WeaponEnergyRecover.WeaponType == WeaponType;
	});

	if (Recover)
	{
		const FWeaponEnergyRecoverEntry* Entry = Recover->Entries.FindByPredicate([PhysicalSurface](const FWeaponEnergyRecoverEntry& RecoverEntry)
		{
			return RecoverEntry.PhysicalSurface == PhysicalSurface;
		});
		
		if (Entry)
		{
			return Entry->RecoverAmount;
		}
	}

	return 0.0f;
}
