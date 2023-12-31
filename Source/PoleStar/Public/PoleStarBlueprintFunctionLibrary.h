

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PoleStarBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class POLESTAR_API UPoleStarBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Test)
	static void FireTestTrigger(const UObject* WorldContextObject, const FGameplayTag& TriggerTag);
};
