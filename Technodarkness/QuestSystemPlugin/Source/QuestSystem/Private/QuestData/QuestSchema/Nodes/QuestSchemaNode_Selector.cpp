// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Selector.h"

#include "QuestData/QuestSchema/QuestSchema.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransitionWithCheck.h"

UQuestSchemaNode_Selector* UQuestSchemaNode_Selector::CreateNode(UQuestSchema* OwningSchema)
{
	check(OwningSchema);
	UQuestSchemaNode_Selector* Node = NewObject<UQuestSchemaNode_Selector>(OwningSchema);
	return Node;
}

#if WITH_EDITORONLY_DATA
void UQuestSchemaNode_Selector::AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex)
{
	check(TransitionToAdd && TransitionToAdd->IsA<UQuestSchemaTransitionWithCheck>());
	Transitions.Add(StaticCast<UQuestSchemaTransitionWithCheck*>(TransitionToAdd));
	TransitionToAdd->GetNextNode()->ExecutionIndex = Transitions.Num() - 1;
}

void UQuestSchemaNode_Selector::RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex)
{
	Transitions.RemoveAll([ToNode](const UQuestSchemaTransitionWithCheck* Transition)
	{
		return Transition->GetNextNode() == ToNode;
	});
	for (int32 Index = 0; Index < Transitions.Num(); ++Index)
	{
		Transitions[Index]->GetNextNode()->ExecutionIndex = Index;
	}
}

void UQuestSchemaNode_Selector::ClearAllTransitions(int32 PinIndex)
{
	Transitions.Empty();
}

void UQuestSchemaNode_Selector::SortTransitions(int32 PinIndex)
{
	Transitions.Sort([](const UQuestSchemaTransitionWithCheck& A, const UQuestSchemaTransitionWithCheck& B)
	{
		return A.GetNextNode()->ExecutionIndex < B.GetNextNode()->ExecutionIndex;
	});
}
#endif

void UQuestSchemaNode_Selector::EnterNode()
{
	Super::EnterNode();

	if (bNodeWasEntered)
	{
		return;
	}
	
	bNodeWasEntered = true;
	ExitNode();
}

void UQuestSchemaNode_Selector::ExitNode()
{
	Super::ExitNode();

	for (const UQuestSchemaTransitionWithCheck* Transition : Transitions)
	{
		if (Transition && Transition->CanMakeTransition())
		{
			Transition->GetNextNode()->EnterNode();
			return;
		}
	}
}
