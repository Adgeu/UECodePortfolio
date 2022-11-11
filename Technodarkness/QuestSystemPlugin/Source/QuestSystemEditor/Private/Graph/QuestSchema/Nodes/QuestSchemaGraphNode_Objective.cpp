// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Objective.h"

#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"

#define LOCTEXT_NAMESPACE "QuestSchemaEditor"

void UQuestSchemaGraphNode_Objective::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UQuestSchemaEditorTypes::PinCategory_MultipleNodes, TEXT("In"));

	CreatePin(EGPD_Output, UQuestSchemaEditorTypes::PinCategory_ObjectiveOnSuccess, TEXT("OutOnSuccess"));
	CreatePin(EGPD_Output, UQuestSchemaEditorTypes::PinCategory_ObjectiveOnFail, TEXT("OutOnFail"));
	CreatePin(EGPD_Output, UQuestSchemaEditorTypes::PinCategory_ObjectiveOnAny, TEXT("OutOnAny"));
}

void UQuestSchemaGraphNode_Objective::GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const
{
	ensure(Pin.GetOwningNode() == this);
	if (Pin.Direction == EGPD_Output)
	{
		if (Pin.PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_ObjectiveOnAny)
		{
			HoverTextOut = LOCTEXT("PinHoverObjectiveOnAny","Running on any objective completion").ToString();
		}
		else if (Pin.PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_ObjectiveOnSuccess)
		{
			HoverTextOut = LOCTEXT("PinHoverObjectiveOnSuccess","Running only on success objective completion").ToString();
		}
		else if (Pin.PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_ObjectiveOnFail)
		{
			HoverTextOut = LOCTEXT("PinHoverObjectiveOnFail","Running only on fail objective completion").ToString();
		}
	}
}

FName UQuestSchemaGraphNode_Objective::GetNameIcon() const
{
	return FName("QuestIcons.Objective");
}

#undef LOCTEXT_NAMESPACE
