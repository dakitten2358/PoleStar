#include "AutomationSpecWorld.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemTestPawn.h"
#include "GameplayAbilitiesModule.h"
#include "AbilitySystemGlobals.h"
#include "GameplayTagsManager.h"

void FAutomationSpecWorld::BeforeEach()
{
	// world
	World = UWorld::CreateWorld(EWorldType::Game, false);
	FWorldContext &WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
	WorldContext.SetCurrentWorld(World);

	FURL URL;
	World->InitializeActorsForPlay(URL);
	World->BeginPlay();

	InitialFrameCounter = GFrameCounter;

	// actors
	AddPawn();
}

void FAutomationSpecWorld::AfterEach()
{
	// clear out any components that we created
	for(TObjectPtr<UActorComponent> CreatedComponent : CreatedComponents)
		CreatedComponent->DestroyComponent();
	CreatedComponents.Reset();

	// clear out any actors we created
	for(TObjectPtr<AActor> Actor : CreatedActors)
		World->EditorDestroyActor(Actor, false);
	CreatedActors.Reset();

	// world
	GFrameCounter = InitialFrameCounter;

	GEngine->DestroyWorldContext(World);
	World->DestroyWorld(false);
}

void FAutomationSpecWorld::TickWorld(float Time)
{
	const float step = 0.1f;
	while (Time > 0.f)
	{
		World->Tick(ELevelTick::LEVELTICK_All, FMath::Min(Time, step));
		Time -= step;

		// This is terrible but required for subticking like this.
		// cache the real GFrameCounter at the start of our tests and restore it when finished.
		GFrameCounter++;
	}
}

UWorld* FAutomationSpecWorld::GetWorld()
{
	return World;
}

ADefaultPawn* FAutomationSpecWorld::GetPawn()
{
	return Cast<ADefaultPawn>(CreatedActors[0]);
}

ADefaultPawn* FAutomationSpecWorld::AddPawn()
{
	CreatedActors.Add(World->SpawnActor<AAbilitySystemTestPawn>());
	return Cast<ADefaultPawn>(CreatedActors.Last());
}

UAbilitySystemComponent* FAutomationSpecWorld::GetAbilitySystemComponent()
{
	return Cast<AAbilitySystemTestPawn>(GetPawn())->GetAbilitySystemComponent();
}
