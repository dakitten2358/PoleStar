#pragma once

#include "CoreMinimal.h"
#include "TestNodeAction.h"
#include "TestNodeAction_WaitTrigger.generated.h"

UCLASS()
class UTestNodeAction_WaitTrigger : public UTestNodeAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Functional Testing")
	FGameplayTag TriggerTag;

	virtual ETestNodeActionResult OnTestNodeStart(const IPoleStarTest* Test, const FVector& Node, TObjectPtr<APawn> Pawn) override;
	virtual ETestNodeActionResult OnTestNodeTick(const IPoleStarTest* Test, const FVector& Node, TObjectPtr<APawn> Pawn, float DeltaTime) override;
};
