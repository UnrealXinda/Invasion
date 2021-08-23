// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "InvasionAIController.generated.h"

/**
 * 
 */
UCLASS()
class INVASION_API AInvasionAIController : public AAIController
{
	GENERATED_BODY()

public:

	AInvasionAIController(const FObjectInitializer& Initializer);

	virtual bool LineOfSightTo(const AActor* Other, FVector ViewPoint = FVector(ForceInit), bool bAlternateChecks = false) const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	class UAIPerceptionComponent* AIPerceptionComp;

private:

	bool LineOfSightTest(const FVector& Start, const FVector& End, const AActor* Other) const;

};
