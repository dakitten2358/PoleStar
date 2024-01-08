#include "TestNodeAction_InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "InputModifiers.h"
#include "InputTriggers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"

ETestNodeActionResult UTestNodeAction_InputAction::OnTestNodeStart(const IPoleStarTest* Test, const FVector& NodeLocation, TObjectPtr<APawn> Pawn)
{
	// reset back to defaults
	CurrentState = EInputActionState::PreAction;
	CurrentPreActionDuration = 0.0f;
	CurrentHoldDuration = 0.0f;
	CurrentPostActionDuration = 0.0f;

	// make sure action is OK
	if (!IsValid(InputAction))
		return ETestNodeActionResult::Failed;

	// get a ref to input system
	if (APlayerController* PC = Pawn->GetController<APlayerController>())
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			InputSystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		}
	}
	if (!IsValid(InputSystem))
		return ETestNodeActionResult::Failed;

	return OnTestNodeTick(Test, NodeLocation, Pawn, 0.0f);
}

ETestNodeActionResult UTestNodeAction_InputAction::OnTestNodeTick(const IPoleStarTest* Test, const FVector& Node, TObjectPtr<APawn> Pawn, float DeltaTime)
{
	if (CurrentState == EInputActionState::PreAction)
	{
		if (CurrentPreActionDuration < PreActionDelay)
		{
			CurrentPreActionDuration += DeltaTime;
			return ETestNodeActionResult::Ongoing;
		}

		CurrentState = EInputActionState::Press;
		if (!FMath::IsNearlyZero(HoldDuration))
			CurrentState = EInputActionState::Hold;
	}

	if (CurrentState == EInputActionState::Press)
	{
		InjectInputAction();
		CurrentState = EInputActionState::PostAction;
	}

	if (CurrentState == EInputActionState::Hold)
	{
		InjectInputAction();
		if (CurrentHoldDuration < HoldDuration)
		{
			CurrentHoldDuration += DeltaTime;
			return ETestNodeActionResult::Ongoing;
		}
		CurrentState = EInputActionState::PostAction;
	}
	
	if (CurrentState == EInputActionState::PostAction)
	{
		if (CurrentPostActionDuration < PostActionDelay)
		{
			CurrentPostActionDuration += DeltaTime;
			return ETestNodeActionResult::Ongoing;
		}
		return ETestNodeActionResult::Success;
	}

	return ETestNodeActionResult::Ongoing;
}

void UTestNodeAction_InputAction::InjectInputAction()
{
	TArray<UInputModifier*> Modifiers;
	TArray<UInputTrigger*> Triggers;
	InputSystem->InjectInputVectorForAction(InputAction, InputValue, Modifiers, Triggers);
}
