// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "QuestLogic/QuestTriggers/QuestTrigger_Service_Or.h"

#include "Utils/StringUtils.h"

FString UQuestTrigger_Service_Or::GetDescription(int32 IndentLevel) const
{
	FString Result(TEXT("Or:\n"));
	
	if (Triggers.Num() > 0)
	{
		for (const UQuestTrigger* Trigger : Triggers)
		{
			Result.Append(FString::Printf(TEXT("%s* %s\n"), *StringUtils::MakeIndent(IndentLevel), Trigger ? *Trigger->GetDescription(IndentLevel + 1) : TEXT("NONE")));
		}
	}
	else
	{
		Result.Append(FString::Printf(TEXT("%s-"), *StringUtils::MakeIndent(IndentLevel)));	
	}
	
	return Result;
}

void UQuestTrigger_Service_Or::OnTrackingBegins_Implementation()
{
	for (UQuestTrigger* Trigger : Triggers)
	{
		if (Trigger)
		{
			Trigger->OnTriggeredStateChanged.BindUObject(this, &UQuestTrigger_Service_Or::OnOtherTriggerChangeState);
			Trigger->BeginTracking();
		}
	}
}

void UQuestTrigger_Service_Or::OnTrackingEnded_Implementation()
{
	for (UQuestTrigger* Trigger : Triggers)
	{
		if (Trigger)
		{
			Trigger->EndTracking();
			Trigger->OnTriggeredStateChanged.Unbind();
		}
	}
}

void UQuestTrigger_Service_Or::OnUndoTriggeredState_Implementation()
{
	for (UQuestTrigger* Trigger : Triggers)
	{
		if (Trigger)
		{
			Trigger->UndoTriggeredState();
		}
	}
}

void UQuestTrigger_Service_Or::OnOtherTriggerChangeState(const UQuestTrigger* InTrigger, bool bNewTriggeredState)
{
	if (bNewTriggeredState)
	{
		EnterTriggeredState();
	}
}
