// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Entry.h"

#include "QuestData/QuestSchema/QuestSchema.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"
#include "QuestLogic/QuestChecks/QuestCheck.h"
#include "Utils/StringUtils.h"

UQuestSchemaNode_Entry* UQuestSchemaNode_Entry::CreateNode(UQuestSchema* OwningSchema)
{
	check(OwningSchema);
	UQuestSchemaNode_Entry* Node = NewObject<UQuestSchemaNode_Entry>(OwningSchema);
	OwningSchema->SetEntryNode(Node);
	return Node;
}

void UQuestSchemaNode_Entry::EnterNode()
{
	Super::EnterNode();

	if (bNodeWasEntered)
	{
		return;
	}
	
	bNodeWasEntered = true;
	ExitNode();
}

#if WITH_EDITORONLY_DATA
FString UQuestSchemaNode_Entry::GetNodeDetails(int32 IndentLevel) const
{
	FString Result(TEXT("Start Requirement:\n"));
	if (StartRequirement)
	{
		Result.Append(FString::Printf(TEXT("%s* %s\n"), *StringUtils::MakeIndent(IndentLevel), *StartRequirement->GetDescription(IndentLevel + 1)));
	}
	else
	{
		Result.Append(FString::Printf(TEXT("%s-\n"), *StringUtils::MakeIndent(IndentLevel)));	
	}
	
	return Result;
}

void UQuestSchemaNode_Entry::AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex)
{
	check(TransitionToAdd);
	Transition = TransitionToAdd;
}

void UQuestSchemaNode_Entry::RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex)
{
	Transition = nullptr;
}

void UQuestSchemaNode_Entry::ClearAllTransitions(int32 PinIndex)
{
	Transition = nullptr;
}
#endif

bool UQuestSchemaNode_Entry::IsStartRequirementChecked(const UQuestLogComponent* InQuestLog) const
{
	return !StartRequirement || StartRequirement->IsChecked(InQuestLog);
}

void UQuestSchemaNode_Entry::ExitNode()
{
	Super::ExitNode();

	if (Transition)
	{
		Transition->GetNextNode()->EnterNode();
	}
}
