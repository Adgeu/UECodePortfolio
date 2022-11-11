// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "CraftingBaseRecipe.h"
#include "CraftingWeaponUpgradeRecipe.generated.h"

/**
 * Recipe asset that contains information for crafting weapon upgrade.
 */
UCLASS()
class TECHNODARKNESS_API UCraftingWeaponUpgradeRecipe : public UCraftingBaseRecipe
{
	GENERATED_BODY()

public:
	UCraftingWeaponUpgradeRecipe();

	/** Returns class of the weapon to which the upgrade belongs. */
	FORCEINLINE const TSubclassOf<UClass>& GetWeaponClass() const
	{
		return WeaponClass;
	}
	
protected:
	// Class of the weapon to which the upgrade belongs.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe Settings", meta = (MustImplement = "WeaponUpgradable"))
	TSubclassOf<UClass> WeaponClass;
};
