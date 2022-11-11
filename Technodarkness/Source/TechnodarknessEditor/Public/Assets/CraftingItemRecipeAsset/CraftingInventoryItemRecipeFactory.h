// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CraftingInventoryItemRecipeFactory.generated.h"

/**
 * 
 */
UCLASS()
class TECHNODARKNESSEDITOR_API UCraftingInventoryItemRecipeFactory : public UFactory
{
	GENERATED_BODY()

public:
	UCraftingInventoryItemRecipeFactory();
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
