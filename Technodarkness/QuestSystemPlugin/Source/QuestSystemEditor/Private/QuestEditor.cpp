#include "QuestEditor.h"

#include "EdGraphNode_Comment.h"
#include "EdGraphUtilities.h"
#include "GraphEditorActions.h"
#include "QuestEditorApplicationModes.h"
#include "SNodePanel.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/QuestSchema/QuestSchemaGraph.h"
#include "Graph/QuestSchema/QuestSchemaGraphSchema.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaTransitionGraphNode.h"
#include "QuestData/Quest.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Objective.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"

#define LOCTEXT_NAMESPACE "QuestEditor"

FQuestEditor::FQuestEditor()
{
}

FQuestEditor::~FQuestEditor()
{
}

void FQuestEditor::InitQuestEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UQuest* InQuest)
{
	Quest = InQuest;

	// Initialize the asset editor and spawn nothing (dummy layout)
	const TSharedRef<FTabManager::FLayout> DummyLayout = FTabManager::NewLayout("NullLayout")->AddArea(FTabManager::NewPrimaryArea());
	InitAssetEditor(Mode, InitToolkitHost, FName("QuestEditorApp"), DummyLayout, true, true, InQuest);

	//Init DocumentManager
	DocumentManager = MakeShareable(new FDocumentTracker);
	DocumentManager->Initialize(SharedThis(this));
	DocumentManager->SetTabManager(TabManager.ToSharedRef());

	// Init Mode
	AddApplicationMode(FQuestEditorApplicationModesStatics::QuestSchemaMode, MakeShareable(new FQuestSchemaEditorApplicationMode(SharedThis(this))));
	SetCurrentMode(FQuestEditorApplicationModesStatics::QuestSchemaMode);
}

void FQuestEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

FName FQuestEditor::GetToolkitFName() const
{
	return FName("QuestEditor");
}

FText FQuestEditor::GetBaseToolkitName() const
{
	return LOCTEXT("QuestEditor", "Quest Editor");
}

FString FQuestEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "Quest ").ToString();
}

FLinearColor FQuestEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

void FQuestEditor::RegisterToolbarTab(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FQuestEditor::InvokeQuestGraphTab()
{
	bool bNewGraph = false;
	if (!Quest->QuestGraph)
	{
		bNewGraph = true;
		UQuestSchemaGraph* QuestGraph = CastChecked<UQuestSchemaGraph>(FBlueprintEditorUtils::CreateNewGraph(Quest.Get(), NAME_None, UQuestSchemaGraph::StaticClass(), UQuestSchemaGraphSchema::StaticClass()));
		Quest->QuestGraph = QuestGraph;

		// Initialize the quest schema graph
		const UEdGraphSchema* Schema = QuestGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*QuestGraph);
	}
	
	TSharedRef<FTabPayload_UObject> Payload = FTabPayload_UObject::Make(Quest->QuestGraph);
	TSharedPtr<SDockTab> DocumentTab = DocumentManager->OpenDocument(Payload, bNewGraph ? FDocumentTracker::OpenNewDocument : FDocumentTracker::RestorePreviousDocument);
}

void FQuestEditor::DeleteSelectedNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
		{
			if (Node->CanUserDeleteNode())
			{
				Node->Modify();
				Node->DestroyNode();
			}
		}
	}
}

void FQuestEditor::DeleteSelectedDuplicatableNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	// Delete the duplicatable nodes
	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

bool FQuestEditor::CanDeleteNodes() const
{
	// If any of the nodes can be deleted then we should allow deleting
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		const UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}

void FQuestEditor::OnCreateComment()
{
	const TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (UEdGraph* EdGraph = CurrentGraphEditor.IsValid() ? CurrentGraphEditor->GetCurrentGraph() : nullptr)
	{
		TSharedPtr<FEdGraphSchemaAction> Action = EdGraph->GetSchema()->GetCreateCommentAction();
		if (Action.IsValid())
		{
			Action->PerformAction(EdGraph, nullptr, FVector2D());
		}
	}
}

bool FQuestEditor::CanCreateComment() const
{
	return true;
}

void FQuestEditor::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	FString ExportedText;
	
	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (!Node)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UQuestSchemaGraphNode* Node = Cast<UQuestSchemaGraphNode>(*SelectedIter))
		{
			Node->PostCopyNode();
		}
	}
}

bool FQuestEditor::CanCopyNodes() const
{
	// If any of the nodes can be duplicated then we should allow copying
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		const UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FQuestEditor::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedDuplicatableNodes();
}

bool FQuestEditor::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FQuestEditor::PasteNodes()
{
	if (const TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin())
	{
		PasteNodesHere(CurrentGraphEditor->GetPasteLocation());
	}
}

void FQuestEditor::PasteNodesHere(const FVector2D& Location)
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	// Undo/Redo support
	const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();
	// UAIGraph* AIGraph = Cast<UAIGraph>(EdGraph);

	EdGraph->Modify();

	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	// Clear the selection set (newly pasted stuff will be selected)
	CurrentGraphEditor->ClearSelectionSet();
 
	// Grab the text to paste from the clipboard.
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	// Import the nodes
	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, /*out*/ PastedNodes);

	//Average position of nodes so we can move them while still maintaining relative distances to each other
	FVector2D AvgNodePosition(0.0f, 0.0f);
	
	// Number of nodes used to calculate AvgNodePosition
	int32 AvgCount = 0;

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		if (const UEdGraphNode* EdNode = *It)
		{
			AvgNodePosition.X += EdNode->NodePosX;
			AvgNodePosition.Y += EdNode->NodePosY;
			++AvgCount;
		}
	}

	if (AvgCount > 0)
	{
		const float InvNumNodes = 1.0f / static_cast<float>(AvgCount);
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		if (UEdGraphNode* PasteNode = *It)
		{
			// Select the newly pasted stuff
			CurrentGraphEditor->SetNodeSelection(PasteNode, true);

			PasteNode->NodePosX = PasteNode->NodePosX - AvgNodePosition.X + Location.X;
			PasteNode->NodePosY = PasteNode->NodePosY - AvgNodePosition.Y + Location.Y;

			PasteNode->SnapToGrid(SNodePanel::GetSnapGridSize());
			
			// Give new node a different Guid from the old one
			PasteNode->CreateNewGuid();
		}
	}

	// Update UI
	CurrentGraphEditor->NotifyGraphChanged();
	
	if (UObject* GraphOwner = EdGraph->GetOuter())
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

bool FQuestEditor::CanPasteNodes() const
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (!CurrentGraphEditor.IsValid())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FQuestEditor::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FQuestEditor::CanDuplicateNodes() const
{
	return CanCopyNodes();
}

FGraphPanelSelectionSet FQuestEditor::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	if (const TSharedPtr<SGraphEditor> FocusedGraphEd = UpdateGraphEdPtr.Pin())
	{
		CurrentSelection = FocusedGraphEd->GetSelectedNodes();
	}

	return CurrentSelection;
}

void FQuestEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	const TArray<UObject*> Selection = GetSelectionForPropertyEditor(NewSelection);
	if (DetailsView.IsValid())
	{
		if (Selection.Num() > 0)
		{
			DetailsView->SetObjects(Selection);
		}
		else
		{
			DetailsView->SetObject(Quest.Get());
		}
	}
}

TArray<UObject*> FQuestEditor::GetSelectionForPropertyEditor(const TSet<UObject*>& InSelection)
{
	TArray<UObject*> Selection;
	for (UObject* SelectionEntry : InSelection)
	{
		if (const UQuestSchemaGraphNode* QuestSchemaGraphNode = Cast<UQuestSchemaGraphNode>(SelectionEntry))
		{
			Selection.Add(QuestSchemaGraphNode->NodeInstance);
		}
		else if (const UQuestSchemaTransitionGraphNode* TransitionGraphNode = Cast<UQuestSchemaTransitionGraphNode>(SelectionEntry))
		{
			Selection.Add(TransitionGraphNode->Transition);
		}
		else if (SelectionEntry->IsA<UEdGraphNode_Comment>())
		{
			Selection.Add(SelectionEntry);
		}
	}
	return Selection;
}

void FQuestEditor::OnGraphEditorFocused(const TSharedRef<SGraphEditor>& InGraphEditor)
{
	UpdateGraphEdPtr = InGraphEditor;

	const FGraphPanelSelectionSet CurrentSelection = InGraphEditor->GetSelectedNodes();
	OnSelectedNodesChanged(CurrentSelection);
}

void FQuestEditor::OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged)
{
	if (NodeBeingChanged)
	{
		static const FText TranscationTitle = FText::FromString(FString(TEXT("Rename Node")));
		const FScopedTransaction Transaction(TranscationTitle);
		NodeBeingChanged->Modify();
		NodeBeingChanged->OnRenameNode(NewText.ToString());
	}
}

void FQuestEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	Quest->QuestGraph->GetSchema()->ForceVisualizationCacheClear();
}

#undef LOCTEXT_NAMESPACE
