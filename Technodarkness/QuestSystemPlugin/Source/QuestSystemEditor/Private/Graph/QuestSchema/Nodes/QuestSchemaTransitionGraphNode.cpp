// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/Nodes/QuestSchemaTransitionGraphNode.h"

#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"

#define LOCTEXT_NAMESPACE "QuestGraphSchema"

void UQuestSchemaTransitionGraphNode::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Transition"), TEXT("In"));
	Inputs->bHidden = true;
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Out"));
	Outputs->bHidden = true;
}

FText UQuestSchemaTransitionGraphNode::GetTooltipText() const
{
	return LOCTEXT("QuestSchemaTransitionTooltip", "This is a quest schema transition with additional properties");
}

void UQuestSchemaTransitionGraphNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.Num() == 0)
	{
		// Commit suicide; transitions must always have an input and output connection
		Modify();

		// Our parent graph will have our graph in SubGraphs so needs to be modified to record that.
		if(UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}

void UQuestSchemaTransitionGraphNode::DestroyNode()
{
	const UQuestSchemaGraphNode* PreviousNode = GetPreviousNode();
	const UQuestSchemaGraphNode* NextNode = GetNextNode();

	if (PreviousNode && NextNode)
	{
		for (UEdGraphPin* NextNodePin : NextNode->Pins)
		{
			if (NextNodePin->Direction == EGPD_Input)
			{
				for (int32 Index = 0; Index < NextNodePin->LinkedTo.Num(); ++Index)
				{
					if (NextNodePin->LinkedTo[Index]->GetOwningNode() == PreviousNode)
					{
						GetSchema()->BreakSinglePinLink(NextNodePin->LinkedTo[Index], NextNodePin);
					}
				}
			}
		}
	}
	
	Super::DestroyNode();
}

UQuestSchemaGraphNode* UQuestSchemaTransitionGraphNode::GetPreviousNode() const
{
	if (Pins[0]->LinkedTo.Num() > 0)
	{
		return Cast<UQuestSchemaGraphNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
	}
	return nullptr;
}

UQuestSchemaGraphNode* UQuestSchemaTransitionGraphNode::GetNextNode() const
{
	if (Pins[1]->LinkedTo.Num() > 0)
	{
		return Cast<UQuestSchemaGraphNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
	}
	return nullptr;
}

void UQuestSchemaTransitionGraphNode::CreateConnections(UEdGraphPin* PreviousPin, UEdGraphPin* NextPin)
{
	// Previous to this
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();
	PreviousPin->Modify();
	Pins[0]->MakeLinkTo(PreviousPin);
	
	// This to next
	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();
	NextPin->Modify();
	Pins[1]->MakeLinkTo(NextPin);
}

#undef LOCTEXT_NAMESPACE
