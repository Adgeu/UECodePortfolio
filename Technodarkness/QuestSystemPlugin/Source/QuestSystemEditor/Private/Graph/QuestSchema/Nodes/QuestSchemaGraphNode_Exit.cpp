// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Exit.h"

#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"

void UQuestSchemaGraphNode_Exit::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UQuestSchemaEditorTypes::PinCategory_MultipleNodes, TEXT("In"));
}

FName UQuestSchemaGraphNode_Exit::GetNameIcon() const
{
	return FName("QuestIcons.Exit");
}
