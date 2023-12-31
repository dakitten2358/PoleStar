// Fill out your copyright notice in the Description page of Project Settings.


#include "TestNodeAction_MoveToLocation.h"
#include "PoleStarLogChannels.h"
#include "GameFramework/CharacterMovementComponent.h"

const float UTestNodeAction_MoveToLocation::DefaultArrivalRadius = 20.0f;

ETestNodeActionResult UTestNodeAction_MoveToLocation::OnTestNodeStart(const IPoleStarTest* Test, const FVector& NodeLocation, TObjectPtr<APawn> Pawn)
{
	Super::OnTestNodeStart(Test, NodeLocation, Pawn);
	UE_LOG(LogPoleStar, Verbose, TEXT("Starting MoveToLocation action"));

	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
	if (bOrientTowardsMovement && CharacterMovementComponent)
	{
		bPreviousOrientRotationToMovement = CharacterMovementComponent->bOrientRotationToMovement;
		CharacterMovementComponent->bOrientRotationToMovement = true;
	}

	return ETestNodeActionResult::Ongoing;
}

ETestNodeActionResult UTestNodeAction_MoveToLocation::OnTestNodeTick(const IPoleStarTest* Test, const FVector& NodeLocation, TObjectPtr<APawn> Pawn, float DeltaTime)
{
	Super::OnTestNodeTick(Test, NodeLocation, Pawn, DeltaTime);
	
	FVector ActorLocation = Pawn->GetActorLocation();

	UE_LOG(LogPoleStar, VeryVerbose, TEXT("Moving towards (%.2f, %.2f, %.2f)"), NodeLocation.X, NodeLocation.Y, NodeLocation.Z);
	bool bForceMovement = false;
	FVector Direction = (NodeLocation - ActorLocation).GetSafeNormal2D();
	Pawn->GetMovementComponent()->AddInputVector(Direction, bForceMovement);

	// check after
	if (FVector::Dist2D(ActorLocation, NodeLocation) < ArrivalRadius)
	{
		UE_LOG(LogPoleStar, Verbose, TEXT("Reached destination."));
		return ETestNodeActionResult::Success;
	}
	
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