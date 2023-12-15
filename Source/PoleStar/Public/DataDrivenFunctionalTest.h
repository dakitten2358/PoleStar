#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "TestNode.h"
#include "TestNodeSource.h"
#include "DataDrivenFunctionalTest.generated.h"

class UDataDrivenTestRenderComponent;

UCLASS(Blueprintable, BlueprintType)
class POLESTAR_API ADataDrivenFunctionalTest : public AFunctionalTest, public ITestNodeSource
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category="Funcitonal Testing", AdvancedDisplay)
	bool NodeMovementRelative;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Functional Testing")
	TArray<FTestNode> Nodes;

	// ITestNodeSource
	TArray<FTestNode> GetTestNodes() const { return Nodes; }

protected:
	void UpdateDrawing();
	
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
