// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestChecks/QuestCheck_QuestsInCompletedList.h"

#include "Components/QuestLogComponent.h"
#include "QuestData/Quest.h"

bool UQuestCheck_QuestsInCompletedList::IsChecked(const UQuestLogComponent* InQuestLog) const
{
	if (!InQuestLog)
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Invalid quest log!"), *FString(__FUNCTION__));
		return false;
	}
	
	TArray<UQuest*> CompletedQuests;
	if (!GetCompletedQuests(InQuestLog, CompletedQuests))
	{
		return false;
	}

	return CheckRequiresInCompletedQuests(CompletedQuests);
}

bool UQuestCheck_QuestsInCompletedList::GetCompletedQuests(const UQuestLogComponent* InQuestLog, TArray<UQuest*>& OutCompletedQuests) const
{
	if (bShouldFilterByCompletionType)
	{
		switch (FilterCompletionType)
		{
			case EQuestCompletionType::Success:
				OutCompletedQuests = InQuestLog->GetSuccessfulCompletedQuests();
				break;
				
			case EQuestCompletionType::Fail:
				OutCompletedQuests = InQuestLog->GetFailedCompletedQuests();
				break;

			default:
				UE_LOG(LogQuest, Error, TEXT("%s() Invalid filter completion type"), *FString(__FUNCTION__));
				return false;
		}
	}
	else
	{
		OutCompletedQuests = InQuestLog->GetCompletedQuests();
	}

	return OutCompletedQuests.Num() > 0;
}

bool UQuestCheck_QuestsInCompletedList::CheckRequiresInCompletedQuests(const TArray<UQuest*>& CompletedQuests) const
{
	for (UQuest* RequireQuest : RequireQuests)
	{
		const bool bContains = RequireQuest && CompletedQuests.ContainsByPredicate([RequireQuest](const UQuest* CompletedQuest)
		{
			return RequireQuest->GetQuestUniqueName() == CompletedQuest->GetQuestUniqueName();
		});
		
		if (!bContains)
		{
			return false;
		}
	}
	return true;
}
