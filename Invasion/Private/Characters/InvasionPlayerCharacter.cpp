// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InvasionPlayerCharacter.h"

AInvasionPlayerCharacter::AInvasionPlayerCharacter()
{
	TimeGroup = ETimeGroup::Player;

	MaxWalkSpeed = 200;
	MaxRunSpeed = 600;
	MaxSprintSpeed = 800;
}
