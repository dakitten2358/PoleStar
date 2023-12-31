#pragma once

#include "CoreMinimal.h"
#include "TestNode.generated.h"

class UTestNodeAction;

USTRUCT(BlueprintType)
struct POLESTAR_API FTestNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget=true))
	FVector Location;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<TObjectPtr<UTestNodeAction>> Actions;
};
