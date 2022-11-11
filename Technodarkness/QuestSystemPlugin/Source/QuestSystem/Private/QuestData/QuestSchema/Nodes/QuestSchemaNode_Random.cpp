// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Random.h"

#include "QuestData/QuestSchema/QuestSchema.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransitionWithWeight.h"
#include "Utils/ArrayUtils.h"

UQuestSchemaNode_Random* UQuestSchemaNode_Random::CreateNode(UQuestSchema* OwningSchema)
{
	check(OwningSchema);
	UQuestSchemaNode_Random* Node = NewObject<UQuestSchemaNode_Random>(OwningSchema);
	return Node;
}

#if WITH_EDITORONLY_DATA
void UQuestSchemaNode_Random::AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex)
{
	check(TransitionToAdd && TransitionToAdd->IsA<UQuestSchemaTransitionWithWeight>());
	Transitions.Add(StaticCast<UQuestSchemaTransitionWithWeight*>(TransitionToAdd));
}

void UQuestSchemaNode_Random::RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex)
{
	Transitions.RemoveAll([ToNode](const UQuestSchemaTransitionWithWeight* Transition)
	{
		return Transition->GetNextNode() == ToNode;
	});
	for (int32 Index = 0; Index < Transitions.Num(); ++Index)
	{
		Transitions[Index]->GetNextNode()->ExecutionIndex = Index;
	}
}

void UQuestSchemaNode_Random::ClearAllTransitions(int32 PinIndex)
{
	Transitions.Empty();
}
#endif

void UQuestSchemaNode_Random::EnterNode()
{
	Super::EnterNode();

	if (bNodeWasEntered)
	{
		return;
	}
	
	bNodeWasEntered = true;
	ExitNode();
}

void UQuestSchemaNode_Random::ExitNode()
{
	Super::ExitNode();

	TArray<float> Weights;
	ArrayUtils::Select(Transitions, Weights, [](const UQuestSchemaTransitionWithWeight* Transition)
	{
		return Transition->GetWeight();
	});

	const int32 Index = ArrayUtils::GetRandomIndexWithWeight(Weights);
	check(Transitions[Index]);
	Transitions[Index]->GetNextNode()->EnterNode();
}
