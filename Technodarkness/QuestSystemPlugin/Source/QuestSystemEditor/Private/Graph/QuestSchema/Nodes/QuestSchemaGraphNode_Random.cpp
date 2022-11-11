// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Random.h"

#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"

void UQuestSchemaGraphNode_Random::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UQuestSchemaEditorTypes::PinCategory_MultipleNodes, TEXT("In"));
	CreatePin(EGPD_Output, UQuestSchemaEditorTypes::PinCategory_MultipleWithWeight, TEXT("Out"));
}

FName UQuestSchemaGraphNode_Random::GetNameIcon() const
{
	return FName("QuestIcons.Random");
}
