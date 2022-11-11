#include "QuestEditorTabFactories.h"

#include "GraphEditorActions.h"
#include "QuestEditor.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/QuestSchema/QuestSchemaGraph.h"
#include "QuestData/Quest.h"

#define LOCTEXT_NAMESPACE "QuestTabFactory"

////////////////////////////////////////////////////////////////////////
// FQuestSchemaDetailsTabFactory

FQuestSchemaDetailsTabFactory::FQuestSchemaDetailsTabFactory(TSharedPtr<FQuestEditor> InQuestEditor)
	: FWorkflowTabFactory(FName("Details"), InQuestEditor)
	, QuestEditor(InQuestEditor)
{
	TabLabel = LOCTEXT("DetailsLabel", "Details");
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "Kismet.Tabs.Components");
	bIsSingleton = true;
	ViewMenuDescription = LOCTEXT("DetailsView", "Details");
	ViewMenuTooltip = LOCTEXT("DetailsView_ToolTip", "Show the details view");
}

TSharedRef<SWidget> FQuestSchemaDetailsTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	// Create a detail panel
	FQuestEditor* Editor = QuestEditor.Pin().Get();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, false);
	DetailsViewArgs.NotifyHook = Editor;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	TSharedRef<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(Editor->GetQuest());
	DetailsView->OnFinishedChangingProperties().AddSP(QuestEditor.Pin().Get(), &FQuestEditor::OnFinishedChangingProperties);
	Editor->SetDetailsView(DetailsView);
	//DetailsView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(Editor, &FQuestEditor::IsPropertyEditable));
	//DetailsView->OnFinishedChangingProperties().AddSP(Editor, &FQuestEditor::OnFinishedChangingProperties);

	return 		
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			DetailsView
		];
}

FText FQuestSchemaDetailsTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("DetailsTabTooltip", "The details tab allows editing of the properties");
}

////////////////////////////////////////////////////////////////////////
// FQuestSchemaGraphTabFactory

FQuestSchemaGraphTabFactory::FQuestSchemaGraphTabFactory(TSharedPtr<FQuestEditor> InQuestEditor)
	:  FDocumentTabFactoryForObjects<UQuestSchemaGraph>(FName("QuestSchemaGraph"), InQuestEditor)
	, QuestEditor(InQuestEditor)
{
	TabLabel = LOCTEXT("QuestSchemaGraphLabel", "QuestSchemaGraph");
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "Kismet.Tabs.Components");
	bIsSingleton = true;
	ViewMenuDescription = LOCTEXT("QuestSchemaGraphView", "QuestSchemaGraph");
	ViewMenuTooltip = LOCTEXT("QuestSchemaGraphToolTip", "Quest Schema Graph");

	CreateCommandList();
}

void FQuestSchemaGraphTabFactory::OnTabActivated(TSharedPtr<SDockTab> Tab) const
{
	check(QuestEditor.IsValid());
	const TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	QuestEditor.Pin()->OnGraphEditorFocused(GraphEditor);
}

void FQuestSchemaGraphTabFactory::OnTabRefreshed(TSharedPtr<SDockTab> Tab) const
{
	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	GraphEditor->NotifyGraphChanged();
}

void FQuestSchemaGraphTabFactory::CreateCommandList()
{
	if (GraphEditorCommands.IsValid())
	{
		return;
	}

	GraphEditorCommands = MakeShareable(new FUICommandList);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::CanDeleteNodes));

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::CanCutNodes));

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::CanCopyNodes));

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::PasteNodes),
		FCanExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::CanPasteNodes));

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::DuplicateNodes),
		FCanExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::CanDuplicateNodes));

	GraphEditorCommands->MapAction(
		FGraphEditorCommands::Get().CreateComment,
		FExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::OnCreateComment),
		FCanExecuteAction::CreateRaw(QuestEditor.Pin().Get(), &FQuestEditor::CanCreateComment));
}

TAttribute<FText> FQuestSchemaGraphTabFactory::ConstructTabNameForObject(UQuestSchemaGraph* DocumentID) const
{
	return TAttribute<FText>(FText::FromString(DocumentID->GetName()));
}

TSharedRef<SWidget> FQuestSchemaGraphTabFactory::CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info, UQuestSchemaGraph* DocumentID) const
{
	check(DocumentID);

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(QuestEditor.Pin().Get(), &FQuestEditor::OnSelectedNodesChanged);
	InEvents.OnTextCommitted = FOnNodeTextCommitted::CreateSP(QuestEditor.Pin().Get(), &FQuestEditor::OnNodeTitleCommitted);

	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_QuestSchema", "QUEST SCHEMA");

	// Make title bar
	TSharedRef<SWidget> TitleBarWidget =
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(1.f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("QuestSchemaGraphLabel", "Quest Schema"))
				.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
			]
		];

	// Make full graph editor
	//const bool bGraphIsEditable = InGraph->bEditable;
	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.TitleBar(TitleBarWidget)
		.GraphToEdit(DocumentID)
		.GraphEvents(InEvents);
}

const FSlateBrush* FQuestSchemaGraphTabFactory::GetTabIconForObject(const FWorkflowTabSpawnInfo& Info, UQuestSchemaGraph* DocumentID) const
{
	return nullptr;
}

#undef LOCTEXT_NAMESPACE
