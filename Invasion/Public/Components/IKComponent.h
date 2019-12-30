// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IKComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVASION_API UIKComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IK")
	bool bEnableIKAnimation;

	// Optimization for processing IK only when no velocity on the character
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IK")
	bool bEnableVelocityOptimization;

	// Extra distance at the bottom of player capsule.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IK")
	float IKTraceDistance;

	// Socket names set up in the skeleton
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IK")
	FName LeftFootSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IK")
	FName RightFootSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IK")
	float IKAdjustFootOffset;

	// Interpolation speed
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IK")
	float IKInterpSpeed;

	// Variables to expose to animation blueprints.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "IK")
	float AnimIKLeftFootEffector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "IK")
	float AnimIKRightFootEffector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "IK")
	FRotator AnimIKLeftFootRotation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "IK")
	FRotator AnimIKRightFootRotation;

public:
	// Sets default values for this component's properties
	UIKComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void IKFootTrace(float TraceDistance, FName SocketName, float& Offset, FRotator& Rotation);

	class AInvasionCharacter* CharacterRef;
		
};
