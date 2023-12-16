#include "TestNodeActionRunner.h"
#include "TestNodeAction.h"
#include "PoleStarLogChannels.h"

void FTestNodeActionRunner::SetActions(const TArray<TObjectPtr<UTestNodeAction>>& SourceActions)
{
	Actions.Append(SourceActions);
	if (Actions.IsEmpty())
	{
		UE_LOG(LogPoleStar, Verbose, TEXT("No actions found, adding default MoveToLocation"));
		Actions.Emplace(NewObject<UTestNodeAction_MoveToLocation>());
	}
	CurrentActionIndex = -1;
}

ETestNodeActionResult FTestNodeActionRunner::Tick(const FVector& NodeLocation, TObjectPtr<APawn> Pawn, float DeltaTime)
{
	if (CurrentActionIndex == Actions.Num())
		return ETestNodeActionResult::Success;
	else if (CurrentActionIndex == -1)
	{
		ETestNodeActionResult StartResult = Actions[0]->OnTestNodeStart(NodeLocation, Pawn);
		if (StartResult == ETestNodeActionResult::Failed)
		{
			UE_LOG(LogPoleStar, Warning, TEXT("Failed to start action [0]"));
			return StartResult;
		}

		CurrentActionIndex = 0;
		UE_LOG(LogPoleStar, Verbose, TEXT("Starting action [0]"));
	} 

	TObjectPtr<UTestNodeAction> Action = Actions[CurrentActionIndex];
	if (!IsValid(Action))
	{
		UE_LOG(LogPoleStar, Warning, TEXT("Action [%d] is invalid"), CurrentActionIndex);
		return ETestNodeActionResult::Failed;
	}
	
	ETestNodeActionResult TickResult = Action->OnTestNodeTick(NodeLocation, Pawn, DeltaTime);
	switch (TickResult)
	{
	case ETestNodeActionResult::Success:
		UE_LOG(LogPoleStar, Verbose, TEXT("Ending action [%d]"), CurrentActionIndex);
		Actions[CurrentActionIndex]->OnTestNodeEnd(ETestNodeEndReason::Requested, Pawn);
		++CurrentActionIndex;
		if (CurrentActionIndex < Actions.Num())
		{
			UE_LOG(LogPoleStar, Verbose, TEXT("Starting action [%d]"), CurrentActionIndex);
			Actions[CurrentActionIndex]->OnTestNodeStart(NodeLocation, Pawn);
		}
		break;
	case ETestNodeActionResult::Failed:
		UE_LOG(LogPoleStar, Warning, TEXT("Failed action [%d]"), CurrentActionIndex);
		Actions[CurrentActionIndex]->OnTestNodeEnd(ETestNodeEndReason::Requested, Pawn);
		CurrentActionIndex = -1;
		return ETestNodeActionResult::Failed;
	}

	return ETestNodeActionResult::Ongoing;
}