// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/QuestSchema.h"

#include "QuestData/Quest.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Entry.h"

void UQuestSchema::StartSchemaProcess()
{
	EntryNode->EnterNode();
}

void UQuestSchema::EndSchemaProcess(EQuestCompletionType WithQuestCompletionType)
{
	if (!bSchemaProcessWasEnded)
	{
		bSchemaProcessWasEnded = true;
		OwningQuest->CompleteQuest(WithQuestCompletionType);
	}
}

bool UQuestSchema::CanStartSchema(const UQuestLogComponent* InQuestLog) const
{
	check(EntryNode);
	return EntryNode && EntryNode->IsStartRequirementChecked(InQuestLog);
}
