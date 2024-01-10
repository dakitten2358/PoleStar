#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "TestNode.h"
#include "TestNodeSource.h"
#include "TestNodeRunner.h"
#include "PoleStarTest.h"
#include "DataDrivenFunctionalTest.generated.h"

class UDataDrivenTestRenderComponent;
class UTestNodeAction;

UCLASS(Blueprintable, BlueprintType)
class POLESTAR_API ADataDrivenFunctionalTest : public AFunctionalTest, public ITestNodeSource, public IPoleStarTest
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Functional Testing", meta=(ToolTip="The player start to use when running the test (Optional)"))
	TObjectPtr<class APlayerStart> PlayerStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functional Testing")
	TArray<FTestNode> Nodes;

	// ITestNodeSource
	TArray<FTestNode> GetTestNodes() const { return Nodes; }

	// IPoleStarTest
	virtual void Trigger(const FGameplayTag& TriggerTag) override;
	virtual bool IsTriggered(const FGameplayTag& TriggerTag) const override;

	// AActor
	virtual void Tick(float DeltaSeconds) override;

protected:
	void UpdateDrawing();

	// AFunctionalTest
	virtual bool IsReady_Implementation() override;

private:
	FTestNodeRunner NodeRunner;

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<APawn> TestPawn;

	TArray<FGameplayTag> TriggeredTags;
	void ResetTriggerStates();

	virtual void StartTest() override;
	void AttemptToAcquirePawn();

#if WITH_EDITORONLY_DATA
#if WITH_EDITOR
public:
	void OnPropertyChanged(const FName PropName);
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;
#endif // WITH_EDITOR
private:
	/** Editor Preview */
	UPROPERTY(Transient)
	TObjectPtr<UDataDrivenTestRenderComponent> TestRenderComponent;
#endif // WITH_EDITORONLY_DATA

public:
	TObjectPtr<class APlayerStart> FindPlayerStart() const;

protected:
	void TeleportToPlayerStart();
};
