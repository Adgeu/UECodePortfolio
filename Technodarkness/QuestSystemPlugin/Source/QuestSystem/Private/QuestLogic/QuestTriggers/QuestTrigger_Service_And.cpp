// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestTriggers/QuestTrigger_Service_And.h"

#include "Utils/StringUtils.h"

FString UQuestTrigger_Service_And::GetDescription(int32 IndentLevel) const
{
	FString Result(TEXT("And:\n"));
	
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

void UQuestTrigger_Service_And::OnTrackingBegins_Implementation()
{
	checkf(Triggers.Num() <= UINT8_MAX, TEXT("%s() Max count of triggers must be less than %d. Now it's %d"), *FString(__FUNCTION__), UINT8_MAX, Triggers.Num());

	for (UQuestTrigger* Trigger : Triggers)
	{
		if (Trigger)
		{
			++TargetTriggeredCount;
			Trigger->OnTriggeredStateChanged.BindUObject(this, &UQuestTrigger_Service_And::OnOtherTriggerChangeState);
			Trigger->BeginTracking();
		}
	}
}

void UQuestTrigger_Service_And::OnTrackingEnded_Implementation()
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

void UQuestTrigger_Service_And::OnUndoTriggeredState_Implementation()
{
	for (UQuestTrigger* Trigger : Triggers)
	{
		if (Trigger)
		{
			Trigger->UndoTriggeredState();
		}
	}
}

void UQuestTrigger_Service_And::OnOtherTriggerChangeState(const UQuestTrigger* InTrigger, bool bNewTriggeredState)
{
	if (bNewTriggeredState)
	{
		if (++TriggeredCount >= TargetTriggeredCount)
		{
			EnterTriggeredState();
		}
	}
	else
	{
		--TriggeredCount;
	}
}
