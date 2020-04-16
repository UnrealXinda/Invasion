// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/InvasionAIController.h"
#include "InvasionDebug.h"
#include "NetworkingDistanceConstants.h"
#include "Characters/InvasionCharacter.h"
#include "Characters/InvasionEnemyCharacter.h"
#include "Characters/InvasionPlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "InvasionGameplayStatics.h"

AInvasionAIController::AInvasionAIController(const FObjectInitializer& Initializer)
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
}

bool AInvasionAIController::LineOfSightTo(const AActor* Other, FVector ViewPoint, bool bAlternateChecks) const
{
	if (!Other)
	{
		return false;
	}

	if (ViewPoint.IsZero())
	{
		FRotator ViewRotation;
		GetActorEyesViewPoint(ViewPoint, ViewRotation);

		// if we still don't have a view point we simply fail
		if (ViewPoint.IsZero())
		{
			return false;
		}
	}

	FVector OutHitLoc;
	FVector TargetLocation = Other->GetTargetLocation(GetPawn());
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(const_cast<AActor*>(Other));
	IgnoredActors.Add(GetPawn());

	bool bHit = UInvasionGameplayStatics::RaycastTest(this, ViewPoint, TargetLocation, IgnoredActors, OutHitLoc);
	if (!bHit)
	{
		return true;
	}

	// if other isn't using a cylinder for collision and isn't a Pawn (which already requires an accurate cylinder for AI)
	// then don't go any further as it likely will not be tracing to the correct location
	const APawn * OtherPawn = Cast<const APawn>(Other);
	if (!OtherPawn && !Other->GetRootComponent()->IsA<UCapsuleComponent>())
	{
		return false;
	}

	const FVector OtherActorLocation = Other->GetActorLocation();
	const float DistSq = (OtherActorLocation - ViewPoint).SizeSquared();
	if (DistSq > FARSIGHTTHRESHOLDSQUARED)
	{
		return false;
	}

	if (!OtherPawn && (DistSq > NEARSIGHTTHRESHOLDSQUARED))
	{
		return false;
	}

	float OtherRadius, OtherHeight;
	Other->GetSimpleCollisionCylinder(OtherRadius, OtherHeight);

	if (!bSkipExtraLOSChecks)
	{
		// only check sides if width of other is significant compared to distance
		if (OtherRadius * OtherRadius / (OtherActorLocation - ViewPoint).SizeSquared() < 0.0001f)
		{
			return false;
		}
		//try checking sides - look at dist to four side points, and cull furthest and closest
		FVector Points[4];
		Points[0] = OtherActorLocation - FVector(OtherRadius, -1 * OtherRadius, 0);
		Points[1] = OtherActorLocation + FVector(OtherRadius, OtherRadius, 0);
		Points[2] = OtherActorLocation - FVector(OtherRadius, OtherRadius, 0);
		Points[3] = OtherActorLocation + FVector(OtherRadius, -1 * OtherRadius, 0);
		int32 IndexMin = 0;
		int32 IndexMax = 0;
		float CurrentMax = (Points[0] - ViewPoint).SizeSquared();
		float CurrentMin = CurrentMax;
		for (int32 PointIndex = 1; PointIndex < 4; PointIndex++)
		{
			const float NextSize = (Points[PointIndex] - ViewPoint).SizeSquared();
			if (NextSize > CurrentMin)
			{
				CurrentMin = NextSize;
				IndexMax = PointIndex;
			}
			else if (NextSize < CurrentMax)
			{
				CurrentMax = NextSize;
				IndexMin = PointIndex;
			}
		}

		for (int32 PointIndex = 0; PointIndex < 4; PointIndex++)
		{
			if ((PointIndex != IndexMin) && (PointIndex != IndexMax))
			{
				bHit = UInvasionGameplayStatics::RaycastTest(this, ViewPoint, Points[PointIndex], IgnoredActors, OutHitLoc);
				if (!bHit)
				{
					return true;
				}
			}
		}
	}
	return false;
}

ETeamAttitude::Type AInvasionAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (Cast<const AInvasionEnemyCharacter>(&Other))
	{
		return ETeamAttitude::Friendly;
	}

	if (Cast<const AInvasionPlayerCharacter>(&Other))
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Neutral;
}
