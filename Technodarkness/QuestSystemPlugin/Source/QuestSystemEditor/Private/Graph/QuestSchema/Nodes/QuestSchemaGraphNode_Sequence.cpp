// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Sequence.h"

#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"

void UQuestSchemaGraphNode_Sequence::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UQuestSchemaEditorTypes::PinCategory_MultipleNodes, TEXT("In"));
	CreatePin(EGPD_Output, UQuestSchemaEditorTypes::PinCategory_MultipleWithCheck, TEXT("Out"));
}

FName UQuestSchemaGraphNode_Sequence::GetNameIcon() const
{
	return FName("QuestIcons.Sequence");
}
