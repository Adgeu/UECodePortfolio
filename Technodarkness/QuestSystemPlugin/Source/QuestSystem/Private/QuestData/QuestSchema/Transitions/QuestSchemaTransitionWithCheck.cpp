// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/Transitions/QuestSchemaTransitionWithCheck.h"

#include "QuestData/Quest.h"
#include "QuestLogic/QuestChecks/QuestCheck.h"

bool UQuestSchemaTransitionWithCheck::CanMakeTransition() const
{
	const UQuestSchemaNode_Base* OwnedNode = GetTypedOuter<UQuestSchemaNode_Base>();
	check(OwnedNode);
	const UQuest* Quest = OwnedNode->GetOwningQuest();
	check(Quest);
	return !TransitionCheck || TransitionCheck->IsChecked(Quest->GetQuestLogComponent());
}
