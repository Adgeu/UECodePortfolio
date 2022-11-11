// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "CraftingSystem/CraftingSystemTypes.h"
#include "UObject/Object.h"
#include "CraftingBaseRecipe.generated.h"

/**
 * Types of the recipe for easy identification.
 */
UENUM()
enum class ERecipeType : uint8
{
	// Base type (Not valid).
	None,

	// Recipe for inventory item.
	InventoryItem,

	// Recipe for weapon upgrade.
	WeaponUpgrade
};

class UAnimMontage;

/**
 * Recipe asset that contains information for crafting items.
 */
UCLASS(Abstract)
class TECHNODARKNESS_API UCraftingBaseRecipe : public UObject
{
	GENERATED_BODY()

public:
	/** Returns allowed workspaces for this recipe. */
	FORCEINLINE ECraftingWorkspaceType GetAllowedWorkspaces() const
	{
		return static_cast<ECraftingWorkspaceType>(AllowedWorkspaces);
	}

	/** Returns recipe displaying name. */
	FORCEINLINE const FText& GetDisplayingName() const
	{
		return DisplayingName;
	}

	/** Returns recipe description. */
	FORCEINLINE const FText& GetDescription() const
	{
		return Description;
	}
	
	/** Loads and returns recipe displaying icon image. */
	FORCEINLINE UTexture2D* GetDisplayingIcon() const
	{
		return DisplayingIcon.LoadSynchronous();
	}
	
	/** Returns ingredients that item requires for crafting. */
	FORCEINLINE const TArray<FCraftingIngredient>& GetIngredients() const
	{
		return Ingredients;
	}
	
	/** Returns crafting anim montage. */
	FORCEINLINE UAnimMontage* GetCraftingAnimMontage() const
	{
		return CraftingAnimMontage;
	}
	
	/** Returns class of result item. */
	FORCEINLINE const TSubclassOf<UObject>& GetCraftingResultClass() const
	{
		return CraftingResultClass;
	}

	/** Returns type of the recipe for easy identification. */
	FORCEINLINE ERecipeType GetRecipeType() const
	{
		return RecipeType;
	}
	
protected:
	// Allowed types of workspaces on which we can craft items with this recipe.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe Settings", meta = (Bitmask, BitmaskEnum = ECraftingWorkspaceType))
	uint8 AllowedWorkspaces = 0;

	// Recipe displaying name.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe Settings")
	FText DisplayingName;

	// Recipe description.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe Settings")
	FText Description;

	// Recipe displaying icon image.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe Settings")
	TSoftObjectPtr<UTexture2D> DisplayingIcon;
	
	// Ingredients that item requires for crafting.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredients")
	TArray<FCraftingIngredient> Ingredients;

	// Crafting anim montage.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* CraftingAnimMontage;

	// Class of result item.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Result")
	TSubclassOf<UObject> CraftingResultClass;

	// Type of the recipe for easy identification.
	ERecipeType RecipeType = ERecipeType::None;
};
