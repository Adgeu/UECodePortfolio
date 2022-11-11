#include "Graph/QuestSchema/QuestSchemaGraphNodeFactory.h"

#include "Graph/QuestSchema/QuestSchemaGraphConnectionDrawingPolicy.h"
#include "Graph/QuestSchema/QuestSchemaGraphSchema.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaTransitionGraphNode.h"
#include "Graph/QuestSchema/Nodes/Slates/SGraphNode_QuestSchema.h"
#include "Graph/QuestSchema/Nodes/Slates/SGraphNode_QuestSchemaTransition.h"
#include "KismetPins/SGraphPinExec.h"

TSharedPtr<SGraphNode> FQuestSchemaGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UQuestSchemaGraphNode* QuestSchemaGraphNode = Cast<UQuestSchemaGraphNode>(Node))
	{
		return SNew(SGraphNode_QuestSchema, QuestSchemaGraphNode);
	}
	if (UQuestSchemaTransitionGraphNode* QuestSchemaTransitionGraphNode = Cast<UQuestSchemaTransitionGraphNode>(Node))
	{
		return SNew(SGraphNode_QuestSchemaTransition, QuestSchemaTransitionGraphNode);
	}
	
	return nullptr;
}

TSharedPtr<SGraphPin> FQuestSchemaGraphPinFactory::CreatePin(UEdGraphPin* Pin) const
{
	if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
	{
		return SNew(SGraphPinExec, Pin);
	}

	return nullptr;
}

FConnectionDrawingPolicy* FQuestSchemaGraphPinConnectionFactory::CreateConnectionPolicy(const UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const
{
	if (Schema->IsA<UQuestSchemaGraphSchema>())
	{
		return new FQuestSchemaGraphConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}

	return nullptr;
}
