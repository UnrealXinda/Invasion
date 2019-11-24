// Fill out your copyright notice in the Description page of Project Settings.


#include "InvasionCharacter.h"

// Sets default values
AInvasionCharacter::AInvasionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInvasionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInvasionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInvasionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

