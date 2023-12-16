// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestNode.h"
#include "TestNodeActionRunner.h"
#include "TestNodeRunner.generated.h"

class Pawn;

USTRUCT()
struct FTestNodeRunner
{
	GENERATED_BODY();

public:
	ETestNodeActionResult Tick(TObjectPtr<AActor> Owner, const TArray<FTestNode>& Nodes, TObjectPtr<APawn> Pawn, float DeltaSeconds);
	
private:
	int CurrentIndex = -1;
	FTestNodeActionRunner ActionRunner;
};
