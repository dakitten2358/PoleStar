#include "TestNodeAction_InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "InputModifiers.h"
#include "InputTriggers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"

ETestNodeActionResult UTestNodeAction_InputAction::OnTestNodeTick(const FVector& Node, TObjectPtr<APawn> Pawn, float DeltaTime)
{
	if (APlayerController* PC = Pawn->GetController<APlayerController>())
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* InputSystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (IsValid(InputAction) && InputSystem)
			{
				TArray<UInputModifier*> Modifiers;
				TArray<UInputTrigger*> Triggers;
				InputSystem->InjectInputVectorForAction(InputAction, InputValue, Modifiers, Triggers);
				return ETestNodeActionResult::Success;
			}
		}
	}
	return ETestNodeActionResult::Failed;
}
