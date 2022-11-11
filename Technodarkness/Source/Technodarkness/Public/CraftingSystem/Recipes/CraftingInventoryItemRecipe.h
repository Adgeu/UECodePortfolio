// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "CraftingBaseRecipe.h"
#include "CraftingInventoryItemRecipe.generated.h"

/**
 * Recipe asset that contains information for crafting inventory items.
 */
UCLASS(BlueprintType)
class TECHNODARKNESS_API UCraftingInventoryItemRecipe : public UCraftingBaseRecipe
{
	GENERATED_BODY()

public:
	UCraftingInventoryItemRecipe();
};
