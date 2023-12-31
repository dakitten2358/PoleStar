#include "Misc/AutomationTest.h"
#include "DataDrivenFunctionalTest.h"
#include "AutomationSpecWorld.h"
#include "NativeGameplayTags.h"

namespace PoleStarTags
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TestTag, "PoleStar.Test.ExampleTag");
}

BEGIN_DEFINE_SPEC(FPoleStarDataDrivenFunctionalTestTests, "Project.ActorTests", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	FAutomationSpecWorld AutomationWorld;
	TObjectPtr<ADataDrivenFunctionalTest> TestActor;
END_DEFINE_SPEC(FPoleStarDataDrivenFunctionalTestTests)

#define TEST_STUB(text) It(text, [this]() {TestTrue("implemented", false); })

void FPoleStarDataDrivenFunctionalTestTests::Define()
{
	Describe("ADataDrivenFunctionalTest", [this]()
	{
		BeforeEach([this]()
		{
			AutomationWorld.BeforeEach();
			TestActor = AutomationWorld.AddActor<ADataDrivenFunctionalTest>();
		});

		AfterEach([this]()
		{
			AutomationWorld.AfterEach();
		});

		It("should be able to set a trigger", [this]()
		{
			TestActor->Trigger(PoleStarTags::TestTag);
			TestTrue("triggered", TestActor->IsTriggered(PoleStarTags::TestTag));
		});

		It("should be able to handle a trigger that hasn't be triggered yet", [this]()
		{
			TestFalse("triggered", TestActor->IsTriggered(PoleStarTags::TestTag));
		});

		It("should be triggerable from blueprint", [this]()
		{
			UPoleStarBlueprintFunctionLibrary::FireTestTrigger(TestActor, PoleStarTags::TestTag);
			TestTrue("triggered", TestActor->IsTriggered(PoleStarTags::TestTag));
		});
	});
}

#undef TEST_STUB