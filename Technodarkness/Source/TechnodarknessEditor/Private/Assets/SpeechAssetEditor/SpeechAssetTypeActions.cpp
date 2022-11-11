// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#include "Assets/SpeechAssetEditor/SpeechAssetTypeActions.h"

#include "TechnodarknessEditor.h"
#include "Speech/SpeechAsset.h"

UClass* FSpeechAssetTypeActions::GetSupportedClass() const
{
	return USpeechAsset::StaticClass();
}

FText FSpeechAssetTypeActions::GetName() const
{
	return INVTEXT("Speech Asset");
}

FColor FSpeechAssetTypeActions::GetTypeColor() const
{
	return FColor(255, 69, 0);
}

uint32 FSpeechAssetTypeActions::GetCategories()
{
	const FTechnodarknessEditorModule& Module = FModuleManager::LoadModuleChecked<FTechnodarknessEditorModule>("TechnodarknessEditor");
	return Module.GetTechnoDarknessAssetCategoryBit();
}

const TArray<FText>& FSpeechAssetTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Speech")
	};
	return SubMenus;
}

void FSpeechAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}

