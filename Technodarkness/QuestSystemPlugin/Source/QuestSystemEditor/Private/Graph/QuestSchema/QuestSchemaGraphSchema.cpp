// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/QuestSchema/QuestSchemaGraphSchema.h"

#include "ToolMenu.h"
#include "ToolMenuSection.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"
#include "Graph/QuestSchema/QuestSchemaGraph.h"
#include "Graph/QuestSchema/QuestSchemaGraphSchemaActions.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Entry.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Objective.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaTransitionGraphNode.h"
#include "QuestData/Quest.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Entry.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransitionWithCheck.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransitionWithWeight.h"

////////////////////////////////////////////////////////////////////////////////////
// FCanCreateConnectionBetweenNodes

bool FCanCreateConnectionBetweenNodes::NotSameDirections(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse)
{
	OutResponse = A->Direction == B->Direction
		? FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Same direction"))
		: FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	
	return OutResponse.Response == ECanCreateConnectionResponse::CONNECT_RESPONSE_MAKE;
}

bool FCanCreateConnectionBetweenNodes::NotSameNode(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse)
{
	const UQuestSchemaGraphNode* ABase = Cast<UQuestSchemaGraphNode>(A->GetOwningNode());
	const UQuestSchemaGraphNode* BBase = Cast<UQuestSchemaGraphNode>(B->GetOwningNode());

	OutResponse = ABase->GetUniqueID() == BBase->GetUniqueID()
		? FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Same node"))
		: FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	
	return OutResponse.Response == ECanCreateConnectionResponse::CONNECT_RESPONSE_MAKE;
}

bool FCanCreateConnectionBetweenNodes::CheckForMultipleConnections(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse)
{
	if (!UQuestSchemaEditorTypes::AllowMultipleNodesInPin(A->PinType.PinCategory) && A->LinkedTo.Num() > 0)
	{
		OutResponse = FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Allowed only one linked node"));
		return false;
	}
	if (!UQuestSchemaEditorTypes::AllowMultipleNodesInPin(B->PinType.PinCategory) && B->LinkedTo.Num() > 0)
	{
		OutResponse = FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Allowed only one linked node"));
		return false;
	}
	OutResponse = FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	return true;
}

bool FCanCreateConnectionBetweenNodes::CheckForOnlyObjectivesPin(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse)
{
	if (A->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_MultipleObjectivesOnly)
	{
		if (!B->GetOwningNode()->IsA<UQuestSchemaGraphNode_Objective>())
		{
			OutResponse = FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Allowed only objectives nodes"));
			return false;
		}
	}
	if (B->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_MultipleObjectivesOnly)
	{
		if (!A->GetOwningNode()->IsA<UQuestSchemaGraphNode_Objective>())
		{
			OutResponse = FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Allowed only objectives nodes"));
			return false;
		}
	}
	OutResponse = FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	return true;
}

bool FCanCreateConnectionBetweenNodes::CheckImportantOrderPins(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse)
{
	if (A->Direction == EGPD_Output && UQuestSchemaEditorTypes::IsNodeOrderImportant(A->PinType.PinCategory))
	{
		for (const UEdGraphPin* LinkedToB : B->LinkedTo)
		{
			if (UQuestSchemaEditorTypes::IsNodeOrderImportant(LinkedToB->PinType.PinCategory))
			{
				OutResponse = FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("We cannot connect output pin with important order to node that have already connection with important order"));
				return false;
			}
		}
	}
	if (B->Direction == EGPD_Output && UQuestSchemaEditorTypes::IsNodeOrderImportant(B->PinType.PinCategory))
	{
		for (const UEdGraphPin* LinkedToA : A->LinkedTo)
		{
			if (UQuestSchemaEditorTypes::IsNodeOrderImportant(LinkedToA->PinType.PinCategory))
			{
				OutResponse = FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("We cannot connect output pin with important order to node that have already connection with important order"));
				return false;
			}
		}
	}
	OutResponse = FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	return true;
}

bool FCanCreateConnectionBetweenNodes::CheckForCycling(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse)
{
	// -------------------------------
	// Helper for checking for cycling
	// -------------------------------
	class FNodeVisitorCycleChecker
	{
	public:
		/** Check whether a loop in the graph would be caused by linking the passed-in nodes */
		bool CheckForLoop(UEdGraphNode* StartNode, UEdGraphNode* EndNode)
		{
			VisitedNodes.Add(EndNode);
			return TraverseInputNodesToRoot(StartNode);
		}

	private:
		/** 
		 * Helper function for CheckForLoop()
		 * @param	Node	The node to start traversal at
		 * @return true if we reached a root node (i.e. a node with no input pins), false if we encounter a node we have already seen
		 */
		bool TraverseInputNodesToRoot(UEdGraphNode* Node)
		{
			VisitedNodes.Add(Node);

			// Follow every input pin until we cant any more ('root') or we reach a node we have seen (cycle)
			for (int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
			{
				UEdGraphPin* MyPin = Node->Pins[PinIndex];

				if (MyPin->Direction == EGPD_Input)
				{
					for (int32 LinkedPinIndex = 0; LinkedPinIndex < MyPin->LinkedTo.Num(); ++LinkedPinIndex)
					{
						const UEdGraphPin* OtherPin = MyPin->LinkedTo[LinkedPinIndex];
						if(OtherPin)
						{
							UEdGraphNode* OtherNode = OtherPin->GetOwningNode();
							if (VisitedNodes.Contains(OtherNode))
							{
								return false;
							}
							return TraverseInputNodesToRoot(OtherNode);
						}
					}
				}
			}
			return true;
		}

		TSet<UEdGraphNode*> VisitedNodes;
	};
	// ----------------------------
	
	// Check for cycles
	FNodeVisitorCycleChecker CycleChecker;
	OutResponse = !CycleChecker.CheckForLoop(A->GetOwningNode(), B->GetOwningNode())
		? FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cycle graph"))
		: FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	
	return OutResponse.Response == ECanCreateConnectionResponse::CONNECT_RESPONSE_MAKE;
}

////////////////////////////////////////////////////////////////////////////////////
// UQuestSchemaGraphSchema

#define LOCTEXT_NAMESPACE "QuestGraphSchema"

int32 UQuestSchemaGraphSchema::CurrentCacheRefreshID = 0;

void UQuestSchemaGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	const UQuest* Quest = CastChecked<UQuest>(Graph.GetOuter());
	
	FGraphNodeCreator<UQuestSchemaGraphNode_Entry> NodeCreator(Graph);
	UQuestSchemaGraphNode_Entry* MyNode = NodeCreator.CreateNode();
	MyNode->NodeInstance = UQuestSchemaNode_Entry::CreateNode(Quest->GetQuestSchema());
	NodeCreator.Finalize();
	SetNodeMetaData(MyNode, FNodeMetadata::DefaultGraphNode);
}

const FPinConnectionResponse UQuestSchemaGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	return FCanCreateConnectionBetweenNodes::Check(A, B);
}

bool UQuestSchemaGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	return Super::CreateAutomaticConversionNodeAndConnections(A, B);
}

void UQuestSchemaGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	UQuest* Quest = CastChecked<UQuest>(ContextMenuBuilder.CurrentGraph->GetOuter());
	UQuestSchema* QuestSchema = Quest->GetQuestSchema();

	const FText CategoryEmpty = FText::GetEmpty();
	const FText CategoryFlow = LOCTEXT("QuestSchemaFlowNodes", "Flow Nodes");
	const FText CategoryData = LOCTEXT("QuestSchemaDataNodes", "Data Nodes");

	const FText CommentMenuDesc = LOCTEXT("CommentMenuDesc", "Add Comment...");
	const FText CommentToolTip = LOCTEXT("CommentToolTip", "Adding a comment box");
	const TSharedPtr<FQuestSchemaGraphSchemaAction_AddComment> NewCommentNode(new FQuestSchemaGraphSchemaAction_AddComment(CommentMenuDesc, CommentToolTip));
	ContextMenuBuilder.AddAction(NewCommentNode);

	const FText ObjectiveMenuDesc = LOCTEXT("ObjectiveMenuDesc", "Objective");
	const FText ObjectiveToolTip = LOCTEXT("ObjectiveToolTip", "An Objective Node");
	const TSharedPtr<FQuestSchemaGraphSchemaAction_CreateObjectiveNode> NewObjectiveNode(new FQuestSchemaGraphSchemaAction_CreateObjectiveNode(CategoryData, ObjectiveMenuDesc, ObjectiveToolTip, 0, QuestSchema));
	ContextMenuBuilder.AddAction(NewObjectiveNode);

	const FText EffectMenuDesc = LOCTEXT("EffectMenuDesc", "Effect");
	const FText EffectToolTip = LOCTEXT("EffectToolTip", "An Effect Node");
	const TSharedPtr<FQuestSchemaGraphSchemaAction_CreateEffectNode> NewEffectNode(new FQuestSchemaGraphSchemaAction_CreateEffectNode(CategoryData, EffectMenuDesc, EffectToolTip, 0, QuestSchema));
	ContextMenuBuilder.AddAction(NewEffectNode);

	const FText ExitMenuDesc = LOCTEXT("ExitMenuDesc", "Exit");
	const FText ExitToolTip = LOCTEXT("ExitToolTip", "An Exit Node");
	const TSharedPtr<FQuestSchemaGraphSchemaAction_CreateExitNode> NewExitNode(new FQuestSchemaGraphSchemaAction_CreateExitNode(CategoryFlow, ExitMenuDesc, ExitToolTip, 0, QuestSchema));
	ContextMenuBuilder.AddAction(NewExitNode);

	const FText SequenceMenuDesc = LOCTEXT("SequenceMenuDesc", "Sequence");
	const FText SequenceToolTip = LOCTEXT("SequenceToolTip", "A Sequence Node");
	const TSharedPtr<FQuestSchemaGraphSchemaAction_CreateSequenceNode> NewSequenceNode(new FQuestSchemaGraphSchemaAction_CreateSequenceNode(CategoryFlow, SequenceMenuDesc, SequenceToolTip, 0, QuestSchema));
	ContextMenuBuilder.AddAction(NewSequenceNode);

	const FText SelectorMenuDesc = LOCTEXT("SelectorMenuDesc", "Selector");
	const FText SelectorToolTip = LOCTEXT("SelectorToolTip", "A Selector Node");
	const TSharedPtr<FQuestSchemaGraphSchemaAction_CreateSelectorNode> NewSelectorNode(new FQuestSchemaGraphSchemaAction_CreateSelectorNode(CategoryFlow, SelectorMenuDesc, SelectorToolTip, 0, QuestSchema));
	ContextMenuBuilder.AddAction(NewSelectorNode);

	const FText RandomMenuDesc = LOCTEXT("RandomMenuDesc", "Random");
	const FText RandomToolTip = LOCTEXT("RandomToolTip", "A Random Node");
	const TSharedPtr<FQuestSchemaGraphSchemaAction_CreateRandomNode> NewRandomNode(new FQuestSchemaGraphSchemaAction_CreateRandomNode(CategoryFlow, RandomMenuDesc, RandomToolTip, 0, QuestSchema));
	ContextMenuBuilder.AddAction(NewRandomNode);

	const FText WaitForAllFlowsMenuDesc = LOCTEXT("WaitForAllFlowsMenuDesc", "Wait For All Flows");
	const FText WaitForAllFlowsToolTip = LOCTEXT("WaitForAllFlowsToolTip", "A Wait For All Flows Node");
	const TSharedPtr<FQuestSchemaGraphSchemaAction_CreateWaitForAllFlowsNode> NewWaitForAllFlowsNode(new FQuestSchemaGraphSchemaAction_CreateWaitForAllFlowsNode(CategoryFlow, WaitForAllFlowsMenuDesc, WaitForAllFlowsToolTip, 0, QuestSchema));
	ContextMenuBuilder.AddAction(NewWaitForAllFlowsNode);

	const FText ObjectiveXORCompletionMenuDesc = LOCTEXT("ObjectiveXORCompletionMenuDesc", "Objective XOR Completion");
	const FText ObjectiveXORCompletionToolTip = LOCTEXT("ObjectiveXORCompletionToolTip", "An Objective XOR Completion Node");
	const TSharedPtr<FQuestSchemaGraphSchemaAction_CreateObjectiveXORCompletionNode> NewObjectiveXORCompletionNode(new FQuestSchemaGraphSchemaAction_CreateObjectiveXORCompletionNode(CategoryFlow, ObjectiveXORCompletionMenuDesc, ObjectiveXORCompletionToolTip, 0, QuestSchema));
	ContextMenuBuilder.AddAction(NewObjectiveXORCompletionNode);
}

void UQuestSchemaGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	check(Context && Context->Graph);
	if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("QuestSchemaNodeActions", LOCTEXT("NodeActionsMenuHeader", "Node Actions"));
		if (!Context->bIsDebugging)
		{
			Section.AddMenuEntry(FGenericCommands::Get().Delete);
			Section.AddMenuEntry(FGenericCommands::Get().Cut);
			Section.AddMenuEntry(FGenericCommands::Get().Copy);
			Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
		}
	}
	
	Super::GetContextMenuActions(Menu, Context);
}

bool UQuestSchemaGraphSchema::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return CurrentCacheRefreshID != InVisualizationCacheID;
}

int32 UQuestSchemaGraphSchema::GetCurrentVisualizationCacheID() const
{
	return CurrentCacheRefreshID;
}

void UQuestSchemaGraphSchema::ForceVisualizationCacheClear() const
{
	++CurrentCacheRefreshID;
}

TSharedPtr<FEdGraphSchemaAction> UQuestSchemaGraphSchema::GetCreateCommentAction() const
{
	return TSharedPtr<FEdGraphSchemaAction>(static_cast<FEdGraphSchemaAction*>(new FQuestSchemaGraphSchemaAction_AddComment));
}

bool UQuestSchemaGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	const bool bResult = Super::TryCreateConnection(A, B);
	
	if (bResult)
	{
		UEdGraphPin* OutputPin = A->Direction == EGPD_Output ? A : B;
		UEdGraphPin* InputPin = A->Direction == EGPD_Input ? A : B;

		const UQuestSchemaGraphNode* SourceNode = Cast<UQuestSchemaGraphNode>(OutputPin->GetOwningNode());
		const UQuestSchemaGraphNode* ToNode = Cast<UQuestSchemaGraphNode>(InputPin->GetOwningNode());

		if (SourceNode && ToNode)
		{
			UQuestSchemaNode_Base* SourceQuestSchemaNode = SourceNode->NodeInstance;
			UQuestSchemaTransition* Transition = nullptr;
			
			if (UQuestSchemaEditorTypes::ShouldCreateTransitionNode(OutputPin->PinType.PinCategory))
			{
				// Adding transition node
				if (OutputPin->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_MultipleWithCheck)
				{
					Transition = NewObject<UQuestSchemaTransitionWithCheck>(SourceQuestSchemaNode);
				}
				else if (OutputPin->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_MultipleWithWeight)
				{
					Transition = NewObject<UQuestSchemaTransitionWithWeight>(SourceQuestSchemaNode);
				}

				UEdGraph* Graph = SourceNode->GetGraph();

				UQuestSchemaTransitionGraphNode* TransitionGraphNode = NewObject<UQuestSchemaTransitionGraphNode>(Graph);
				TransitionGraphNode->AllocateDefaultPins();
				TransitionGraphNode->CreateConnections(OutputPin, InputPin);
				TransitionGraphNode->Transition = Transition;
				TransitionGraphNode->SetFlags(RF_Transactional);
				TransitionGraphNode->Rename(nullptr, Graph, REN_NonTransactional);
				TransitionGraphNode->CreateNewGuid();

				Graph->Modify();
				Graph->AddNode(TransitionGraphNode, false, true);
			}
			else
			{
				Transition = NewObject<UQuestSchemaTransition>(SourceQuestSchemaNode);
			}
			Transition->InitTransition(ToNode->NodeInstance);

			const int32 PinIndex = UQuestSchemaEditorTypes::GetPinIndexByPinCategory(OutputPin->PinType.PinCategory);
			SourceQuestSchemaNode->AddTransition(Transition, PinIndex);
			UpdateExecutionIndexForLinkedNodes(OutputPin);
		}
	}
	
	return bResult;
}

void UQuestSchemaGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
	const UQuestSchemaGraphNode* TargetNode = CastChecked<UQuestSchemaGraphNode>(TargetPin.GetOwningNode());

	if (TargetPin.Direction == EGPD_Output)
	{
		TargetNode->NodeInstance->ClearAllTransitions(UQuestSchemaEditorTypes::GetPinIndexByPinCategory(TargetPin.PinType.PinCategory));
	}
	else
	{
		for (UEdGraphPin* LinkedPin : TargetPin.LinkedTo)
		{
			if (const UQuestSchemaGraphNode* LinkedNode = Cast<UQuestSchemaGraphNode>(LinkedPin->GetOwningNode()))
			{
				const int32 PinIndex = UQuestSchemaEditorTypes::GetPinIndexByPinCategory(TargetPin.PinType.PinCategory); 
				LinkedNode->NodeInstance->RemoveTransition(TargetNode->NodeInstance, PinIndex);
			}

			UpdateExecutionIndexForLinkedNodes(LinkedPin);
		}
	}
	
	Super::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}

void UQuestSchemaGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	const UQuestSchemaGraphNode* SourceNode = CastChecked<UQuestSchemaGraphNode>(SourcePin->GetOwningNode());
	const UQuestSchemaGraphNode* TargetNode = CastChecked<UQuestSchemaGraphNode>(TargetPin->GetOwningNode());

	const int32 PinIndex = UQuestSchemaEditorTypes::GetPinIndexByPinCategory(TargetPin->PinType.PinCategory); 
	SourceNode->NodeInstance->RemoveTransition(TargetNode->NodeInstance, PinIndex);

	UpdateExecutionIndexForLinkedNodes(SourcePin);

	Super::BreakSinglePinLink(SourcePin, TargetPin);
}

void UQuestSchemaGraphSchema::UpdateExecutionIndexForLinkedNodes(UEdGraphPin* InPin) const
{
	if (!UQuestSchemaEditorTypes::IsNodeOrderImportant(InPin->PinType.PinCategory))
	{
		return;
	}

	InPin->LinkedTo.Sort(FCompareNodeXLocation());

	int32 ExecutionIndex = 0;
	for (const UEdGraphPin* LinkedPin : InPin->LinkedTo)
	{
		if (const UQuestSchemaGraphNode* Child = Cast<UQuestSchemaGraphNode>(LinkedPin->GetOwningNode()))
		{
			Child->NodeInstance->ExecutionIndex = ExecutionIndex;
			++ExecutionIndex;
		}
	}

	CastChecked<UQuestSchemaGraphNode>(InPin->GetOwningNode())->NodeInstance->SortTransitions();
}

#undef LOCTEXT_NAMESPACE
