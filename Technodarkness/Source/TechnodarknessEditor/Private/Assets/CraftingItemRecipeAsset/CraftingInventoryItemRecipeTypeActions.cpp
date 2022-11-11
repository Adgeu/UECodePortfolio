// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#include "Assets/CraftingItemRecipeAsset/CraftingInventoryItemRecipeTypeActions.h"

#include "TechnodarknessEditor.h"
#include "CraftingSystem/Recipes/CraftingInventoryItemRecipe.h"

UClass* FCraftingInventoryItemRecipeTypeActions::GetSupportedClass() const
{
	return UCraftingInventoryItemRecipe::StaticClass();
}

FText FCraftingInventoryItemRecipeTypeActions::GetName() const
{
	return INVTEXT("Inventory Item Recipe");
}

FColor FCraftingInventoryItemRecipeTypeActions::GetTypeColor() const
{
	return FColor(189, 218, 87);
}

uint32 FCraftingInventoryItemRecipeTypeActions::GetCategories()
{
	const FTechnodarknessEditorModule& Module = FModuleManager::LoadModuleChecked<FTechnodarknessEditorModule>("TechnodarknessEditor");
	return Module.GetTechnoDarknessAssetCategoryBit();
}

const TArray<FText>& FCraftingInventoryItemRecipeTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		INVTEXT("Crafting System")
	};
	return SubMenus;
}

void FCraftingInventoryItemRecipeTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}
