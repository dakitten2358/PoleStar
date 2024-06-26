#include "Misc/AutomationTest.h"
#include "DataDrivenFunctionalTest.h"
#include "AutomationSpecWorld.h"
#include "TestNodeAction_WaitTrigger.h"
#include "PoleStarGameplayTags.h"

BEGIN_DEFINE_SPEC(FPoleStarWaitTriggerTests, "Project.ActorTests", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	FAutomationSpecWorld AutomationWorld;
	TObjectPtr<APlayerStart> PlayerStart;
	TObjectPtr<ADataDrivenFunctionalTest> TestActor;
	TObjectPtr<UTestNodeAction_WaitTrigger> WaitTriggerNode;
END_DEFINE_SPEC(FPoleStarWaitTriggerTests)

#define TEST_STUB(text) It(text, [this]() {TestTrue("implemented", false); })

void FPoleStarWaitTriggerTests::Define()
{
	Describe("TestNodeAction_WaitTrigger", [this]()
	{
		BeforeEach([this]()
		{
			AutomationWorld.BeforeEach();
			PlayerStart = AutomationWorld.AddActor<APlayerStart>();
			TestActor = AutomationWorld.AddActorDeferred<ADataDrivenFunctionalTest>();
			TestActor->PlayerStart = PlayerStart;
			AutomationWorld.FinishAddActor(TestActor);
			WaitTriggerNode = NewObject<UTestNodeAction_WaitTrigger>();
			WaitTriggerNode->TriggerTag = PoleStarGameplayTags::Test_Trigger_Default;
		});

		AfterEach([this]()
		{
			AutomationWorld.AfterEach();
		});

		It("Immediate success on start when pre-triggered", [this]()
		{
			TestActor->Trigger(PoleStarGameplayTags::Test_Trigger_Default);
			ETestNodeActionResult Result = WaitTriggerNode->OnTestNodeStart(TestActor, FVector::ZeroVector, AutomationWorld.GetPawn());
			TestEqual("result", Result, ETestNodeActionResult::Success);
		});

		It("Success when triggered after the start", [this]()
		{
			ETestNodeActionResult StartResult = WaitTriggerNode->OnTestNodeStart(TestActor, FVector::ZeroVector, AutomationWorld.GetPawn());
			TestEqual("StartResult", StartResult, ETestNodeActionResult::Ongoing);

			TestActor->Trigger(PoleStarGameplayTags::Test_Trigger_Default);

			ETestNodeActionResult TickResult = WaitTriggerNode->OnTestNodeTick(TestActor, FVector::ZeroVector, AutomationWorld.GetPawn(), 0.1f);
			TestEqual("TickResult", TickResult, ETestNodeActionResult::Success);
		});
	});
}

#undef TEST_STUB