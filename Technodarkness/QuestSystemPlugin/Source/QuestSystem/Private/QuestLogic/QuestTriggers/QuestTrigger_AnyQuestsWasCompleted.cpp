// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestTriggers/QuestTrigger_AnyQuestsWasCompleted.h"

#include "Components/QuestLogComponent.h"
#include "QuestData/Quest.h"


void UQuestTrigger_AnyQuestsWasCompleted::OnTrackingBegins_Implementation()
{
	for (UQuest* Quest : QuestsForTracking)
	{
		if (Quest && GetQuestLog()->IsQuestActive(Quest))
		{
			Quest->OnQuestEndedDelegate.AddDynamic(this, &UQuestTrigger_AnyQuestsWasCompleted::OnQuestCompleted);
		}
	}
}

void UQuestTrigger_AnyQuestsWasCompleted::OnTrackingEnded_Implementation()
{
	for (UQuest* Quest : QuestsForTracking)
	{
		if (Quest)
		{
			Quest->OnQuestEndedDelegate.RemoveDynamic(this, &UQuestTrigger_AnyQuestsWasCompleted::OnQuestCompleted);
		}
	}
}

void UQuestTrigger_AnyQuestsWasCompleted::OnQuestCompleted(UQuest* InQuest)
{
	if (bShouldFilterByCompletionType)
	{
		if (FilterCompletionType == EQuestCompletionType::Fail && InQuest->GetCompletionType() == EQuestCompletionType::Fail)
		{
			EnterTriggeredState();
		}
		else if (FilterCompletionType == EQuestCompletionType::Success && InQuest->GetCompletionType() == EQuestCompletionType::Success)
		{
			EnterTriggeredState();
		}
	}
	else
	{
		EnterTriggeredState();
	}
}
