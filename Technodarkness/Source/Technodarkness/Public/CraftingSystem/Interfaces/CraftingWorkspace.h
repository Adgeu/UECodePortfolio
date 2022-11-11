// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "CraftingSystem/CraftingSystemTypes.h"
#include "UObject/Interface.h"
#include "CraftingWorkspace.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCraftingWorkspace : public UInterface
{
	GENERATED_BODY()
};

/**
 * Actors that represents place for crafting should implements this interface.
 */
class TECHNODARKNESS_API ICraftingWorkspace
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Returns available difficulty that we can work with while working on this workspace. */
	UFUNCTION(BlueprintImplementableEvent)
	ECraftingWorkspaceType GetCraftingWorkspaceType() const;

	/** Returns anim montage that should playing on entering crafting mode. */
	UFUNCTION(BlueprintImplementableEvent)
	UAnimMontage* GetEnterCraftingModeAnimMontage() const;

	/** Returns anim montage that should playing on exiting crafting mode. */
	UFUNCTION(BlueprintImplementableEvent)
	UAnimMontage* GetExitCraftingModeAnimMontage() const;
};
