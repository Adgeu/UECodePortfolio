// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CraftingSystemTypes.generated.h"

///////////////////////////////////////////////////////

// Log category for crafting system.
DEFINE_LOG_CATEGORY_STATIC(LogCrafting, All, All);

///////////////////////////////////////////////////////

/** Types of the crafting workspaces. */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECraftingWorkspaceType : uint8
{
	// Invalid crafting workspace.
	None				= 0 UMETA(Hidden),

	// We can craft on crafting table.
	CraftingTable		= 1 << 0,

	// We can craft with gunsmith.
	Gunsmith			= 1 << 1,

	// We can craft on any workspaces.
	Any					= CraftingTable | Gunsmith
};
ENUM_CLASS_FLAGS(ECraftingWorkspaceType);

///////////////////////////////////////////////////////

/** Ingredient for crafting. */
USTRUCT(BlueprintType, meta = (HasNativeMake = "Technodarkness.CraftingBlueprintFunctionLibrary.MakeCraftingIngredient"))
struct FCraftingIngredient
{
	GENERATED_BODY()

	FCraftingIngredient() = default;
	FCraftingIngredient(TSubclassOf<UObject> IngredientClass_In, int32 Amount_In) :
		IngredientClass(IngredientClass_In), Amount(Amount_In)
	{
	}

	// Class of the ingredient.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MustImplement = "CraftingIngredientItem"));
	TSubclassOf<UObject> IngredientClass;

	// Amount of the ingredient.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1));
	int32 Amount = 1;
};

///////////////////////////////////////////////////////

/** Crafting ingredient info */
USTRUCT(BlueprintType)
struct FCraftingIngredientInfo
{
	GENERATED_BODY()

	// Ingredient displaying name.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;
};
