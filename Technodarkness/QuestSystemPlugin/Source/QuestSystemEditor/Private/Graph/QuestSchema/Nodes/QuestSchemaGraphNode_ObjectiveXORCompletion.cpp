// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_ObjectiveXORCompletion.h"

#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"

#define LOCTEXT_NAMESPACE "QuestSchemaEditor"

void UQuestSchemaGraphNode_ObjectiveXORCompletion::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UQuestSchemaEditorTypes::PinCategory_MultipleNodes, TEXT("In"));
	CreatePin(EGPD_Output, UQuestSchemaEditorTypes::PinCategory_MultipleObjectivesOnly, TEXT("Out"));
}

void UQuestSchemaGraphNode_ObjectiveXORCompletion::GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const
{
	ensure(Pin.GetOwningNode() == this);
	if (Pin.Direction == EGPD_Output)
	{
		if (Pin.PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_MultipleObjectivesOnly)
		{
			HoverTextOut = LOCTEXT("PinHoverMultipleObjectivesOnly","Allows only objectives nodes to connect").ToString();
		}
	}
}

FName UQuestSchemaGraphNode_ObjectiveXORCompletion::GetNameIcon() const
{
	return FName("QuestIcons.ObjectiveXORCompletion");
}

#undef LOCTEXT_NAMESPACE
