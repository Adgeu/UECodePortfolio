// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "CraftingSystemTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CraftingBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TECHNODARKNESS_API UCraftingBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Crafting System", meta = (BlueprintThreadSafe))
	static FCraftingIngredient MakeCraftingIngredient(TSubclassOf<UObject> IngredientClass, int32 Amount);
};
