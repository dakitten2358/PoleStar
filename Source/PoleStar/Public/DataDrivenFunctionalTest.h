#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "TestNode.h"
#include "TestNodeSource.h"
#include "TestNodeRunner.h"
#include "DataDrivenFunctionalTest.generated.h"

class UDataDrivenTestRenderComponent;
class UTestNodeAction;

UCLASS(Blueprintable, BlueprintType)
class POLESTAR_API ADataDrivenFunctionalTest : public AFunctionalTest, public ITestNodeSource
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functional Testing")
	TArray<FTestNode> Nodes;

	// ITestNodeSource
	TArray<FTestNode> GetTestNodes() const { return Nodes; }

	// AActor
	virtual void Tick(float DeltaSeconds) override;

	// FunctionalTest
	virtual void PrepareTest() override;

protected:
	void UpdateDrawing();

private:
	FTestNodeRunner NodeRunner;

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<APawn> TestPawn;
	
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
};
