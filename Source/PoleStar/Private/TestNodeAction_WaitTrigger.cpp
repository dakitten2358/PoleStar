#include "TestNodeAction_WaitTrigger.h"
#include "PoleStarLogChannels.h"
#include "PoleStarGameplayTags.h"

UTestNodeAction_WaitTrigger::UTestNodeAction_WaitTrigger()
{
	TriggerTag = PoleStarGameplayTags::Test_Trigger_Default;
}

ETestNodeActionResult UTestNodeAction_WaitTrigger::OnTestNodeStart(const IPoleStarTest* Test, const FVector& NodeLocation, TObjectPtr<APawn> Pawn)
{
	UE_LOG(LogPoleStar, Verbose, TEXT("UTestNodeAction_WaitTrigger::OnTestNodeStart"));
	if (!TriggerTag.IsValid())
		return ETestNodeActionResult::Failed;

	if (Test->IsTriggered(TriggerTag))
		return ETestNodeActionResult::Success;

	return ETestNodeActionResult::Ongoing;
}

ETestNodeActionResult UTestNodeAction_WaitTrigger::OnTestNodeTick(const IPoleStarTest* Test, const FVector& Node, TObjectPtr<APawn> Pawn, float DeltaTime)
{
	UE_LOG(LogPoleStar, VeryVerbose, TEXT("UTestNodeAction_WaitTrigger::OnTestNodeTick"));
	if (Test->IsTriggered(TriggerTag))
		return ETestNodeActionResult::Success;
	
	return ETestNodeActionResult::Ongoing;
}
