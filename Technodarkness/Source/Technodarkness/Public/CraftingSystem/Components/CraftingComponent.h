// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingSystem/CraftingSystemTypes.h"
#include "CraftingSystem/Interfaces/CraftingWorkspace.h"
#include "CraftingSystem/Interfaces/WeaponUpgradable.h"
#include "CraftingComponent.generated.h"

class UCraftingBaseRecipe;
class UCraftingInventoryItemRecipe;
class UCraftingWeaponUpgradeRecipe;

/** Notifier for player begins entering crafting mode. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginEnterCraftingMode);
/** Notifier for player ends entering crafting mode. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndEnterCraftingMode);

/** Notifier for player begins exiting crafting mode. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginExitCraftingMode);
/** Notifier for player ends exiting crafting mode. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndExitCraftingMode);

/** Notifier for player begins crafting. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingBegins, const UCraftingBaseRecipe*, WithRecipe);
/** Notifier for player ends crafting. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingEnded, const UCraftingBaseRecipe*, WithRecipe);

/**
 * Notifier for inventory item was crafted.
 * @param InventoryItem Inventory item that was crafted.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftedInventoryItem, UObject*, InventoryItem);
/**
 * Notifier for weapon upgrade was crafted.
 * @param ForWeaponClass Class of the weapon for what we crafted upgrade.
 * @param UpgradeClass Class of the weapon upgrade that was crafted.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCraftedWeaponUpgrade, TSubclassOf<UObject>, ForWeaponClass, TSubclassOf<UObject>, UpgradeClass);

///////////////////////////////////////////////////////

/**
 * Component that manages various items crafting.
 * Items are crafted using recipes.
 * The component receives the ingredients for crafting from the inventory component.
 * Communication with the inventory is carried out through TDPlayerCharacter.
 *
 * @note UCraftingComponent works only with ATDPlayerCharacter.
 *
 * @see ATDPlayerCharacter, UCraftingBaseRecipe
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TECHNODARKNESS_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	/**
	 * Entering crafting mode on interaction with workspace.
	 * @param InCraftingWorkspace The crafting workspace from which we entered the crafting state.
	 */
	UFUNCTION(BlueprintCallable)
	void EnterCraftingMode(TScriptInterface<ICraftingWorkspace> InCraftingWorkspace);

	/**
	 * Exiting crafting mode.
	 */
	UFUNCTION(BlueprintCallable)
	void ExitCraftingMode();

	/**
	 * Start crafting item with Recipe.
	 * @param Recipe Item recipe that we want to craft.
	 */
	UFUNCTION(BlueprintCallable)
	void StartCrafting(const UCraftingBaseRecipe* Recipe);

	/** Checks if we can craft item with recipe. */
	bool CanCraft(const UCraftingBaseRecipe* Recipe) const;

	/** Add new recipe to the crafting recipes list. */
	UFUNCTION(BlueprintCallable)
	void AddCraftingRecipe(UCraftingBaseRecipe* RecipeToAdd);

	/** Returns recipes that we can craft based on current workspace difficulty and available ingredients. */
	TArray<UCraftingBaseRecipe*> GetAvailableRecipes() const;

	/** Returns recipes based on current workspace type. */
	TArray<UCraftingBaseRecipe*> GetRecipesByWorkspaceType() const;

	// Notify that the player begins entering crafting mode.
	UPROPERTY(BlueprintAssignable)
	FOnBeginEnterCraftingMode OnBeginEnterCraftingMode;

	// Notify that the player ends entering crafting mode.
	UPROPERTY(BlueprintAssignable)
	FOnEndEnterCraftingMode OnEndEnterCraftingMode;

	// Notify that the player begins exiting crafting mode.
	UPROPERTY(BlueprintAssignable)
	FOnBeginEnterCraftingMode OnBeginExitCraftingMode;

	// Notify that the player ends exiting crafting mode.
	UPROPERTY(BlueprintAssignable)
	FOnEndEnterCraftingMode OnEndExitCraftingMode;

	// Notify that the player begins crafting.
	UPROPERTY(BlueprintAssignable)
	FOnCraftingBegins OnCraftingBegins;

	// Notify that the player ends crafting.
	UPROPERTY(BlueprintAssignable)
	FOnCraftingEnded OnCraftingEnded;

	// Notify that the inventory item was crafted.
	UPROPERTY(BlueprintAssignable)
	FOnCraftedInventoryItem OnCraftedInventoryItem;

	// Notify that the weapon upgrade was crafted.
	UPROPERTY(BlueprintAssignable)
	FOnCraftedWeaponUpgrade OnCraftedWeaponUpgrade;
	
protected:
	virtual void BeginPlay() override;

	// Crafting recipes.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Recipes")
	TArray<UCraftingBaseRecipe*> CraftingRecipes;

private:
	/** Start craft inventory item with recipe. */
	void CraftInventoryItem(const UCraftingInventoryItemRecipe* Recipe);
	/** On inventory item crafted with recipe. */
	void OnCraftInventoryItemEnded(const UCraftingInventoryItemRecipe* Recipe);
	
	/** Start craft weapon upgrade with recipe. */
	void CraftWeaponUpgrade(const UCraftingWeaponUpgradeRecipe* Recipe);
	/** On weapon upgrade crafted with recipe. */
	void OnCraftWeaponUpgradeEnded(UObject* InWeapon, const UCraftingWeaponUpgradeRecipe* Recipe);
	
	/** Ends entering crafting mode. */
	void EndEnterCraftingMode();
	/** Ends exiting crafting mode. */
	void EndExitCraftingMode();
	
	/** Checks if there are enough ingredients in available ingredients list. */
	bool AreThereEnoughIngredients(const TArray<FCraftingIngredient>& IngredientsToCheck) const;

	/** Consume ingredient in inventory and refresh available ingredients. */
	void ConsumeIngredients(const TArray<FCraftingIngredient>& Ingredients);

	// List of available ingredients that the player has.
	TArray<FCraftingIngredient> AvailableIngredients;

	// List of upgradable weapons that the player has.
	TArray<TScriptInterface<IWeaponUpgradable>> UpgradableWeapons;

	// Crafting workspace we are currently working on.
	TScriptInterface<ICraftingWorkspace> CurrentCraftingWorkspace;

	/**
	 * Cached player character.
	 * @note Crafting component can only works with TDPlayerCharacter.
	 * @see ATDPlayerCharacter
	 */
	TWeakObjectPtr<class ATDPlayerCharacter> CachedCharacter;

	///////////////////////////////////////////////////////

#pragma region Helpers

private:
	/**
	 * Gets upgradable weapon from UpgradableWeapons array by its class.
	 *
	 * @param WeaponClass Class of the weapon that we want to get.
	 * @param OutUpgradableWeapon [OUT] Found weapon as IWeaponUpgradable.
	 * @return UpgradableWeapons array has weapon with the class.
	 */
	bool GetUpgradableWeaponByClass(const TSubclassOf<UObject>& WeaponClass, OUT TScriptInterface<IWeaponUpgradable>& OutUpgradableWeapon) const;
	
	/** Returns type of the current workspace. */
	FORCEINLINE ECraftingWorkspaceType GetWorkspaceType() const
	{
		return ICraftingWorkspace::Execute_GetCraftingWorkspaceType(CurrentCraftingWorkspace.GetObject());
	}

	/** Returns enter anim montage of the current workspace. */
	FORCEINLINE UAnimMontage* GetWorkspaceEnterAnimMontage() const
	{
		return ICraftingWorkspace::Execute_GetEnterCraftingModeAnimMontage(CurrentCraftingWorkspace.GetObject());
	}

	/** Returns exit anim montage of the current workspace. */
	FORCEINLINE UAnimMontage* GetWorkspaceExitAnimMontage() const
	{
		return ICraftingWorkspace::Execute_GetExitCraftingModeAnimMontage(CurrentCraftingWorkspace.GetObject());
	}

#pragma endregion ~Helpers

	///////////////////////////////////////////////////////

#pragma region [EDITOR] Updating recipes array

// It's just a helper functionality for filling CraftingRecipe array.
// Property bRefreshRecipesArray working as a button.
// Check bRefreshRecipesArray for refilling CraftingRecipe array.

 #if WITH_EDITORONLY_DATA

private:
	/** Refreshing CraftingRecipes array. */
	void RefreshRecipesArray();

	// Specify the directory from which we want to take recipe assets. Leave it empty if we want to take all the recipes in the project.
	UPROPERTY(EditDefaultsOnly, Category = "Crafting Recipes", meta = (AllowPrivateAccess = "true", DisplayName = "[Editor] Recipes Root Dir", DisplayPriority = 0))
	FString RecipesRootDir;

	// Click on this checkbox for refreshing crafting recipes array.
	UPROPERTY(EditDefaultsOnly, Category = "Crafting Recipes", meta = (AllowPrivateAccess = "true", DisplayName = "[Editor] Refresh Recipes Array Button", DisplayPriority = 1))
	bool bRefreshRecipesArray = false;

#endif

#pragma endregion ~[EDITOR] Updating recipes array
};
