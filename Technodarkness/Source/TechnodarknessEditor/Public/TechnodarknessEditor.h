#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleManager.h"

class FTechnodarknessEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    FString GetRootDir() const;

    ///////////////////////////////////////////////////////

#pragma region TechnoDarkness Category

public:
    FORCEINLINE EAssetTypeCategories::Type GetTechnoDarknessAssetCategoryBit() const
    {
        return TechnoDarknessAssetCategoryBit;
    }
    
private:
    void RegisterTechnoDarknessCategory();
    
    EAssetTypeCategories::Type TechnoDarknessAssetCategoryBit;

#pragma endregion ~TechnoDarkness Category
    
    ///////////////////////////////////////////////////////


#pragma region Speech
    
private:
    void RegisterSpeechAsset();
    void UnregisterSpeechAsset();
    
    TSharedPtr<class FSpeechAssetTypeActions> SpeechAssetTypeActions;
    TSharedPtr<class FSlateStyleSet> SpeechStyleSet;

#pragma endregion ~Speech

    ///////////////////////////////////////////////////////

#pragma region Crafting System
    
private:
    void RegisterCraftingInventoryItemRecipe();
    void UnregisterCraftingInventoryItemRecipe();

    void RegisterCraftingWeaponUpgradeRecipe();
    void UnregisterCraftingWeaponUpgradeRecipe();
    
    TSharedPtr<class FCraftingInventoryItemRecipeTypeActions> CraftingInventoryItemRecipeTypeActions;
    TSharedPtr<class FSlateStyleSet> CraftingInventoryItemRecipeStyleSet;

    TSharedPtr<class FCraftingWeaponUpgradeRecipeTypeActions> CraftingWeaponUpgradeRecipeTypeActions;
    TSharedPtr<class FSlateStyleSet> CraftingWeaponUpgradeRecipeStyleSet;

#pragma endregion ~Crafting System
};
