#pragma once

#include "CoreMinimal.h"
#include "TestNode.generated.h"

USTRUCT(BlueprintType)
struct POLESTAR_API FTestNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget=true))
	FVector TargetPosition;

	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<AActor> TargetNodeActor;
};
