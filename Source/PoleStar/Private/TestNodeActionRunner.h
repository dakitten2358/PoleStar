// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestNodeAction.h"
#include "TestNodeActionRunner.generated.h"

USTRUCT()
struct FTestNodeActionRunner
{
	GENERATED_BODY();

public:
	void SetActions(class ADataDrivenFunctionalTest* OwningFunctionalTest, const TArray<TObjectPtr<UTestNodeAction>>& SourceActions);
	ETestNodeActionResult Tick(const FVector& NodeLocation, TObjectPtr<APawn> Pawn, float DeltaTime);
	
private:
	int CurrentActionIndex = -1;

	UPROPERTY(Transient)
	TObjectPtr<class ADataDrivenFunctionalTest> FunctionalTest;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UTestNodeAction>> Actions;
};
