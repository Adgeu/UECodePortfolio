#include "QuestEditorApplicationModes.h"

#include "QuestEditor.h"
#include "QuestEditorTabFactories.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FQuestEditorApplicationModes

const FName FQuestEditorApplicationModesStatics::QuestSchemaMode(TEXT("QuestSchemaMode"));
const FName FQuestEditorApplicationModesStatics::QuestTriggerMode(TEXT("QuestTriggerMode"));

#define LOCTEXT_NAMESPACE "QuestEditor"

FText FQuestEditorApplicationModesStatics::GetLocalizedMode(FName InMode)
{
	static TMap<FName, FText> LocModes;

	if (LocModes.Num() == 0)
	{
		LocModes.Add(QuestSchemaMode, LOCTEXT("QuestSchemaMode", "Quest Schema"));
		LocModes.Add(QuestTriggerMode, LOCTEXT("QuestTriggerMode", "Quest Trigger"));
	}

	check(InMode != NAME_None);
	const FText* OutDesc = LocModes.Find(InMode);
	check(OutDesc);
	return *OutDesc;

}

#undef LOCTEXT_NAMESPACE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FQuestSchemaEditorApplicationMode

FQuestSchemaEditorApplicationMode::FQuestSchemaEditorApplicationMode(TSharedPtr<FQuestEditor> InQuestEditor)
	:FApplicationMode(FQuestEditorApplicationModesStatics::QuestSchemaMode, FQuestEditorApplicationModesStatics::GetLocalizedMode)
{
	QuestEditor = InQuestEditor;

	QuestTabFactories.RegisterFactory(MakeShareable(new FQuestSchemaDetailsTabFactory(InQuestEditor)));
	TabLayout = FTabManager::NewLayout("QuestSchemaEditorLayout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.2f)
			->SetHideTabWell(true)
			->AddTab(InQuestEditor->GetToolbarTabId(), ETabState::OpenedTab)
		)
		->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.85f)
					->AddTab("Document", ETabState::OpenedTab)
				)
			)
			->Split
			(
			FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.15f)
					->AddTab("Details", ETabState::OpenedTab)
				)
			)
		)
	);
}

void FQuestSchemaEditorApplicationMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FQuestEditor> Editor = QuestEditor.Pin();
	Editor->RegisterToolbarTab(InTabManager.ToSharedRef());
	Editor->PushTabFactories(QuestTabFactories);
	Editor->GetDocumentManager()->RegisterDocumentFactory(MakeShareable(new FQuestSchemaGraphTabFactory(Editor)));
	
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FQuestSchemaEditorApplicationMode::AddTabFactory(FCreateWorkflowTabFactory FactoryCreator)
{
	if (FactoryCreator.IsBound())
	{
		QuestTabFactories.RegisterFactory(FactoryCreator.Execute(QuestEditor.Pin()));
	}
}

void FQuestSchemaEditorApplicationMode::RemoveTabFactory(FName TabFactoryID)
{
	QuestTabFactories.UnregisterFactory(TabFactoryID);
}

void FQuestSchemaEditorApplicationMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FQuestSchemaEditorApplicationMode::PostActivateMode()
{
	QuestEditor.Pin()->InvokeQuestGraphTab();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FQuestTriggerEditorApplicationMode

FQuestTriggerEditorApplicationMode::FQuestTriggerEditorApplicationMode(TSharedPtr<FQuestEditor> InQuestEditor)
	:FApplicationMode(FQuestEditorApplicationModesStatics::QuestTriggerMode, FQuestEditorApplicationModesStatics::GetLocalizedMode)
{
	QuestEditor = InQuestEditor;

	TabLayout = FTabManager::NewLayout("QuestTriggerEditorLayout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.2f)
			->SetHideTabWell(true)
			->AddTab(InQuestEditor->GetToolbarTabId(), ETabState::OpenedTab)
		)
		->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.1f)
					->AddTab("Palette", ETabState::ClosedTab)
				)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.75f)
					->AddTab("Graph", ETabState::ClosedTab)
				)
			)
			->Split
			(
			FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.15f)
					->AddTab("Details", ETabState::ClosedTab)
				)
			)
		)
	);
}

void FQuestTriggerEditorApplicationMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FQuestTriggerEditorApplicationMode::AddTabFactory(FCreateWorkflowTabFactory FactoryCreator)
{
	FApplicationMode::AddTabFactory(FactoryCreator);
}

void FQuestTriggerEditorApplicationMode::RemoveTabFactory(FName TabFactoryID)
{
	FApplicationMode::RemoveTabFactory(TabFactoryID);
}

void FQuestTriggerEditorApplicationMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FQuestTriggerEditorApplicationMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
