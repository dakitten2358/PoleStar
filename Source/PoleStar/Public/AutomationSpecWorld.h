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

	ADefaultPawn* AddPawn();

private:
	TObjectPtr<UWorld> World;
	uint64 InitialFrameCounter;

	TArray<TObjectPtr<UActorComponent>> CreatedComponents;
	TArray<TObjectPtr<AActor>> CreatedActors;
};
