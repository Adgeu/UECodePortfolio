#pragma once
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class FQuestEditorApplicationModesStatics
{
public:
	static FText GetLocalizedMode(FName InMode);
	
	/* Modes in mode switcher */
	static const FName QuestSchemaMode;
	static const FName QuestTriggerMode;
};

///////////////////////////////////////////////////////

class FQuestSchemaEditorApplicationMode : public FApplicationMode
{
public:
	FQuestSchemaEditorApplicationMode(TSharedPtr<class FQuestEditor> InQuestEditor);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void AddTabFactory(FCreateWorkflowTabFactory FactoryCreator) override;
	virtual void RemoveTabFactory(FName TabFactoryID) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<FQuestEditor> QuestEditor;
	FWorkflowAllowedTabSet QuestTabFactories;
};

///////////////////////////////////////////////////////

class FQuestTriggerEditorApplicationMode : public FApplicationMode
{
public:
	FQuestTriggerEditorApplicationMode(TSharedPtr<class FQuestEditor> InQuestEditor);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void AddTabFactory(FCreateWorkflowTabFactory FactoryCreator) override;
	virtual void RemoveTabFactory(FName TabFactoryID) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<FQuestEditor> QuestEditor;
	FWorkflowAllowedTabSet QuestTabFactories;
};
