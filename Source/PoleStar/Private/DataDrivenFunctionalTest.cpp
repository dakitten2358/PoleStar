#include "DataDrivenFunctionalTest.h"
#include "DataDrivenTestRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PoleStarLogChannels.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

#if WITH_EDITOR
#include "Misc/TransactionObjectEvent.h"
#include "Editor/EditorEngine.h"
extern UNREALED_API UEditorEngine* GEditor;
#endif // 

ADataDrivenFunctionalTest::ADataDrivenFunctionalTest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	TestRenderComponent = CreateEditorOnlyDefaultSubobject<UDataDrivenTestRenderComponent>(TEXT("TestRender"));
	if (TestRenderComponent)
	{
		TestRenderComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
#endif
}

bool ADataDrivenFunctionalTest::IsReady_Implementation()
{
	bool isReady = Super::IsReady_Implementation();
	if (isReady)
	{
		if (IsValid(TestPawn))
			return true;

		AttemptToAcquirePawn();
		isReady = IsValid(TestPawn) && IsValid(FindPlayerStart());
	}
	return isReady;
}

void ADataDrivenFunctionalTest::StartTest()
{
	Super::StartTest();

	// Prep for running the test
	NodeRunner = FTestNodeRunner();
	ResetTriggerStates();
	TeleportToPlayerStart();
}

void ADataDrivenFunctionalTest::AttemptToAcquirePawn()
{
	TObjectPtr<APawn> Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(Pawn))
		TestPawn = Pawn;
}

void ADataDrivenFunctionalTest::Trigger(const FGameplayTag& TriggerTag)
{
	TriggeredTags.Add(TriggerTag);
}

bool ADataDrivenFunctionalTest::IsTriggered(const FGameplayTag& TriggerTag) const
{
	return TriggeredTags.Contains(TriggerTag);
}

void ADataDrivenFunctionalTest::ResetTriggerStates()
{
	TriggeredTags.Empty();
}

void ADataDrivenFunctionalTest::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsRunning == false)
		return;
	
	if (IsReady())
	{
		ETestNodeActionResult NodesResult = NodeRunner.Tick(this, Nodes, TestPawn, DeltaSeconds);
		switch (NodesResult)
		{
			case ETestNodeActionResult::Success:
				FinishTest(EFunctionalTestResult::Succeeded, FString(TEXT("Yay")));
				return;
			case ETestNodeActionResult::Failed:
				FinishTest(EFunctionalTestResult::Failed, FString(TEXT("OMGFAIL")));
				return;
		}
	}
}

TObjectPtr<APlayerStart> ADataDrivenFunctionalTest::FindPlayerStart() const
{
	// if once was explicitly set, use that
	if (IsValid(PlayerStart))
		return PlayerStart;

	// otherwise prefer the nearest player start
	float BestDistance = MAX_FLT;
	TObjectPtr<APlayerStart> BestPlayerStart = nullptr;
	for (TActorIterator<APlayerStart> PlayerStartIterator(GetWorld()); PlayerStartIterator; ++PlayerStartIterator)
	{
		TObjectPtr<APlayerStart> CurrentPlayerStart = *PlayerStartIterator;
		if (IsValid(CurrentPlayerStart))
		{
			float Distance = FVector::Distance(CurrentPlayerStart->GetActorLocation(), GetActorLocation());
			if (Distance < BestDistance)
			{
				BestDistance = Distance;
				BestPlayerStart = CurrentPlayerStart;
			}
		}
	}
	
	if (!IsValid(BestPlayerStart))
	{
		UE_LOG(LogPoleStar, Error, TEXT("No player start found for test %s"), *GetName());
	}
	return BestPlayerStart;
}

void ADataDrivenFunctionalTest::TeleportToPlayerStart()
{
	if (APlayerStart* TargetPlayerStart = FindPlayerStart())
	{
		TestPawn->TeleportTo(TargetPlayerStart->GetActorLocation(), TargetPlayerStart->GetActorRotation());
	}
}

void ADataDrivenFunctionalTest::UpdateDrawing()
{
#if WITH_EDITORONLY_DATA
	if (HasAnyFlags(RF_ClassDefaultObject) == true)
	{
		return;
	}

	if (TestRenderComponent != NULL && TestRenderComponent->GetVisibleFlag())
	{
		TestRenderComponent->MarkRenderStateDirty();

#if WITH_EDITOR
		if (GEditor != NULL)
		{
			GEditor->RedrawLevelEditingViewports();
		}
#endif // WITH_EDITOR
	}
#endif // WITH_EDITORONLY_DATA
}

#if WITH_EDITOR
void ADataDrivenFunctionalTest::OnPropertyChanged(const FName PropName)
{
	UpdateDrawing();
}

void ADataDrivenFunctionalTest::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		OnPropertyChanged(PropertyChangedEvent.MemberProperty->GetFName());
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ADataDrivenFunctionalTest::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	UpdateDrawing();
}

void ADataDrivenFunctionalTest::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	Super::PostTransacted(TransactionEvent);

	if (TransactionEvent.GetEventType() == ETransactionObjectEventType::UndoRedo)
	{
		if (TransactionEvent.GetChangedProperties().Num() > 0)
		{
			// targeted update
			for (const FName PropertyName : TransactionEvent.GetChangedProperties())
			{
				OnPropertyChanged(PropertyName);
			}
		}
		else
		{
			// fallback - make sure the results are up to date
			UpdateDrawing();
		}
	}
}

#endif // WITH_EDITOR