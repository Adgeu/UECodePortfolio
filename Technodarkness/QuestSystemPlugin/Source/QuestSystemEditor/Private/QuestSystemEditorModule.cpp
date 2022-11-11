#include "QuestSystemEditorModule.h"

#include "AssetToolsModule.h"
#include "AssetTypeActions_Quest.h"
#include "QuestStyle.h"
#include "Customizations/DocURLCustomization.h"
#include "Graph/QuestSchema/QuestSchemaGraphNodeFactory.h"
#include "Types/QuestTypes.h"

#define LOCTEXT_NAMESPACE "FQuestSystemEditorModule"

void FQuestSystemEditorModule::StartupModule()
{
	RegisterQuestAsset();

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FDocURL::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDocURLCustomization::MakeInstance));
}

void FQuestSystemEditorModule::ShutdownModule()
{
	UnregisterQuestAsset();
}

void FQuestSystemEditorModule::RegisterQuestAsset()
{
	// Register category
	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
	QuestCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("QuestCategory")), INVTEXT("QuestSystem"));

	// Register asset
	QuestTypeActions = MakeShareable(new FAssetTypeActions_Quest);
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(QuestTypeActions.ToSharedRef());

	// Register style
	FQuestStyle::Initialize();

	// Register quest graph nodes
	QuestSchemaGraphPanelNodeFactory = MakeShareable(new FQuestSchemaGraphNodeFactory);
	QuestSchemaPanelPinFactory = MakeShareable(new FQuestSchemaGraphPinFactory);
	QuestSchemaPanelPinConnectionFactory = MakeShareable(new FQuestSchemaGraphPinConnectionFactory);
	FEdGraphUtilities::RegisterVisualNodeFactory(QuestSchemaGraphPanelNodeFactory);
	FEdGraphUtilities::RegisterVisualPinFactory(QuestSchemaPanelPinFactory);
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(QuestSchemaPanelPinConnectionFactory);
}

void FQuestSystemEditorModule::UnregisterQuestAsset()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(QuestTypeActions.ToSharedRef());
	}
	FQuestStyle::Shutdown();
	
	FEdGraphUtilities::UnregisterVisualNodeFactory(QuestSchemaGraphPanelNodeFactory);
	FEdGraphUtilities::UnregisterVisualPinFactory(QuestSchemaPanelPinFactory);
	FEdGraphUtilities::UnregisterVisualPinConnectionFactory(QuestSchemaPanelPinConnectionFactory);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FQuestSystemEditorModule, QuestSystemEditor)