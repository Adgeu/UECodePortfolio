// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Effect.h"

#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"

void UQuestSchemaGraphNode_Effect::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UQuestSchemaEditorTypes::PinCategory_MultipleNodes, TEXT("In"));
	CreatePin(EGPD_Output, UQuestSchemaEditorTypes::PinCategory_SingleNode, TEXT("Out"));
}

FName UQuestSchemaGraphNode_Effect::GetNameIcon() const
{
	return FName("QuestIcons.Effect");
}
