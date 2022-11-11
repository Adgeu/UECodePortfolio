// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_WaitForAllFlows.h"

#include "QuestData/QuestSchema/QuestSchema.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"

UQuestSchemaNode_WaitForAllFlows* UQuestSchemaNode_WaitForAllFlows::CreateNode(UQuestSchema* OwningSchema)
{
	check(OwningSchema);
	UQuestSchemaNode_WaitForAllFlows* Node = NewObject<UQuestSchemaNode_WaitForAllFlows>(OwningSchema);
	return Node;
}

void UQuestSchemaNode_WaitForAllFlows::EnterNode()
{
	Super::EnterNode();

	bNodeWasEntered = true;
	if (--FlowCountToWait <= 0)
	{
		ExitNode();
	}
}

#if WITH_EDITORONLY_DATA
void UQuestSchemaNode_WaitForAllFlows::AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex)
{
	check(TransitionToAdd)
	Transition = TransitionToAdd;
}

void UQuestSchemaNode_WaitForAllFlows::RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex)
{
	Transition = nullptr;
}

void UQuestSchemaNode_WaitForAllFlows::ClearAllTransitions(int32 PinIndex)
{
	Transition = nullptr;
}
#endif

void UQuestSchemaNode_WaitForAllFlows::ExitNode()
{
	Super::ExitNode();

	if (Transition)
	{
		Transition->GetNextNode()->EnterNode();
	}
}
