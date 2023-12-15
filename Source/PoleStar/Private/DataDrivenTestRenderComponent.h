#pragma once

#include "CoreMinimal.h"
#include "Debug/DebugDrawComponent.h"
#include "TestNodeSource.h"
#include "DataDrivenTestRenderComponent.generated.h"

class FTestSceneProxy final : public FDebugRenderSceneProxy
{
public:
	POLESTAR_API virtual SIZE_T GetTypeHash() const override;
	POLESTAR_API explicit FTestSceneProxy(const UPrimitiveComponent& InComponent);
	POLESTAR_API virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

private:
	// can be 0
	AActor* ActorOwner;
	const ITestNodeSource* TestNodeSource;
};

UCLASS(ClassGroup=Debug, MinimalAPI)
class UDataDrivenTestRenderComponent : public UDebugDrawComponent
{
	GENERATED_UCLASS_BODY()

protected:
	POLESTAR_API virtual FBoxSphereBounds CalcBounds(const FTransform &LocalToWorld) const override;

#if UE_ENABLE_DEBUG_DRAWING
	POLESTAR_API virtual FDebugRenderSceneProxy* CreateDebugSceneProxy() override;
#endif	
};
