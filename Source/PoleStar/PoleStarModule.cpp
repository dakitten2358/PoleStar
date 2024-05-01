#pragma once

#include "CoreMinimal.h"
#include "AutomationWorkerMessages.h"
#include "MessageEndpoint.h"
#include "MessageEndpointBuilder.h"
#include "Modules/ModuleManager.h"
#include "PoleStarLogChannels.h"

class FPoleStarModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	typedef FPoleStarModule Self;

	void Initialize();

	void HandlePingMessage(const FAutomationWorkerPing& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void HandleStartTestSession(const FAutomationWorkerStartTestSession& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void HandleStopTestSession(const FAutomationWorkerStopTestSession& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void HandleRequestTestsMessage(const FAutomationWorkerRequestTests& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void HandleRunTestsMessage(const FAutomationWorkerRunTests& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void HandleStopTestsMessage(const struct FAutomationWorkerStopTests& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);

private:
	// Holds the messaging endpoint.
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> MessageEndpoint;
};


#define LOCTEXT_NAMESPACE "FPoleStarModule"

void FPoleStarModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	Initialize();
}

void FPoleStarModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FPoleStarModule::Initialize()
{
	UE_LOG(LogPoleStar, Log, TEXT("=========== INITIALIZE ==========="));
	MessageEndpoint = FMessageEndpoint::Builder("FPoleStarModule")
		.Handling<FAutomationWorkerPing>(this, &Self::HandlePingMessage)
		.Handling<FAutomationWorkerStartTestSession>(this, &Self::HandleStartTestSession)
		.Handling<FAutomationWorkerStopTestSession>(this, &Self::HandleStopTestSession)
		.Handling<FAutomationWorkerRequestTests>(this, &Self::HandleRequestTestsMessage)
		.Handling<FAutomationWorkerRunTests>(this, &Self::HandleRunTestsMessage)
		.Handling<FAutomationWorkerStopTests>(this, &Self::HandleStopTestsMessage)
		.WithInbox();
}

void FPoleStarModule::HandlePingMessage(const FAutomationWorkerPing& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	UE_LOG(LogPoleStar, Log, TEXT("Message: Ping"));
	MessageEndpoint->Send(FMessageEndpoint::MakeMessage<FAutomationWorkerPong>(), Context->GetSender());
}

void FPoleStarModule::HandleStartTestSession(const FAutomationWorkerStartTestSession& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	UE_LOG(LogPoleStar, Log, TEXT("Message: StartTestSession"));
}

void FPoleStarModule::HandleStopTestSession(const FAutomationWorkerStopTestSession& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	UE_LOG(LogPoleStar, Log, TEXT("Message: StopTestSession"));
}

void FPoleStarModule::HandleRequestTestsMessage(const FAutomationWorkerRequestTests& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	UE_LOG(LogPoleStar, Log, TEXT("Message: RequestTests"));
}

void FPoleStarModule::HandleRunTestsMessage(const FAutomationWorkerRunTests& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	UE_LOG(LogPoleStar, Log, TEXT("Message: RunTests"));
}

void FPoleStarModule::HandleStopTestsMessage(const struct FAutomationWorkerStopTests& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	UE_LOG(LogPoleStar, Log, TEXT("Message: StopTests"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPoleStarModule, PoleStar)