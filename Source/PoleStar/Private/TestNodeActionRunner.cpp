#include "TestNodeActionRunner.h"
#include "TestNodeAction.h"
#include "PoleStarLogChannels.h"

void FTestNodeActionRunner::SetActions(class ADataDrivenFunctionalTest* OwningFunctionalTest, const TArray<TObjectPtr<UTestNodeAction>>& SourceActions)
{
	FunctionalTest = OwningFunctionalTest;
	Actions.Append(SourceActions);
	if (Actions.IsEmpty())
	{
		UE_LOG(LogPoleStar, Log, TEXT("No actions found, adding default MoveToLocation %p"), OwningFunctionalTest);
		Actions.Emplace(NewObject<UTestNodeAction_MoveToLocation>());
	}
	CurrentActionIndex = -1;
}

ETestNodeActionResult FTestNodeActionRunner::Tick(const FVector& NodeLocation, TObjectPtr<APawn> Pawn, float DeltaTime)
{
	check(Actions.Num() > 0);

	if (CurrentActionIndex == Actions.Num())
		return ETestNodeActionResult::Success;
	else if (CurrentActionIndex == -1)
	{
		check(IsValid(Actions[0]));
		ETestNodeActionResult StartResult = Actions[0]->OnTestNodeStart(FunctionalTest, NodeLocation, Pawn);
		if (StartResult == ETestNodeActionResult::Failed)
		{
			UE_LOG(LogPoleStar, Error, TEXT("Failed to start action [0]"));
			return StartResult;
		}

		CurrentActionIndex = 0;
		UE_LOG(LogPoleStar, Verbose, TEXT("Starting action [0]"));
	} 

	TObjectPtr<UTestNodeAction> Action = Actions[CurrentActionIndex];
	if (!IsValid(Action))
	{
		UE_LOG(LogPoleStar, Error, TEXT("Action [%d] is invalid"), CurrentActionIndex);
		return ETestNodeActionResult::Failed;
	}
	
	ETestNodeActionResult TickResult = Action->OnTestNodeTick(FunctionalTest, NodeLocation, Pawn, DeltaTime);
	return ProgressToNextAction(TickResult, NodeLocation, Pawn);
}

ETestNodeActionResult FTestNodeActionRunner::ProgressToNextAction(ETestNodeActionResult TickResult, const FVector& NodeLocation, TObjectPtr<APawn> Pawn)
{
	while (TickResult == ETestNodeActionResult::Success)
	{
		UE_LOG(LogPoleStar, Verbose, TEXT("Ending action [%d]"), CurrentActionIndex);
		Actions[CurrentActionIndex]->OnTestNodeEnd(ETestNodeEndReason::Requested, Pawn);
		++CurrentActionIndex;
		if (CurrentActionIndex == Actions.Num())
			return ETestNodeActionResult::Success;
		UE_LOG(LogPoleStar, Verbose, TEXT("Starting action [%d]"), CurrentActionIndex);
		TickResult = Actions[CurrentActionIndex]->OnTestNodeStart(FunctionalTest, NodeLocation, Pawn);
	}

	if (TickResult == ETestNodeActionResult::Failed)
	{
		UE_LOG(LogPoleStar, Error, TEXT("Failed action [%d]"), CurrentActionIndex);
		Actions[CurrentActionIndex]->OnTestNodeEnd(ETestNodeEndReason::Requested, Pawn);
		CurrentActionIndex = -1;
	}

	return TickResult;
}