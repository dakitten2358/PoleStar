#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "PoleStarTest.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoleStarTest : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class POLESTAR_API IPoleStarTest
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Trigger(const FGameplayTag& TriggerTag) = 0;
	virtual bool IsTriggered(const FGameplayTag& TriggerTag) const = 0;
};
