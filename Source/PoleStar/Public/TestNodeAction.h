#pragma once

#include "UObject/ObjectMacros.h"
#include "TestNodeAction.generated.h"

class APawn;

UENUM(BlueprintType)
enum class ETestNodeActionResult : uint8
{
    Ongoing UMETA(DisplayName = "Ongoing"),
    Success UMETA(DisplayName = "Success"),
    Failed  UMETA(DisplayName = "Failed"),
};

UENUM(BlueprintType)
enum class ETestNodeEndReason : uint8
{
	Requested UMETA(DisplayName="Requested"),
	Aborted UMETA(DisplayName="Aborted"),
};

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class POLESTAR_API UTestNodeAction : public UObject
{
	GENERATED_BODY()
	
public:
	virtual ETestNodeActionResult OnTestNodeStart(const FVector& NodeLocation, TObjectPtr<APawn> Pawn) { return ETestNodeActionResult::Success; }
	virtual ETestNodeActionResult OnTestNodeTick(const FVector& NodeLocation, TObjectPtr<APawn> Pawn, float DeltaTime) { return ETestNodeActionResult::Success; }
	virtual void OnTestNodeEnd(ETestNodeEndReason ReasonForEnd) {}
};

#if UE_ENABLE_INCLUDE_ORDER_DEPRECATED_IN_5_2
#include "CoreMinimal.h"
#endif
