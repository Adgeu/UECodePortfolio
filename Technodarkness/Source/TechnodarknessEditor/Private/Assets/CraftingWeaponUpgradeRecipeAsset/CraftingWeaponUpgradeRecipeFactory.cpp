// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)


#include "Assets/CraftingWeaponUpgradeRecipeAsset/CraftingWeaponUpgradeRecipeFactory.h"

#include "CraftingSystem/Recipes/CraftingWeaponUpgradeRecipe.h"

UCraftingWeaponUpgradeRecipeFactory::UCraftingWeaponUpgradeRecipeFactory()
{
	SupportedClass = UCraftingWeaponUpgradeRecipe::StaticClass();
	bCreateNew = true;
}

UObject* UCraftingWeaponUpgradeRecipeFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UCraftingWeaponUpgradeRecipe>(InParent, InClass, InName, Flags, Context);
}
