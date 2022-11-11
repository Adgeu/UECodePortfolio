// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestEffects/QuestEffect_CompleteQuest.h"

#include "Types/QuestTypes.h"
#include "Components/QuestLogComponent.h"
#include "QuestData/Quest.h"

void UQuestEffect_CompleteQuest::ApplyEffect()
{
	Super::ApplyEffect();
	
	if (!QuestToComplete)
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Quest to complete is not valid!"), *FString(__FUNCTION__));
		return;
	}

	if (!GetQuestLog()->IsQuestActive(QuestToComplete))
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Quest \"%s\" is not in the active list of the quest log!"), *FString(__FUNCTION__), *QuestToComplete->GetName());
		return;
	}

	QuestToComplete->CompleteQuest(CompletionType);
}
