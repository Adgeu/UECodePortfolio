// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_ObjectiveXORCompletion.h"

#include "QuestData/QuestObjective.h"
#include "QuestData/QuestSchema/QuestSchema.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Objective.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"

UQuestSchemaNode_ObjectiveXORCompletion* UQuestSchemaNode_ObjectiveXORCompletion::CreateNode(UQuestSchema* OwningSchema)
{
	check(OwningSchema);
	UQuestSchemaNode_ObjectiveXORCompletion* Node = NewObject<UQuestSchemaNode_ObjectiveXORCompletion>(OwningSchema);
	return Node;
}

void UQuestSchemaNode_ObjectiveXORCompletion::EnterNode()
{
	Super::EnterNode();

	if (bNodeWasEntered)
	{
		return;
	}
	
	bNodeWasEntered = true;

	for (const UQuestSchemaTransition* Transition : Transitions)
	{
		check(Transition->GetNextNode()->IsA<UQuestSchemaNode_Objective>());
		const UQuestSchemaNode_Objective* ObjectiveNode = StaticCast<UQuestSchemaNode_Objective*>(Transition->GetNextNode());
		UQuestObjective* Objective = ObjectiveNode->GetObjective();
		const FDelegateHandle ObjectiveDelegate = Objective->OnObjectiveEndedDelegate.AddUObject(this, &UQuestSchemaNode_ObjectiveXORCompletion::OnObjectiveEnded);

		FTrackedObjective TrackedObjective
		{
			Objective,
			ObjectiveDelegate
		};
		TrackedObjectives.Emplace(TrackedObjective);
	}

	ExitNode();
}

#if WITH_EDITORONLY_DATA
void UQuestSchemaNode_ObjectiveXORCompletion::AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex)
{
	check(TransitionToAdd);
	Transitions.Add(TransitionToAdd);
}

void UQuestSchemaNode_ObjectiveXORCompletion::RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex)
{
	Transitions.RemoveAll([ToNode](const UQuestSchemaTransition* Transition)
	{
		return Transition->GetNextNode() == ToNode;
	});
	for (int32 Index = 0; Index < Transitions.Num(); ++Index)
	{
		Transitions[Index]->GetNextNode()->ExecutionIndex = Index;
	}
}

void UQuestSchemaNode_ObjectiveXORCompletion::ClearAllTransitions(int32 PinIndex)
{
	Transitions.Empty();
}
#endif

void UQuestSchemaNode_ObjectiveXORCompletion::ExitNode()
{
	Super::ExitNode();

	for (const UQuestSchemaTransition* Transition : Transitions)
	{
		Transition->GetNextNode()->EnterNode();
	}
}

void UQuestSchemaNode_ObjectiveXORCompletion::OnObjectiveEnded(UQuestObjective* EndedObjective)
{
	for (const FTrackedObjective& TrackedObjective : TrackedObjectives)
	{
		TrackedObjective.Objective->OnObjectiveEndedDelegate.Remove(TrackedObjective.ObjectiveEndedDelegate);
	}
	
	const EQuestCompletionType CompleteOtherWithType = EndedObjective->GetCompletionType() == EQuestCompletionType::Success ? OnSuccessCompleteOtherWithType : OnFailCompleteOtherWithType;
	for (const FTrackedObjective& TrackedObjective : TrackedObjectives)
	{
		if (TrackedObjective.Objective != EndedObjective)
		{
			TrackedObjective.Objective->EndObjective(CompleteOtherWithType);
		}
	}

	TrackedObjectives.Empty();
}
