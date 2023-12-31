#include "PoleStarBlueprintFunctionLibrary.h"
#include "DataDrivenFunctionalTest.h"
#include "EngineUtils.h"

void UPoleStarBlueprintFunctionLibrary::FireTestTrigger(const UObject* WorldContextObject, const FGameplayTag& TriggerTag)
{
	for (TActorIterator<ADataDrivenFunctionalTest> It(WorldContextObject->GetWorld()); It; ++It)
	{
		if (ADataDrivenFunctionalTest* Test = *It)
			Test->Trigger(TriggerTag);
	}
}
