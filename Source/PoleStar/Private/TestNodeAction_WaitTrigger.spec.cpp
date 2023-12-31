#include "Misc/AutomationTest.h"
#include "DataDrivenFunctionalTest.h"
#include "AutomationSpecWorld.h"
#include "TestNodeAction_WaitTrigger.h"
#include "NativeGameplayTags.h"

namespace PoleStarTags
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(WaitTriggerTestTag, "PoleStar.Test.WaitTriggerTag");
}

BEGIN_DEFINE_SPEC(FPoleStarWaitTriggerTests, "Project.ActorTests", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	FAutomationSpecWorld AutomationWorld;
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
			TestActor = AutomationWorld.AddActor<ADataDrivenFunctionalTest>();
			WaitTriggerNode = NewObject<UTestNodeAction_WaitTrigger>();
			WaitTriggerNode->TriggerTag = PoleStarTags::WaitTriggerTestTag;
		});

		AfterEach([this]()
		{
			AutomationWorld.AfterEach();
		});

		It("Immediate success on start when pre-triggered", [this]()
		{
			TestActor->Trigger(PoleStarTags::WaitTriggerTestTag);
			ETestNodeActionResult Result = WaitTriggerNode->OnTestNodeStart(TestActor, FVector::ZeroVector, AutomationWorld.GetPawn());
			TestEqual("result", Result, ETestNodeActionResult::Success);
		});

		It("Success when triggered after the start", [this]()
		{
			ETestNodeActionResult StartResult = WaitTriggerNode->OnTestNodeStart(TestActor, FVector::ZeroVector, AutomationWorld.GetPawn());
			TestEqual("StartResult", StartResult, ETestNodeActionResult::Ongoing);

			TestActor->Trigger(PoleStarTags::WaitTriggerTestTag);

			ETestNodeActionResult TickResult = WaitTriggerNode->OnTestNodeTick(TestActor, FVector::ZeroVector, AutomationWorld.GetPawn(), 0.1f);
			TestEqual("TickResult", TickResult, ETestNodeActionResult::Success);
		});
	});
}

#undef TEST_STUB