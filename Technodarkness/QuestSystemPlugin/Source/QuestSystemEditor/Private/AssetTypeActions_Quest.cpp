// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "AssetTypeActions_Quest.h"

#include "QuestEditor.h"
#include "QuestSystemEditorModule.h"
#include "QuestData/Quest.h"

FText FAssetTypeActions_Quest::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_QuestAsset", "Quest Asset");
}

FColor FAssetTypeActions_Quest::GetTypeColor() const
{
	return FColor(106, 90, 205);
}

UClass* FAssetTypeActions_Quest::GetSupportedClass() const
{
	return UQuest::StaticClass();
}

void FAssetTypeActions_Quest::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* Object : InObjects)
	{
		if (UQuest* NewAsset = Cast<UQuest>(Object))
		{
			TSharedRef<FQuestEditor> NewEditor(new FQuestEditor());
			NewEditor->InitQuestEditor(Mode, EditWithinLevelEditor, NewAsset);
		}
	}
}

uint32 FAssetTypeActions_Quest::GetCategories()
{
	const FQuestSystemEditorModule& Module = FModuleManager::LoadModuleChecked<FQuestSystemEditorModule>("QuestSystemEditor");
	return Module.GetQuestCategoryBit();
}
