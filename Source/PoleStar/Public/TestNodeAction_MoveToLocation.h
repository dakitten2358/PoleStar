// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestNodeAction.h"
#include "TestAcceptanceRadiusProvider.h"
#include "TestNodeAction_MoveToLocation.generated.h"

/**
 * 
 */
UCLASS()
class POLESTAR_API UTestNodeAction_MoveToLocation : public UTestNodeAction, public ITestAcceptanceRadiusProvider
{
	GENERATED_BODY()
	
public:
	static const float DefaultArrivalRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ArrivalRadius = DefaultArrivalRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOrientTowardsMovement = true;

	virtual ETestNodeActionResult OnTestNodeStart(const FVector& Node, TObjectPtr<APawn> Pawn) override;
	virtual ETestNodeActionResult OnTestNodeTick(const FVector& Node, TObjectPtr<APawn> Pawn, float DeltaTime) override;
	virtual void OnTestNodeEnd(ETestNodeEndReason Reason, TObjectPtr<APawn> Pawn) override;

	virtual float GetAcceptanceRadius() const override { return ArrivalRadius; }

private:
	bool bPreviousOrientRotationToMovement = false;
};
