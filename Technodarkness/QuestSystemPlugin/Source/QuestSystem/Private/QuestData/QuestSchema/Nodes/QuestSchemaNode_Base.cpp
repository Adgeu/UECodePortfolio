// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Base.h"

#include "QuestData/Quest.h"
#include "QuestData/QuestSchema/QuestSchema.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransitionWithCheck.h"

UQuestSchemaNode_Base::UQuestSchemaNode_Base()
{
#if WITH_EDITORONLY_DATA
	NodeName = GetNiceClassName();
#endif
}

void UQuestSchemaNode_Base::EnterNode()
{
	if (!bNodeWasEntered)
	{
		UE_LOG(LogQuest, Display, TEXT("Entering the node\"%s\" in quest \"%s\""), *GetName(), *GetOwningQuest()->GetName());
	}
	else
	{
		UE_LOG(LogQuest, Warning, TEXT("Entering the node\"%s\" in quest \"%s\", but it was already entered"), *GetName(), *GetOwningQuest()->GetName());
	}
}

#if WITH_EDITORONLY_DATA
void UQuestSchemaNode_Base::AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex)
{
	// pass
}

void UQuestSchemaNode_Base::RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex)
{
	// pass
}

void UQuestSchemaNode_Base::SortTransitions(int32 PinIndex)
{
	// pass
}

void UQuestSchemaNode_Base::ClearAllTransitions(int32 PinIndex)
{
	// pass
}

FString UQuestSchemaNode_Base::GetNodeName() const
{
	return NodeName.Len() > 0 ? NodeName : GetNiceClassName();
}

FString UQuestSchemaNode_Base::GetNiceClassName() const
{
	FString ClassName = GetClass()->GetName();
	ClassName = ClassName.Replace(TEXT("QuestSchemaNode_"), TEXT(""));
	ClassName = ClassName.Replace(TEXT("_C"), TEXT(""));
	return ClassName;
}

FString UQuestSchemaNode_Base::GetNodeDetails(int32 IndentLevel) const
{
	return FString();
}
#endif

void UQuestSchemaNode_Base::ExitNode()
{
	UE_LOG(LogQuest, Display, TEXT("Exiting the node\"%s\" in the quest \"%s\""), *GetName(), *GetOwningQuest()->GetName());
}

UQuestSchema* UQuestSchemaNode_Base::GetOwningSchema() const
{
	return StaticCast<UQuestSchema*>(GetOuter());
}

UQuest* UQuestSchemaNode_Base::GetOwningQuest() const
{
	return GetOwningSchema()->GetOwningQuest();
}
