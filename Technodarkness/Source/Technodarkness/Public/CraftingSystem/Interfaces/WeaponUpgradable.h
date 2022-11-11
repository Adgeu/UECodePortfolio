// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponUpgradable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UWeaponUpgradable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Weapons that can be upgraded should implement this interface.
 */
class TECHNODARKNESS_API IWeaponUpgradable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Returns info of the weapon.
	 * @todo Change signature when the weapon upgrades feature appears
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void GetUpgradableWeaponInfo(FText& DisplayName) const;
	
	/** Returns all current weapon upgrades. */
	UFUNCTION(BlueprintImplementableEvent)
	TArray<UObject*> GetWeaponUpgrades() const;

	/** Checks if there is an installed upgrade with class. */
	UFUNCTION(BlueprintImplementableEvent)
	bool HasWeaponUpgrade(TSubclassOf<UObject> WeaponUpgradeClass) const;

	/** Set upgrade to the weapon. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetWeaponUpgrade(TSubclassOf<UObject> WeaponUpgradeClass);
};
