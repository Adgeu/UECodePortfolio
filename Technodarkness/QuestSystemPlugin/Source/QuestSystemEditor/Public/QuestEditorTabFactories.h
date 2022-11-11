#pragma once

#include "WorkflowOrientedApp/WorkflowTabFactory.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"

////////////////////////////////////////////////////////////////////////
/// QUEST SCHEMA

struct FQuestSchemaDetailsTabFactory : public FWorkflowTabFactory
{
public:
	FQuestSchemaDetailsTabFactory(TSharedPtr<class FQuestEditor> InQuestEditor);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<FQuestEditor> QuestEditor;
};

////////////////////////////////////////////////////////////////////////

struct FQuestSchemaGraphTabFactory : public FDocumentTabFactoryForObjects<class UQuestSchemaGraph>
{
public:
	FQuestSchemaGraphTabFactory(TSharedPtr<class FQuestEditor> InQuestEditor);

	virtual void OnTabActivated(TSharedPtr<SDockTab> Tab) const override;
	virtual void OnTabRefreshed(TSharedPtr<SDockTab> Tab) const override;

protected:
	void CreateCommandList();

	virtual TAttribute<FText> ConstructTabNameForObject(UQuestSchemaGraph* DocumentID) const override;
	virtual TSharedRef<SWidget> CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info, UQuestSchemaGraph* DocumentID) const override;
	virtual const FSlateBrush* GetTabIconForObject(const FWorkflowTabSpawnInfo& Info, UQuestSchemaGraph* DocumentID) const override;

private:
	TWeakPtr<FQuestEditor> QuestEditor;

	/** The command list for this editor */
	TSharedPtr<FUICommandList> GraphEditorCommands;
};

////////////////////////////////////////////////////////////////////////
/// QUEST TRIGGER


