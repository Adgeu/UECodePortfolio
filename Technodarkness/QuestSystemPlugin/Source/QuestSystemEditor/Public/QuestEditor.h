#pragma once

#include "CoreMinimal.h"
#include "EditorUndoClient.h"
#include "Misc/NotifyHook.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class UQuest;

class FQuestEditor : public FWorkflowCentricApplication, public FNotifyHook, public FEditorUndoClient
{
public:
	FQuestEditor();
	virtual ~FQuestEditor();

	void InitQuestEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UQuest* InQuest);

	void SetDetailsView(TSharedPtr<class IDetailsView> InDetailsView)
	{
		DetailsView = InDetailsView;
	}
	
public:
	// FWorkflowCentricApplication Interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	// ~FWorkflowCentricApplication Interface

	// IToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	// ~IToolkit Interface

	void RegisterToolbarTab(const TSharedRef<FTabManager>& InTabManager);
	void InvokeQuestGraphTab();

	// Commands
	
	// * Delete
	void DeleteSelectedNodes();
	void DeleteSelectedDuplicatableNodes();
	bool CanDeleteNodes() const;
	// ~ Delete

	// * Comment
	void OnCreateComment();
	bool CanCreateComment() const;
	// ~ Commands

	// * Copy
	void CopySelectedNodes();
	bool CanCopyNodes() const;
	// ~ Copy

	// * Cut
	void CutSelectedNodes();
	bool CanCutNodes() const;
	// ~ Cut

	// * Paste
	void PasteNodes();
	void PasteNodesHere(const FVector2D& Location);
	bool CanPasteNodes() const;
	// ~ Paste

	// * Duplicate
	void DuplicateNodes();
	bool CanDuplicateNodes() const;
	// ~ Duplicate
	
	// ~Commands
	
	FGraphPanelSelectionSet GetSelectedNodes() const;
	virtual void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	TArray<UObject*> GetSelectionForPropertyEditor(const TSet<UObject*>& InSelection);

	// Delegates
	void OnGraphEditorFocused(const TSharedRef<SGraphEditor>& InGraphEditor);
	void OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged);
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	
	UQuest* GetQuest() const
	{
		return Quest.Get();
	}

	TSharedPtr<FDocumentTracker> GetDocumentManager() const
	{
		return DocumentManager;
	}

private:
	TWeakObjectPtr<UQuest> Quest;
	TSharedPtr<FDocumentTracker> DocumentManager;
	TSharedPtr<class IDetailsView> DetailsView;

	/** Currently focused graph */
	TWeakPtr<SGraphEditor> UpdateGraphEdPtr;
};
