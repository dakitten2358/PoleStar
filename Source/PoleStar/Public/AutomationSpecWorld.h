#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "Engine/World.h"

class ADefaultPawn;
class UAbilitySystemComponent;
class UWorld;

struct POLESTAR_API FAutomationSpecWorld
{
public:
	void BeforeEach();
	void AfterEach();

	void TickWorld(float Time);

	class UWorld* GetWorld();
	class ADefaultPawn* GetPawn();
	class UAbilitySystemComponent* GetAbilitySystemComponent();

	template<typename ComponentType>
	ComponentType* AddComponent(ADefaultPawn* TargetActor = nullptr)
	{
		if (TargetActor == nullptr)
			TargetActor = GetPawn();
		ComponentType* Component = NewObject<ComponentType>(TargetActor);
		Component->OnComponentCreated();
		Component->RegisterComponent();
		CreatedComponents.Add(Component);
		return Component;
	}

	template<typename ActorType>
	ActorType* AddActor()
	{
		ActorType* Actor = World->SpawnActor<ActorType>();
		CreatedActors.Add(Actor);
		return Actor;
	}

	template<typename ActorType>
	ActorType* AddActorDeferred()
	{
		ActorType* Actor = World->SpawnActorDeferred<ActorType>(ActorType::StaticClass(), FTransform());
		CreatedActors.Add(Actor);
		return Actor;
	}

	template<typename ActorType>
	void FinishAddActor(TObjectPtr<ActorType> Actor)
	{
		Actor.Get()->FinishSpawning(FTransform());
	}

	ADefaultPawn* AddPawn();

private:
	TObjectPtr<UWorld> World;
	uint64 InitialFrameCounter;

	TArray<TObjectPtr<UActorComponent>> CreatedComponents;
	TArray<TObjectPtr<AActor>> CreatedActors;
};
