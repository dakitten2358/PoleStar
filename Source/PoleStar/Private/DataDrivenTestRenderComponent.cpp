#include "DataDrivenTestRenderComponent.h"
#include "Engine/Canvas.h"
#include "TestNode.h"
#include "TestNodeSource.h"
#include "SceneInterface.h"
#include "TestNodeAction_MoveToLocation.h"
#include "TestAcceptanceRadiusProvider.h"
#include "GameFramework/PlayerStart.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DataDrivenTestRenderComponent)

SIZE_T FTestSceneProxy::GetTypeHash() const
{
	static size_t UniquePointer;
	return reinterpret_cast<size_t>(&UniquePointer);
}

FTestSceneProxy::FTestSceneProxy(const UPrimitiveComponent& InComponent)
	: FDebugRenderSceneProxy(&InComponent)
	, ActorOwner(nullptr)
{
	FColor NoHitColor = FColor::Green;
	FColor HitColor = FColor::Red;
	FVector BoxSize = FVector(6.0f, 6.0f, 6.0f);

	bWantsSelectionOutline = false;
	
	ActorOwner = InComponent.GetOwner();
	TestNodeSource = Cast<const ITestNodeSource>(ActorOwner);
	
	if (TestNodeSource)
	{
		FVector Offset = ActorOwner->GetActorLocation();
		const TArray<FTestNode>& TestNodes = TestNodeSource->GetTestNodes();
		if (TestNodes.Num() > 1)
		{
			// draw lines between the nodes
			for (int nodeIndex = 1; nodeIndex < TestNodes.Num(); ++nodeIndex)
				Lines.Emplace(TestNodes[nodeIndex - 1].Location + Offset, TestNodes[nodeIndex].Location + Offset, FColor::Green, 2.0f);

			// draw spheres around each node
			for (const FTestNode& TestNode : TestNodes)
			{
				float Radius = UTestNodeAction_MoveToLocation::DefaultArrivalRadius;
				for (const TObjectPtr<UTestNodeAction>& Action : TestNode.Actions)
				{
					if (ITestAcceptanceRadiusProvider* RadiusProvider = Cast<ITestAcceptanceRadiusProvider>(Action))
					{
						Radius = RadiusProvider->GetAcceptanceRadius();
						break;
					}
				}
				Spheres.Add(FSphere(Radius, TestNode.Location + Offset, FColor::Green));
			}
		}

		// if we have at least one node, find the nearest player start
		// and draw a line from that
		if (TestNodes.Num() > 0)
		{
			for (TObjectIterator<APlayerStart> Itr; Itr; ++Itr)
			{
				if (Itr->IsA(APlayerStart::StaticClass()))
				{
					APlayerStart* PlayerStart = Cast<APlayerStart>(*Itr);
					Lines.Emplace(PlayerStart->GetActorLocation(), TestNodes[0].Location + Offset, FColor::Green);
					break;
				}
			}
		}
	}
}

FPrimitiveViewRelevance FTestSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = true; //View->Family->EngineShowFlags.GetSingleFlag(ViewFlagIndex) && IsShown(View) 
		/*&& (!bDrawOnlyWhenSelected || SafeIsActorSelected())*/;
	Result.bDynamicRelevance = true;
	// ideally the TranslucencyRelevance should be filled out by the material, here we do it conservative
	Result.bSeparateTranslucency = Result.bNormalTranslucency = IsShown(View);
	Result.bOpaque = true;
	Result.bEditorPrimitiveRelevance = true;
	return Result;
}

UDataDrivenTestRenderComponent::UDataDrivenTestRenderComponent(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

#if UE_ENABLE_DEBUG_DRAWING
FDebugRenderSceneProxy* UDataDrivenTestRenderComponent::CreateDebugSceneProxy()
{
	FTestSceneProxy* NewSceneProxy = new FTestSceneProxy(*this);
	return NewSceneProxy;
}
#endif

FBoxSphereBounds UDataDrivenTestRenderComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds::Builder DebugBoundsBuilder;

	AActor* ActorOwner = Cast<AActor>(GetOwner());
	if (ActorOwner) DebugBoundsBuilder += ActorOwner->GetActorLocation();

	const ITestNodeSource* TestNodeSource = Cast<const ITestNodeSource>(GetOwner());
	const TArray<FTestNode>& TestNodes = TestNodeSource->GetTestNodes();

	FVector Offset = ActorOwner->GetActorLocation();
	for(const FTestNode& TestNode : TestNodes)
		DebugBoundsBuilder += TestNode.Location + Offset;

	return DebugBoundsBuilder;
}
