// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "QuestLogic/QuestTriggers/QuestTrigger.h"

UQuestTrigger::UQuestTrigger()
{
	bTickableTrigger = false;
	bCanUndo = true;
	bInvertTriggeredState = false;
	bIsTriggered = false;
	bIsTracking = false;
}

FString UQuestTrigger::GetDescription(int32 IndentLevel) const
{
	const FString TriggerName = GetClass()->GetName()
	                                      .Replace(TEXT("BP_"), TEXT(""))
	                                      .Replace(TEXT("QuestTrigger_"), TEXT(""))
	                                      .Replace(TEXT("_C"), TEXT(""));
	
	return FString::Printf(TEXT("%s%s"), bInvertTriggeredState ? TEXT("Not ") : TEXT(""), *TriggerName);
}

void UQuestTrigger::BeginTracking()
{
	bIsTracking = true;
	bIsTriggered = false;

	OnTrackingBegins();

	if (!bIsTriggered)
	{
		// Execute OnTriggeredStateChanged for handling inverted trigger cases
		// If bIsTriggered is true, there was OnTriggeredStateChanged execution already so we don't need to execute it again
		OnTriggeredStateChanged.ExecuteIfBound(this, IsTriggered());
	}
}

void UQuestTrigger::EndTracking()
{
	bIsTracking = false;
	OnTrackingEnded();
}

void UQuestTrigger::Tick(float DeltaTime)
{
	ReceiveTick(DeltaTime);
}

bool UQuestTrigger::IsTickable() const
{
	return bTickableTrigger && bIsTracking;
}

TStatId UQuestTrigger::GetStatId() const
{
	return TStatId();
}

void UQuestTrigger::OnTrackingBegins_Implementation()
{
	// pass
}

void UQuestTrigger::OnTrackingEnded_Implementation()
{
	// pass
}

void UQuestTrigger::OnUndoTriggeredState_Implementation()
{
	// pass
}

void UQuestTrigger::EnterTriggeredState()
{
	if (!bIsTriggered)
	{
		bIsTriggered = true;
		OnTriggeredStateChanged.ExecuteIfBound(this, IsTriggered());
	}
}

void UQuestTrigger::UndoTriggeredState()
{
	if (bCanUndo && bIsTriggered)
	{
		bIsTriggered = false;
		OnUndoTriggeredState();
		OnTriggeredStateChanged.ExecuteIfBound(this, IsTriggered());
	}
}
