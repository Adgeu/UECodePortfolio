// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"

#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Base.h"

void UQuestSchemaTransition::InitTransition(UQuestSchemaNode_Base* ToNode)
{
	NextNode = ToNode;
}
