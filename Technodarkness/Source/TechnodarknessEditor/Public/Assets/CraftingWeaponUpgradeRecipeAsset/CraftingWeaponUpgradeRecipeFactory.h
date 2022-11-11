// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CraftingWeaponUpgradeRecipeFactory.generated.h"

/**
 * 
 */
UCLASS()
class TECHNODARKNESSEDITOR_API UCraftingWeaponUpgradeRecipeFactory : public UFactory
{
	GENERATED_BODY()

public:
	UCraftingWeaponUpgradeRecipeFactory();
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
