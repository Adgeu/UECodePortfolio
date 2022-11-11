// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Sequence.h"

#include "QuestData/QuestSchema/QuestSchema.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransitionWithCheck.h"

UQuestSchemaNode_Sequence* UQuestSchemaNode_Sequence::CreateNode(UQuestSchema* OwningSchema)
{
	check(OwningSchema);
	UQuestSchemaNode_Sequence* Node = NewObject<UQuestSchemaNode_Sequence>(OwningSchema);
	return Node;
}

#if WITH_EDITORONLY_DATA
void UQuestSchemaNode_Sequence::AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex)
{
	check(TransitionToAdd && TransitionToAdd->IsA<UQuestSchemaTransitionWithCheck>());
	Transitions.Add(StaticCast<UQuestSchemaTransitionWithCheck*>(TransitionToAdd));
}

void UQuestSchemaNode_Sequence::RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex)
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

void UQuestSchemaNode_Sequence::ClearAllTransitions(int32 PinIndex)
{
	Transitions.Empty();
}

void UQuestSchemaNode_Sequence::SortTransitions(int32 PinIndex)
{
	Transitions.Sort([](const UQuestSchemaTransitionWithCheck& A, const UQuestSchemaTransitionWithCheck& B)
	{
		return A.GetNextNode()->ExecutionIndex < B.GetNextNode()->ExecutionIndex;
	});
}
#endif

void UQuestSchemaNode_Sequence::EnterNode()
{
	Super::EnterNode();

	if (bNodeWasEntered)
	{
		return;
	}
	
	bNodeWasEntered = true;
	ExitNode();
}

void UQuestSchemaNode_Sequence::ExitNode()
{
	Super::ExitNode();

	for (const UQuestSchemaTransitionWithCheck* Transition : Transitions)
	{
		if (Transition && Transition->CanMakeTransition())
		{
			Transition->GetNextNode()->EnterNode();
		}
	}
}
