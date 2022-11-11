// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Objective.h"

#include "QuestData/Quest.h"
#include "QuestData/QuestObjective.h"
#include "QuestData/QuestSchema/QuestSchema.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"
#include "QuestLogic/QuestTriggers/QuestTrigger.h"
#include "Utils/StringUtils.h"

UQuestSchemaNode_Objective* UQuestSchemaNode_Objective::CreateNode(UQuestSchema* OwningSchema)
{
	check(OwningSchema);
	UQuestSchemaNode_Objective* Node = NewObject<UQuestSchemaNode_Objective>(OwningSchema);
	Node->Objective = NewObject<UQuestObjective>(OwningSchema->GetOwningQuest());
	return Node;
}

#if WITH_EDITORONLY_DATA
FString UQuestSchemaNode_Objective::GetNodeDetails(int32 IndentLevel) const
{
	FString Result(TEXT("Success trigger:\n"));
	if (Objective->GetSuccessTrigger())
	{
		Result.Append(FString::Printf(TEXT("%s* %s\n"), *StringUtils::MakeIndent(IndentLevel), *Objective->GetSuccessTrigger()->GetDescription(IndentLevel + 1)));
	}
	else
	{
		Result.Append(FString::Printf(TEXT("%s-\n"), *StringUtils::MakeIndent(IndentLevel)));	
	}

	Result.Append(TEXT("\nFail trigger:\n"));
	if (Objective->GetFailTrigger())
	{
		Result.Append(FString::Printf(TEXT("%s* %s\n"), *StringUtils::MakeIndent(IndentLevel), *Objective->GetFailTrigger()->GetDescription(IndentLevel + 1)));
	}
	else
	{
		Result.Append(FString::Printf(TEXT("%s-\n"), *StringUtils::MakeIndent(IndentLevel)));	
	}
	
	return Result;
}

void UQuestSchemaNode_Objective::AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex)
{
	check(TransitionToAdd);
	if (PinIndex == 0)
	{
		TransitionOnSuccessCompletion = TransitionToAdd;
	}
	else if (PinIndex == 1)
	{
		TransitionOnFailCompletion = TransitionToAdd;
	}
	else if (PinIndex == 2)
	{
		TransitionOnAnyCompletion = TransitionToAdd;
	}
	else
	{
		UE_LOG(LogQuest, Fatal, TEXT("%s() Invalid pin index: %d"), *FString(__FUNCTION__), PinIndex);
	}
}

void UQuestSchemaNode_Objective::RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex)
{
	ClearAllTransitions(PinIndex);
}

void UQuestSchemaNode_Objective::ClearAllTransitions(int32 PinIndex)
{
	if (PinIndex == 0)
	{
		TransitionOnSuccessCompletion = nullptr;
	}
	else if (PinIndex == 1)
	{
		TransitionOnFailCompletion = nullptr;
	}
	else if (PinIndex == 2)
	{
		TransitionOnAnyCompletion = nullptr;
	}
	else
	{
		UE_LOG(LogQuest, Fatal, TEXT("%s() Invalid pin index: %d"), *FString(__FUNCTION__), PinIndex);
	}
}
#endif

void UQuestSchemaNode_Objective::EnterNode()
{
	Super::EnterNode();

	if (bNodeWasEntered)
	{
		return;
	}
	
	bNodeWasEntered = true;

	Objective->OnObjectiveEndedDelegate.AddUObject(this, &UQuestSchemaNode_Objective::OnObjectiveEnded);
	Objective->StartObjective();
	GetOwningQuest()->OnObjectiveBegins(Objective);
}

void UQuestSchemaNode_Objective::ExitNode()
{
	Super::ExitNode();

	GetOwningQuest()->OnObjectiveEnded(Objective);
	
	if (Objective->GetCompletionType() == EQuestCompletionType::Success)
	{
		if (TransitionOnSuccessCompletion)
		{
			TransitionOnSuccessCompletion->GetNextNode()->EnterNode();
		}
	}
	else if (Objective->GetCompletionType() == EQuestCompletionType::Fail)
	{
		if (TransitionOnFailCompletion)
		{
			TransitionOnFailCompletion->GetNextNode()->EnterNode();
		}
	}

	if (TransitionOnAnyCompletion)
	{
		TransitionOnAnyCompletion->GetNextNode()->EnterNode();
	}
}

void UQuestSchemaNode_Objective::OnObjectiveEnded(UQuestObjective* EndedObjective)
{
	ExitNode();
}
