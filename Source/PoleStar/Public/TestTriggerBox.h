// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameplayTagContainer.h"
#include "TestTriggerBox.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ATestTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ATestTriggerBox(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
	FGameplayTag TriggerTag;

	//~Begin AActor
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//~End AActor
};
