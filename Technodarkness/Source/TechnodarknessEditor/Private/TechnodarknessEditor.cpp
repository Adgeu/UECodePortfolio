#include "TechnodarknessEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Assets/CraftingItemRecipeAsset/CraftingInventoryItemRecipeTypeActions.h"
#include "Assets/CraftingWeaponUpgradeRecipeAsset/CraftingWeaponUpgradeRecipeTypeActions.h"
#include "CustomDeveloperSettings/InputRebindSettings.h"
#include "Customizations/InputAxisCustomization.h"
#include "Assets/SpeechAssetEditor/SpeechAssetTypeActions.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FTechnodarknessEditorModule"

void FTechnodarknessEditorModule::StartupModule()
{
	RegisterTechnoDarknessCategory();
	
	RegisterSpeechAsset();
	RegisterCraftingInventoryItemRecipe();
	RegisterCraftingWeaponUpgradeRecipe();
	
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FInputAxis::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FInputAxisCustomization::MakeInstance));
}

void FTechnodarknessEditorModule::ShutdownModule()
{
	UnregisterSpeechAsset();
	UnregisterCraftingInventoryItemRecipe();
	UnregisterCraftingWeaponUpgradeRecipe();
}

FString FTechnodarknessEditorModule::GetRootDir() const
{
	return FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir() + "Source/TechnodarknessEditor"));
}

void FTechnodarknessEditorModule::RegisterTechnoDarknessCategory()
{
	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
	TechnoDarknessAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("TechnoDarknessCategory")), INVTEXT("_TechnoDarkness"));
}

void FTechnodarknessEditorModule::RegisterSpeechAsset()
{
	// Register asset
	SpeechAssetTypeActions = MakeShareable(new FSpeechAssetTypeActions);
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(SpeechAssetTypeActions.ToSharedRef());

	// Register thumbnail
	SpeechStyleSet = MakeShareable(new FSlateStyleSet("SpeechStyle"));
	SpeechStyleSet->SetContentRoot(GetRootDir());
	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(SpeechStyleSet->RootToContentDir(TEXT("Resources/SpeechAsset_Icon_128"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	if (ThumbnailBrush)
	{
		SpeechStyleSet->Set("ClassThumbnail.SpeechAsset", ThumbnailBrush);
		FSlateStyleRegistry::RegisterSlateStyle(*SpeechStyleSet);
	}
}

void FTechnodarknessEditorModule::UnregisterSpeechAsset()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(SpeechAssetTypeActions.ToSharedRef());
	}

	FSlateStyleRegistry::UnRegisterSlateStyle(SpeechStyleSet->GetStyleSetName());
}

void FTechnodarknessEditorModule::RegisterCraftingInventoryItemRecipe()
{
	// Register asset
	CraftingInventoryItemRecipeTypeActions = MakeShareable(new FCraftingInventoryItemRecipeTypeActions);
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(CraftingInventoryItemRecipeTypeActions.ToSharedRef());

	// Register thumbnail
	CraftingInventoryItemRecipeStyleSet = MakeShareable(new FSlateStyleSet("CraftingInventoryItemRecipeStyle"));
	CraftingInventoryItemRecipeStyleSet->SetContentRoot(GetRootDir());
	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(CraftingInventoryItemRecipeStyleSet->RootToContentDir(TEXT("Resources/CraftingItemRecipe_Icon_128"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	if (ThumbnailBrush)
	{
		CraftingInventoryItemRecipeStyleSet->Set("ClassThumbnail.CraftingInventoryItemRecipe", ThumbnailBrush);
		FSlateStyleRegistry::RegisterSlateStyle(*CraftingInventoryItemRecipeStyleSet);
	}
}

void FTechnodarknessEditorModule::UnregisterCraftingInventoryItemRecipe()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(CraftingInventoryItemRecipeTypeActions.ToSharedRef());
	}

	FSlateStyleRegistry::UnRegisterSlateStyle(CraftingInventoryItemRecipeStyleSet->GetStyleSetName());
}

void FTechnodarknessEditorModule::RegisterCraftingWeaponUpgradeRecipe()
{
	// Register asset
	CraftingWeaponUpgradeRecipeTypeActions = MakeShareable(new FCraftingWeaponUpgradeRecipeTypeActions);
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(CraftingWeaponUpgradeRecipeTypeActions.ToSharedRef());

	// Register thumbnail
	CraftingWeaponUpgradeRecipeStyleSet = MakeShareable(new FSlateStyleSet("CraftingWeaponUpgradeRecipeStyle"));
	CraftingWeaponUpgradeRecipeStyleSet->SetContentRoot(GetRootDir());
	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(CraftingWeaponUpgradeRecipeStyleSet->RootToContentDir(TEXT("Resources/CraftingWeaponUpgradeRecipe_Icon_128"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	if (ThumbnailBrush)
	{
		CraftingWeaponUpgradeRecipeStyleSet->Set("ClassThumbnail.CraftingWeaponUpgradeRecipe", ThumbnailBrush);
		FSlateStyleRegistry::RegisterSlateStyle(*CraftingWeaponUpgradeRecipeStyleSet);
	}
}

void FTechnodarknessEditorModule::UnregisterCraftingWeaponUpgradeRecipe()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(CraftingWeaponUpgradeRecipeTypeActions.ToSharedRef());
	}

	FSlateStyleRegistry::UnRegisterSlateStyle(CraftingWeaponUpgradeRecipeStyleSet->GetStyleSetName());
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FTechnodarknessEditorModule, TechnodarknessEditor)