// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)


#include "CraftingSystem/CraftingBlueprintFunctionLibrary.h"


FCraftingIngredient UCraftingBlueprintFunctionLibrary::MakeCraftingIngredient(TSubclassOf<UObject> IngredientClass, int32 Amount)
{
	return FCraftingIngredient(IngredientClass, Amount);
}
