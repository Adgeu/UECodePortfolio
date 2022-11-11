// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Effect.h"

#include "QuestData/QuestSchema/QuestSchema.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"
#include "QuestLogic/QuestEffects/QuestEffect.h"
#include "Utils/StringUtils.h"

UQuestSchemaNode_Effect* UQuestSchemaNode_Effect::CreateNode(UQuestSchema* OwningSchema)
{
	check(OwningSchema);
	UQuestSchemaNode_Effect* Node = NewObject<UQuestSchemaNode_Effect>(OwningSchema);
	return Node;
}

void UQuestSchemaNode_Effect::EnterNode()
{
	Super::EnterNode();
	
	if (bNodeWasEntered)
	{
		return;
	}
	
	bNodeWasEntered = true;
	
	for (UQuestEffect* Effect : Effects)
	{
		if (Effect)
		{
			Effect->ApplyEffect();
		}
	}

	ExitNode();
}

#if WITH_EDITORONLY_DATA
FString UQuestSchemaNode_Effect::GetNodeDetails(int32 IndentLevel) const
{
	FString Result(TEXT("Effects:\n"));
	if (Effects.Num() > 0)
	{
		for (const UQuestEffect* Effect : Effects)
		{
			if (Effect)
			{
				Result.Append(FString::Printf(TEXT("%s* %s\n"), *StringUtils::MakeIndent(IndentLevel), *Effect->GetDescription(IndentLevel + 1)));
			}
			else
			{
				Result.Append(FString::Printf(TEXT("%s* NONE\n"), *StringUtils::MakeIndent(IndentLevel)));
			}
		}
	}
	else
	{
		Result.Append(FString::Printf(TEXT("%s-\n"), *StringUtils::MakeIndent(IndentLevel)));	
	}
	
	return Result;
}

void UQuestSchemaNode_Effect::AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex)
{
	check(TransitionToAdd)
	Transition = TransitionToAdd;
}

void UQuestSchemaNode_Effect::RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex)
{
	Transition = nullptr;
}

void UQuestSchemaNode_Effect::ClearAllTransitions(int32 PinIndex)
{
	Transition = nullptr;
}
#endif

void UQuestSchemaNode_Effect::ExitNode()
{
	Super::ExitNode();
	
	if (Transition)
	{
		Transition->GetNextNode()->EnterNode();
	}
}
