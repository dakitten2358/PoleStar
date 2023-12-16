// Fill out your copyright notice in the Description page of Project Settings.


#include "TestNodeAction_MoveToLocation.h"
#include "PoleStarLogChannels.h"

ETestNodeActionResult UTestNodeAction_MoveToLocation::OnTestNodeTick(const FVector& NodeLocation, TObjectPtr<APawn> Pawn, float DeltaTime)
{
	Super::OnTestNodeTick(NodeLocation, Pawn, DeltaTime);
	
	FVector ActorLocation = Pawn->GetActorLocation();

	if (FVector::Dist2D(ActorLocation, NodeLocation) < 20.0f)
	{
		UE_LOG(LogPoleStar, Log, TEXT("Reached destination."));
		return ETestNodeActionResult::Success;
	}

	UE_LOG(LogPoleStar, Log, TEXT("Moving towards (%.2f, %.2f, %.2f)"), NodeLocation.X, NodeLocation.Y, NodeLocation.Z);
	bool bForceMovement = false;
	FVector Direction = (NodeLocation - ActorLocation).GetSafeNormal2D();
	Pawn->GetMovementComponent()->AddInputVector(Direction, bForceMovement);
	return ETestNodeActionResult::Ongoing;
}