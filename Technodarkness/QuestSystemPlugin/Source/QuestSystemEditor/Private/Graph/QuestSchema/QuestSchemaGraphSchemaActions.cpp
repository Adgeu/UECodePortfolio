#include "Graph/QuestSchema/QuestSchemaGraphSchemaActions.h"

#include "EdGraphNode_Comment.h"
#include "SNodePanel.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Effect.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Exit.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Objective.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_ObjectiveXORCompletion.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Random.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Selector.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Sequence.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_WaitForAllFlows.h"
#include "QuestData/QuestSchema/QuestSchema.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Effect.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Exit.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Objective.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_ObjectiveXORCompletion.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Random.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Selector.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Sequence.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_WaitForAllFlows.h"

/////////////////////////////////////////////////////////////////////
// QuestSchemaGraphSchemaActionsUtils

namespace QuestSchemaGraphSchemaActionsUtils
{
	template <typename T>
	T* CreateNode(UEdGraph* ParentGraph, const FVector2D& Location, UQuestSchemaNode_Base* QuestNode)
	{
		T* ResultNode = NewObject<T>(ParentGraph);
		ResultNode->SetFlags(RF_Transactional);
		ResultNode->Rename(nullptr, ParentGraph, REN_NonTransactional);
		ResultNode->CreateNewGuid();
		ResultNode->NodePosX = Location.X;
		ResultNode->NodePosY = Location.Y;
		ResultNode->SnapToGrid(SNodePanel::GetSnapGridSize());
		ResultNode->AllocateDefaultPins();
		ResultNode->NodeInstance = QuestNode;
		
		ParentGraph->Modify();
		ParentGraph->AddNode(ResultNode, true);

		return ResultNode;
	}
}

/////////////////////////////////////////////////////////////////////
// FQuestSchemaGraphSchemaAction_AddComment

UEdGraphNode* FQuestSchemaGraphSchemaAction_AddComment::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode_Comment* const CommentTemplate = NewObject<UEdGraphNode_Comment>();

	FVector2D SpawnLocation = Location;
	FSlateRect Bounds;

	TSharedPtr<SGraphEditor> GraphEditorPtr = SGraphEditor::FindGraphEditorForGraph(ParentGraph);
	if (GraphEditorPtr.IsValid() && GraphEditorPtr->GetBoundsForSelectedNodes(/*out*/ Bounds, 50.0f))
	{
		CommentTemplate->SetBounds(Bounds);
		SpawnLocation.X = CommentTemplate->NodePosX;
		SpawnLocation.Y = CommentTemplate->NodePosY;
	}

	UEdGraphNode* const NewNode = FEdGraphSchemaAction_NewNode::SpawnNodeFromTemplate<UEdGraphNode_Comment>(ParentGraph, CommentTemplate, SpawnLocation, bSelectNewNode);

	return NewNode;
}

/////////////////////////////////////////////////////////////////////
// FQuestSchemaGraphSchemaAction_CreateObjectiveNode

UEdGraphNode* FQuestSchemaGraphSchemaAction_CreateObjectiveNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (!ParentGraph || !QuestSchema)
	{
		return nullptr;
	}

	return QuestSchemaGraphSchemaActionsUtils::CreateNode<UQuestSchemaGraphNode_Objective>(ParentGraph, Location, UQuestSchemaNode_Objective::CreateNode(QuestSchema));
}

/////////////////////////////////////////////////////////////////////
// FQuestSchemaGraphSchemaAction_CreateEffectNode

UEdGraphNode* FQuestSchemaGraphSchemaAction_CreateEffectNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (!ParentGraph || !QuestSchema)
	{
		return nullptr;
	}

	return QuestSchemaGraphSchemaActionsUtils::CreateNode<UQuestSchemaGraphNode_Effect>(ParentGraph, Location, UQuestSchemaNode_Effect::CreateNode(QuestSchema));
}

/////////////////////////////////////////////////////////////////////
// FQuestSchemaGraphSchemaAction_CreateExitNode

UEdGraphNode* FQuestSchemaGraphSchemaAction_CreateExitNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (!ParentGraph || !QuestSchema)
	{
		return nullptr;
	}

	return QuestSchemaGraphSchemaActionsUtils::CreateNode<UQuestSchemaGraphNode_Exit>(ParentGraph, Location, UQuestSchemaNode_Exit::CreateNode(QuestSchema));
}

/////////////////////////////////////////////////////////////////////
// FQuestSchemaGraphSchemaAction_CreateSequenceNode

UEdGraphNode* FQuestSchemaGraphSchemaAction_CreateSequenceNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (!ParentGraph || !QuestSchema)
	{
		return nullptr;
	}

	return QuestSchemaGraphSchemaActionsUtils::CreateNode<UQuestSchemaGraphNode_Sequence>(ParentGraph, Location, UQuestSchemaNode_Sequence::CreateNode(QuestSchema));
}

/////////////////////////////////////////////////////////////////////
// FQuestSchemaGraphSchemaAction_CreateSelectorNode

UEdGraphNode* FQuestSchemaGraphSchemaAction_CreateSelectorNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (!ParentGraph || !QuestSchema)
	{
		return nullptr;
	}

	return QuestSchemaGraphSchemaActionsUtils::CreateNode<UQuestSchemaGraphNode_Selector>(ParentGraph, Location, UQuestSchemaNode_Selector::CreateNode(QuestSchema));
}

/////////////////////////////////////////////////////////////////////
// FQuestSchemaGraphSchemaAction_CreateRandomNode

UEdGraphNode* FQuestSchemaGraphSchemaAction_CreateRandomNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (!ParentGraph || !QuestSchema)
	{
		return nullptr;
	}

	return QuestSchemaGraphSchemaActionsUtils::CreateNode<UQuestSchemaGraphNode_Random>(ParentGraph, Location, UQuestSchemaNode_Random::CreateNode(QuestSchema));
}

/////////////////////////////////////////////////////////////////////
// FQuestSchemaGraphSchemaAction_CreateWaitForAllFlowsNode

UEdGraphNode* FQuestSchemaGraphSchemaAction_CreateWaitForAllFlowsNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (!ParentGraph || !QuestSchema)
	{
		return nullptr;
	}

	return QuestSchemaGraphSchemaActionsUtils::CreateNode<UQuestSchemaGraphNode_WaitForAllFlows>(ParentGraph, Location, UQuestSchemaNode_WaitForAllFlows::CreateNode(QuestSchema));
}

/////////////////////////////////////////////////////////////////////
// FQuestSchemaGraphSchemaAction_CreateObjectiveXORCompletionNode

UEdGraphNode* FQuestSchemaGraphSchemaAction_CreateObjectiveXORCompletionNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (!ParentGraph || !QuestSchema)
	{
		return nullptr;
	}

	return QuestSchemaGraphSchemaActionsUtils::CreateNode<UQuestSchemaGraphNode_ObjectiveXORCompletion>(ParentGraph, Location, UQuestSchemaNode_ObjectiveXORCompletion::CreateNode(QuestSchema));
}
