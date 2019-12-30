// Fill out your copyright notice in the Description page of Project Settings.

#include "IKComponent.h"
#include "InvasionDebug.h"
#include "Characters/InvasionCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UIKComponent::UIKComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bEnableIKAnimation = true;
	bEnableVelocityOptimization = true;
	LeftFootSocketName = FName("Foot_l");
	RightFootSocketName = FName("Foot_r");
	IKInterpSpeed = 150.0F;
	IKTraceDistance = 50.0F;
}


// Called when the game starts
void UIKComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache the owning pawn for use in Tick
	CharacterRef = Cast<AInvasionCharacter>(GetOwner());
	check(CharacterRef);
}


// Called every frame
void UIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEnableIKAnimation && CharacterRef)
	{
		if (bEnableVelocityOptimization && CharacterRef->GetVelocity().SizeSquared() > 0.0F)
		{
			AnimIKLeftFootEffector = FMath::FInterpTo(AnimIKLeftFootEffector, 0.0F, DeltaTime, IKInterpSpeed);
			AnimIKRightFootEffector = FMath::FInterpTo(AnimIKRightFootEffector, 0.0F, DeltaTime, IKInterpSpeed);
			AnimIKLeftFootRotation = FMath::RInterpTo(AnimIKLeftFootRotation, FRotator::ZeroRotator, DeltaTime, IKInterpSpeed);
			AnimIKRightFootRotation = FMath::RInterpTo(AnimIKRightFootRotation, FRotator::ZeroRotator, DeltaTime, IKInterpSpeed);
		}
		else
		{
			float IKLeftFootOffset;
			float IKRightFootOffset;
			FRotator IKLeftFootRotation;
			FRotator IKRightFootRotation;

			// Run feet raycast to calculate offset
			IKFootTrace(IKTraceDistance, LeftFootSocketName, IKLeftFootOffset, IKLeftFootRotation);
			IKFootTrace(IKTraceDistance, RightFootSocketName, IKRightFootOffset, IKRightFootRotation);

			// Update feet effector according to hip offset
			AnimIKLeftFootEffector = FMath::FInterpTo(AnimIKLeftFootEffector, IKLeftFootOffset, DeltaTime, IKInterpSpeed);
			AnimIKRightFootEffector = FMath::FInterpTo(AnimIKRightFootEffector, -IKRightFootOffset, DeltaTime, IKInterpSpeed);
			AnimIKLeftFootRotation = FMath::RInterpTo(AnimIKLeftFootRotation, IKLeftFootRotation, DeltaTime, IKInterpSpeed);
			AnimIKRightFootRotation = FMath::RInterpTo(AnimIKRightFootRotation, IKRightFootRotation, DeltaTime, IKInterpSpeed);
		}
	}
}

void UIKComponent::IKFootTrace(float TraceDistance, FName SocketName, float& Offset, FRotator& Rotation)
{
	FVector ActorLoc = CharacterRef->GetActorLocation();
	FVector SocketLoc = CharacterRef->GetMesh()->GetSocketLocation(SocketName);
	Offset = 0.0F;
	Rotation = FRotator::ZeroRotator;

	float TraceLineLength = TraceDistance + CharacterRef->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector StartLoc = FVector(SocketLoc.X, SocketLoc.Y, ActorLoc.Z);
	FVector EndLoc = FVector(SocketLoc.X, SocketLoc.Y, ActorLoc.Z - TraceLineLength);

	FHitResult HitResult;
	TArray<AActor*> IgnoredActors = { CharacterRef };
	EDrawDebugTrace::Type DebugType = InvasionDebug::g_DebugDrawIKTrace ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true, IgnoredActors, DebugType, HitResult, true))
	{
		Offset = (HitResult.Location - HitResult.TraceEnd).Size() - TraceDistance + IKAdjustFootOffset;

		Rotation.Roll = UKismetMathLibrary::DegAtan2(HitResult.Normal.Y, HitResult.Normal.Z);
		Rotation.Pitch = -UKismetMathLibrary::DegAtan2(HitResult.Normal.X, HitResult.Normal.Z);
		Rotation.Yaw = 0.0F;
	}
}

