// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_WaitForAllFlows.h"

#include "Algo/Count.h"
#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_WaitForAllFlows.h"

void UQuestSchemaGraphNode_WaitForAllFlows::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UQuestSchemaEditorTypes::PinCategory_MultipleNodes, TEXT("In"));
	CreatePin(EGPD_Output, UQuestSchemaEditorTypes::PinCategory_SingleNode, TEXT("Out"));
}

void UQuestSchemaGraphNode_WaitForAllFlows::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);

	if (Pin->Direction == EGPD_Input)
	{
		const int32 FlowsCount = Algo::CountIf(Pin->LinkedTo, [](const UEdGraphPin* ParentPin)
		{
			return ParentPin->GetOwningNode()->IsA<UQuestSchemaGraphNode>();
		});
		
		UQuestSchemaNode_WaitForAllFlows* WaitForAllFlowsNode = CastChecked<UQuestSchemaNode_WaitForAllFlows>(NodeInstance);
		WaitForAllFlowsNode->SetFlowCountToWait(FlowsCount);
	}
}

FName UQuestSchemaGraphNode_WaitForAllFlows::GetNameIcon() const
{
	return FName("QuestIcons.Wait");
}
