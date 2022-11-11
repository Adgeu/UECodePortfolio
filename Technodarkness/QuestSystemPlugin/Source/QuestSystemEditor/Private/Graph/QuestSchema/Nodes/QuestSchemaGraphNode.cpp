// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode.h"

#include "Graph/QuestSchema/QuestSchemaGraphSchema.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaTransitionGraphNode.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Base.h"

UQuestSchemaGraphNode::UQuestSchemaGraphNode()
{
	bShouldUpdateExecutionOrder = true;
}

UEdGraphPin* UQuestSchemaGraphNode::GetInputPin(int32 InputIndex) const
{
	check(InputIndex >= 0);

	for (int32 PinIndex = 0, FoundInputs = 0; PinIndex < Pins.Num(); ++PinIndex)
	{
		if (Pins[PinIndex]->Direction == EGPD_Input)
		{
			if (InputIndex == FoundInputs)
			{
				return Pins[PinIndex];
			}
			
			++FoundInputs;
		}
	}

	return nullptr;
}

UEdGraphPin* UQuestSchemaGraphNode::GetOutputPin(int32 InputIndex) const
{
	check(InputIndex >= 0);

	for (int32 PinIndex = 0, FoundInputs = 0; PinIndex < Pins.Num(); ++PinIndex)
	{
		if (Pins[PinIndex]->Direction == EGPD_Output)
		{
			if (InputIndex == FoundInputs)
			{
				return Pins[PinIndex];
			}
			
			++FoundInputs;
		}
	}

	return nullptr;
}

void UQuestSchemaGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin)
	{
		UEdGraphPin* OutputPin = GetOutputPin();

		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
		else if (OutputPin && GetSchema()->TryCreateConnection(OutputPin, FromPin))
		{
			NodeConnectionListChanged();
		}
	}
}

void UQuestSchemaGraphNode::DestroyNode()
{
	for (UEdGraphPin* Pin : Pins)
	{
		// Removing transition nodes if they are
		for (int32 Index = 0; Index < Pin->LinkedTo.Num(); ++Index)
		{
			if (UQuestSchemaTransitionGraphNode* TransitionNode = Cast<UQuestSchemaTransitionGraphNode>(Pin->LinkedTo[Index]->GetOwningNode()))
			{
				TransitionNode->Modify();
				TransitionNode->DestroyNode();
			}
		}

		// Manually breaking links for updating transitions in quest schema nodes.
		GetSchema()->BreakPinLinks(*Pin, false);
	}
	
	Super::DestroyNode();
}

FText UQuestSchemaGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NodeInstance ? FText::FromString(NodeInstance->GetNodeName()) : Super::GetNodeTitle(TitleType);
}

FText UQuestSchemaGraphNode::GetClassName() const
{
	return NodeInstance ? FText::FromString(NodeInstance->GetNiceClassName()) : FText::GetEmpty();
}

FText UQuestSchemaGraphNode::GetDescription() const
{
	return NodeInstance ? FText::FromString(NodeInstance->GetNodeDetails()) : FText::GetEmpty();
}

FName UQuestSchemaGraphNode::GetNameIcon() const
{
	return FName();
}

void UQuestSchemaGraphNode::UpdateExecutionOrder()
{
	if (bShouldUpdateExecutionOrder)
	{
		const UQuestSchemaGraphSchema* QSGraphSchema = CastChecked<UQuestSchemaGraphSchema>(GetSchema());
		for (UEdGraphPin* Pin : GetInputPin()->LinkedTo)
		{
			QSGraphSchema->UpdateExecutionIndexForLinkedNodes(Pin);
		}
	}
}

void UQuestSchemaGraphNode::PostCopyNode()
{
	ResetNodeOwner();
}

void UQuestSchemaGraphNode::ResetNodeOwner()
{
	if (NodeInstance)
	{
		const UEdGraph* MyGraph = GetGraph();
		UObject* GraphOwner = MyGraph ? MyGraph->GetOuter() : nullptr;

		NodeInstance->Rename(nullptr, GraphOwner, REN_DontCreateRedirectors | REN_DoNotDirty);
		NodeInstance->ClearFlags(RF_Transient);
	}
}
