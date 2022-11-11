// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "Components/QuestGiverComponent.h"

#include "Components/QuestLogComponent.h"
#include "QuestData/Quest.h"
#include "Types/QuestTypes.h"

bool UQuestGiverComponent::GiveAllQuests(UQuestLogComponent* ToQuestLog)
{
	bool bResult = false;
	for (int32 Index = 0; Index < AvailableQuests.Num(); ++Index)
	{
		bResult |= GiveQuestWithIndex(ToQuestLog, Index);
	}
	return bResult;
}

bool UQuestGiverComponent::GiveNextQuest(UQuestLogComponent* ToQuestLog)
{
	if (++CurrentQuestIndex >= AvailableQuests.Num())
	{
		if (bLoopQuestGiving)
		{
			CurrentQuestIndex = 0;
		}
		else
		{
			UE_LOG(LogQuest, Warning, TEXT("Trying to give quest, but we already reached last available quest and bLoopQuestGiving is FALSE!"));
			return false;
		}
	}

	const bool bSuccessGiving = GiveQuestWithIndex(ToQuestLog, CurrentQuestIndex);
	if (!bSuccessGiving && !AvailableQuests[CurrentQuestIndex].bShouldGoToNextOnGivingFailure)
	{
		--CurrentQuestIndex;
	}
	
	return bSuccessGiving;
}

bool UQuestGiverComponent::GiveQuestWithIndex(UQuestLogComponent* ToQuestLog, int32 QuestIndex)
{
	UE_LOG(LogQuest, Display, TEXT("Trying to give quest with index %d from %s"), QuestIndex, *GetOwner()->GetName());

	if (!ToQuestLog)
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Quest log component is not valid!"), *FString(__FUNCTION__));
		return false;
	}
	
	if (!AvailableQuests.IsValidIndex(QuestIndex))
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Index is not valid. Index: %d. Must be in bounds [0, %d]"), *FString(__FUNCTION__), QuestIndex, AvailableQuests.Num() - 1);
		return false;
	}

	const UQuest* QuestToGive = AvailableQuests[QuestIndex].Quest;
	if (!QuestToGive)
	{
		UE_LOG(LogQuest, Error, TEXT("%s() We want to give quest, but quest with index %d is not valid!"), *FString(__FUNCTION__), QuestIndex);
		return false;
	}

	return ToQuestLog->AddNewQuest(QuestToGive);
}
