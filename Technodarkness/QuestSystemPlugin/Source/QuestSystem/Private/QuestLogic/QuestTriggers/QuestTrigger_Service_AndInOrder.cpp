// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestTriggers/QuestTrigger_Service_AndInOrder.h"

#include "Utils/StringUtils.h"

FString UQuestTrigger_Service_AndInOrder::GetDescription(int32 IndentLevel) const
{
	FString Result(TEXT("And In Order:\n"));
	
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

void UQuestTrigger_Service_AndInOrder::OnTrackingBegins_Implementation()
{
	CurrentTrackingTriggerIndex = -1;
	TryTrackNextOrExit();
}

void UQuestTrigger_Service_AndInOrder::OnTrackingEnded_Implementation()
{
	for (UQuestTrigger* Trigger : Triggers)
	{
		if (Trigger && Trigger->IsTracking())
		{
			Trigger->EndTracking();
			Trigger->OnTriggeredStateChanged.Unbind();
		}
	}
}

void UQuestTrigger_Service_AndInOrder::OnUndoTriggeredState_Implementation()
{
	CurrentTrackingTriggerIndex = -1;

	for (UQuestTrigger* Trigger : Triggers)
	{
		if (Trigger && Trigger->IsTracking())
		{
			Trigger->EndTracking();
			Trigger->OnTriggeredStateChanged.Unbind();
		}
	}

	bShouldHandleOthersUndoTriggered = false;
	for (UQuestTrigger* Trigger : Triggers)
	{
		if (Trigger)
		{
			Trigger->UndoTriggeredState();
		}
	}
	bShouldHandleOthersUndoTriggered = true;

	TryTrackNextOrExit();
}

void UQuestTrigger_Service_AndInOrder::OnOtherTriggerChangeState(const UQuestTrigger* InTrigger, bool bNewTriggeredState)
{
	if (bNewTriggeredState)
	{
		TryTrackNextOrExit();
	}
	else
	{
		if (bShouldHandleOthersUndoTriggered)
		{
			HandleOtherTriggerOnUndoTriggered(InTrigger);
		}
	}
}

void UQuestTrigger_Service_AndInOrder::HandleOtherTriggerOnUndoTriggered(const UQuestTrigger* InTrigger)
{
	const int32 UndoTriggerIndex = Triggers.IndexOfByKey(InTrigger);
	check(UndoTriggerIndex != INDEX_NONE);

	bShouldHandleOthersUndoTriggered = false;
	int32 TriggerIndex = UndoTriggerIndex;
	while (++TriggerIndex != Triggers.Num())
	{
		if (UQuestTrigger* Trigger = Triggers[TriggerIndex])
		{
			if (Trigger->IsTriggered())
			{
				Trigger->UndoTriggeredState();
			}
		
			if (Trigger->IsTracking())
			{
				Trigger->EndTracking();
				Trigger->OnTriggeredStateChanged.Unbind();
			}
		}
	}
	bShouldHandleOthersUndoTriggered = true;

	CurrentTrackingTriggerIndex = UndoTriggerIndex;
}

void UQuestTrigger_Service_AndInOrder::TryTrackNextOrExit()
{
	while (++CurrentTrackingTriggerIndex < Triggers.Num())
	{
		if (UQuestTrigger* NextTrigger = Triggers[CurrentTrackingTriggerIndex])
		{
			NextTrigger->OnTriggeredStateChanged.BindUObject(this, &UQuestTrigger_Service_AndInOrder::OnOtherTriggerChangeState);
			NextTrigger->BeginTracking();
			return;
		}
	}

	EnterTriggeredState();
}
