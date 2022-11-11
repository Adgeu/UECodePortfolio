// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestEffects/QuestEffect_StartNewQuests.h"

#include "Components/QuestLogComponent.h"
#include "QuestData/Quest.h"

void UQuestEffect_StartNewQuests::ApplyEffect()
{
	Super::ApplyEffect();

	if (UQuestLogComponent* QuestLogComponent = GetQuestLog())
	{
		for (const UQuest* Quest : QuestsToStart)
		{
			if (Quest)
			{
				UQuest* OutQuest = nullptr;
				QuestLogComponent->AddNewQuest(Quest);
			}
		}
	}
}
