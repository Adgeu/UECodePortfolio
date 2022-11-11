// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Entry.h"

#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"

UQuestSchemaGraphNode_Entry::UQuestSchemaGraphNode_Entry()
{
	bShouldUpdateExecutionOrder = false;
}

void UQuestSchemaGraphNode_Entry::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, UQuestSchemaEditorTypes::PinCategory_SingleNode, TEXT("In"));
}

FName UQuestSchemaGraphNode_Entry::GetNameIcon() const
{
	return FName("QuestIcons.Enter");
}

FText UQuestSchemaGraphNode_Entry::GetTooltipText() const
{
	return UEdGraphNode::GetTooltipText();
}

void UQuestSchemaGraphNode_Entry::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);
}
