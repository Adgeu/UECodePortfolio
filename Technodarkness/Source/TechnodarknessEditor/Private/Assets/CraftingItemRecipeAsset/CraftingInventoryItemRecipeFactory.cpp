// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)


#include "Assets/CraftingItemRecipeAsset/CraftingInventoryItemRecipeFactory.h"

#include "CraftingSystem/Recipes/CraftingInventoryItemRecipe.h"

UCraftingInventoryItemRecipeFactory::UCraftingInventoryItemRecipeFactory()
{
	SupportedClass = UCraftingInventoryItemRecipe::StaticClass();
	bCreateNew = true;
}

UObject* UCraftingInventoryItemRecipeFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UCraftingInventoryItemRecipe>(InParent, InClass, InName, Flags, Context);
}
