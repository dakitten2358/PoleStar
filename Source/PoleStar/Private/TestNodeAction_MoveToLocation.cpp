// Fill out your copyright notice in the Description page of Project Settings.


#include "TestNodeAction_MoveToLocation.h"
#include "PoleStarLogChannels.h"
#include "GameFramework/CharacterMovementComponent.h"

const float UTestNodeAction_MoveToLocation::DefaultArrivalRadius = 20.0f;

ETestNodeActionResult UTestNodeAction_MoveToLocation::OnTestNodeStart(const FVector& NodeLocation, TObjectPtr<APawn> Pawn)
{
	Super::OnTestNodeStart(NodeLocation, Pawn);
	UE_LOG(LogPoleStar, Verbose, TEXT("Starting MoveToLocation action"));

	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
	if (bOrientTowardsMovement && CharacterMovementComponent)
	{
		bPreviousOrientRotationToMovement = CharacterMovementComponent->bOrientRotationToMovement;
		CharacterMovementComponent->bOrientRotationToMovement = true;
	}

	return ETestNodeActionResult::Ongoing;
}

ETestNodeActionResult UTestNodeAction_MoveToLocation::OnTestNodeTick(const FVector& NodeLocation, TObjectPtr<APawn> Pawn, float DeltaTime)
{
	Super::OnTestNodeTick(NodeLocation, Pawn, DeltaTime);
	
	FVector ActorLocation = Pawn->GetActorLocation();

	if (FVector::Dist2D(ActorLocation, NodeLocation) < ArrivalRadius)
	{
		UE_LOG(LogPoleStar, Verbose, TEXT("Reached destination."));
		return ETestNodeActionResult::Success;
	}

	UE_LOG(LogPoleStar, Verbose, TEXT("Moving towards (%.2f, %.2f, %.2f)"), NodeLocation.X, NodeLocation.Y, NodeLocation.Z);
	bool bForceMovement = false;
	FVector Direction = (NodeLocation - ActorLocation).GetSafeNormal2D();
	Pawn->GetMovementComponent()->AddInputVector(Direction, bForceMovement);
	return ETestNodeActionResult::Ongoing;
}

void UTestNodeAction_MoveToLocation::OnTestNodeEnd(ETestNodeEndReason Reason, TObjectPtr<APawn> Pawn)
{
	Super::OnTestNodeEnd(Reason, Pawn);
	UE_LOG(LogPoleStar, Verbose, TEXT("Ending MoveToLocation action"));

	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
	if (bOrientTowardsMovement && CharacterMovementComponent)
		CharacterMovementComponent->bOrientRotationToMovement = bPreviousOrientRotationToMovement;
}