#include "TestTriggerBox.h"
#include "PoleStarBlueprintFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "PoleStarGameplayTags.h"

namespace
{
	static const FColor TestTriggerBaseColor(100, 255, 100, 255);
	static const FName TestTriggerCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

ATestTriggerBox::ATestTriggerBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	UBoxComponent* BoxCollisionComponent = CastChecked<UBoxComponent>(GetCollisionComponent());

	BoxCollisionComponent->ShapeColor = TestTriggerBaseColor;
	BoxCollisionComponent->InitBoxExtent(FVector(40.0f, 40.0f, 40.0f));
	BoxCollisionComponent->SetCollisionProfileName(TestTriggerCollisionProfileName);

	TriggerTag = PoleStarGameplayTags::Test_Trigger_Default;
}

void ATestTriggerBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UPoleStarBlueprintFunctionLibrary::FireTestTrigger(this, TriggerTag);
}
