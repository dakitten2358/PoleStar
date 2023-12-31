#pragma once

#include "CoreMinimal.h"
#include "TestNodeAction.h"
#include "TestNodeAction_InputAction.generated.h"

class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class POLESTAR_API UTestNodeAction_InputAction : public UTestNodeAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functional Testing")
	FVector InputValue = FVector(1, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functional Testing")
	TObjectPtr<const UInputAction> InputAction;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functional Testing", AdvancedDisplay)
	float HoldDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functional Testing", AdvancedDisplay)
	float PreActionDelay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functional Testing", AdvancedDisplay)
	float PostActionDelay = 0.0f;

	virtual ETestNodeActionResult OnTestNodeStart(const IPoleStarTest* Test, const FVector& Node, TObjectPtr<APawn> Pawn) override;
	virtual ETestNodeActionResult OnTestNodeTick(const IPoleStarTest* Test, const FVector& Node, TObjectPtr<APawn> Pawn, float DeltaTime) override;

private:
	enum class EInputActionState
	{
		PreAction,
		Press,
		Hold,
		PostAction,
	};

	EInputActionState CurrentState = EInputActionState::PreAction;
	float CurrentPreActionDuration = 0.0f;
	float CurrentHoldDuration = 0.0f;
	float CurrentPostActionDuration = 0.0f;

	UPROPERTY(Transient)
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSystem;

	void InjectInputAction();


};
