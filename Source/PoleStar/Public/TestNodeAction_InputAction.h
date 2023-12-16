#pragma once

#include "CoreMinimal.h"
#include "TestNodeAction.h"
#include "TestNodeAction_InputAction.generated.h"

class UInputAction;

UCLASS()
class POLESTAR_API UTestNodeAction_InputAction : public UTestNodeAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functional Testing")
	FVector InputValue = FVector(1, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functional Testing")
	TObjectPtr<const UInputAction> InputAction;	

	virtual ETestNodeActionResult OnTestNodeTick(const FVector& Node, TObjectPtr<APawn> Pawn, float DeltaTime) override;
};
