// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"

const FName UQuestSchemaEditorTypes::PinCategory_MultipleNodes("MultipleNodes");
const FName UQuestSchemaEditorTypes::PinCategory_MultipleObjectivesOnly("MultipleObjectivesOnly");
const FName UQuestSchemaEditorTypes::PinCategory_MultipleWithCheck("MultipleWithCheck");
const FName UQuestSchemaEditorTypes::PinCategory_MultipleWithWeight("MultipleWithWeight");
const FName UQuestSchemaEditorTypes::PinCategory_SingleNode("SingleNode");
const FName UQuestSchemaEditorTypes::PinCategory_ObjectiveOnAny("ObjectiveOnAny");
const FName UQuestSchemaEditorTypes::PinCategory_ObjectiveOnSuccess("ObjectiveOnSuccess");
const FName UQuestSchemaEditorTypes::PinCategory_ObjectiveOnFail("ObjectiveOnFail");

bool UQuestSchemaEditorTypes::AllowMultipleNodesInPin(const FName& InPinCategory)
{
	return
		InPinCategory == PinCategory_MultipleNodes ||
		InPinCategory == PinCategory_MultipleObjectivesOnly ||
		InPinCategory == PinCategory_MultipleWithCheck ||
		InPinCategory == PinCategory_MultipleWithWeight;
}

bool UQuestSchemaEditorTypes::ShouldCreateTransitionNode(const FName& InPinCategory)
{
	return
		InPinCategory == PinCategory_MultipleWithCheck ||
		InPinCategory == PinCategory_MultipleWithWeight;
}

bool UQuestSchemaEditorTypes::IsNodeOrderImportant(const FName& ForPinCategory)
{
	return ForPinCategory == PinCategory_MultipleWithCheck;
}

int32 UQuestSchemaEditorTypes::GetPinIndexByPinCategory(const FName& InPinCategory)
{
	int32 PinIndex = 0; 
	if (InPinCategory == PinCategory_ObjectiveOnFail)
	{
		PinIndex = 1;
	}
	else if (InPinCategory == PinCategory_ObjectiveOnAny)
	{
		PinIndex = 2;
	}
	return PinIndex;
}
