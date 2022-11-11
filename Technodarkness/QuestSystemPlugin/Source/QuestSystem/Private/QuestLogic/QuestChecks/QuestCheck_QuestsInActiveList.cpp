// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestChecks/QuestCheck_QuestsInActiveList.h"

#include "Components/QuestLogComponent.h"

bool UQuestCheck_QuestsInActiveList::IsChecked(const UQuestLogComponent* InQuestLog) const
{
	return InQuestLog && InQuestLog->AreQuestsActive(RequireQuests);
}
