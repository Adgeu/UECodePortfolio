// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "CraftingSystem/CraftingSystemTypes.h"
#include "UObject/Interface.h"
#include "CraftingIngredientItem.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCraftingIngredientItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * Inventory items that can be crafting ingredients should implement this interface.
 */
class TECHNODARKNESS_API ICraftingIngredientItem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Returns info of the ingredient. */
	UFUNCTION(BlueprintImplementableEvent)
	FCraftingIngredientInfo GetCraftingIngredientInfo() const;
};
