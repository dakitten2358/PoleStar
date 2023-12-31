#include "TestNodeRunner.h"
#include "DataDrivenFunctionalTest.h"

ETestNodeActionResult FTestNodeRunner::Tick(TObjectPtr<ADataDrivenFunctionalTest> Owner, const TArray<FTestNode>& Nodes, TObjectPtr<APawn> Pawn, float DeltaSeconds)
{
	if (CurrentIndex == Nodes.Num())
		return ETestNodeActionResult::Success;

	else if (CurrentIndex == -1)
	{
		CurrentIndex = 0;
		ActionRunner = FTestNodeActionRunner();
		ActionRunner.SetActions(Owner, Nodes[CurrentIndex].Actions);
	}
	
	if (CurrentIndex < 0 || CurrentIndex >= Nodes.Num())
		return ETestNodeActionResult::Failed;

	ETestNodeActionResult TestNodeResult = ActionRunner.Tick(Nodes[CurrentIndex].Location + Owner->GetActorLocation(), Pawn, DeltaSeconds);
	switch (TestNodeResult)
	{
	case ETestNodeActionResult::Success:
		++CurrentIndex;
		if (CurrentIndex < Nodes.Num())
		{
			ActionRunner = FTestNodeActionRunner();
			ActionRunner.SetActions(Owner, Nodes[CurrentIndex].Actions);
		}
		break;
	case ETestNodeActionResult::Failed:
		CurrentIndex = -1;
		ActionRunner = FTestNodeActionRunner();
		return ETestNodeActionResult::Failed;
	}	

	return ETestNodeActionResult::Ongoing;
}
