// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#include "Assets/CraftingWeaponUpgradeRecipeAsset/CraftingWeaponUpgradeRecipeTypeActions.h"

#include "TechnodarknessEditor.h"
#include "CraftingSystem/Recipes/CraftingWeaponUpgradeRecipe.h"

UClass* FCraftingWeaponUpgradeRecipeTypeActions::GetSupportedClass() const
{
	return UCraftingWeaponUpgradeRecipe::StaticClass();
}

FText FCraftingWeaponUpgradeRecipeTypeActions::GetName() const
{
	return INVTEXT("Weapon Upgrade Recipe");
}

FColor FCraftingWeaponUpgradeRecipeTypeActions::GetTypeColor() const
{
	return FColor(189, 218, 87);
}

uint32 FCraftingWeaponUpgradeRecipeTypeActions::GetCategories()
{
	const FTechnodarknessEditorModule& Module = FModuleManager::LoadModuleChecked<FTechnodarknessEditorModule>("TechnodarknessEditor");
	return Module.GetTechnoDarknessAssetCategoryBit();
}

const TArray<FText>& FCraftingWeaponUpgradeRecipeTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Crafting System")
	};
	return SubMenus;
}

void FCraftingWeaponUpgradeRecipeTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}
