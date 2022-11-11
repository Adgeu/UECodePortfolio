// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "TDBaseCharacter.h"
#include "CraftingSystem/CraftingSystemTypes.h"
#include "CraftingSystem/Interfaces/WeaponUpgradable.h"
#include "TDPlayerCharacter.generated.h"

class UQuestLogComponent;
class UCraftingComponent;

UCLASS()
class TECHNODARKNESS_API ATDPlayerCharacter : public ATDBaseCharacter
{
	GENERATED_BODY()

public:
	ATDPlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	///////////////////////////////////////////////////////

#pragma region Crafting System

public:
	/** Returns all ingredients that the player has in inventory at the moment. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting System")
	TArray<FCraftingIngredient> GetCraftingIngredientsFromInventory() const;

	/**
	 * Check free space in inventory for item.
	 * @param DefaultItemToCheck Default object of the item that we want to check.
	 * @return True if there is free space for the item.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting System")
	bool CheckFreeSpaceInInventory(UObject* DefaultItemToCheck) const;

	/**
	 * Consume items that were used as ingredients for crafting.
	 * @param Ingredients Ingredients that we want to consume.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting System")
	void ConsumeCraftingIngredients(const TArray<FCraftingIngredient>& Ingredients) const;

	/** Returns all upgradable weapons that the player has at the moment. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting System")
	TArray<TScriptInterface<IWeaponUpgradable>> GetUpgradableWeapons() const;

protected:
	// Class of the widget for crafting system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting System")
	TSubclassOf<class UCraftingWidget> CraftingWidgetClass;

private:
	UFUNCTION()
	void OnBeginEnterCraftingMode();
	
	UFUNCTION()
	void OnEndEnterCraftingMode();

	UFUNCTION()
	void OnBeginExitCraftingMode();
	
	UPROPERTY()
	UCraftingWidget* CraftingWidget = nullptr;

#pragma endregion ~Crafting System

	///////////////////////////////////////////////////////

#pragma region Components

public:
	/** Returns quest log subobject. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE UQuestLogComponent* GetQuestLogComponent() const
	{
		return QuestLogNewComponent;
	}

	/** Returns crafting subobject. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE UCraftingComponent* GetCraftingComponent() const
	{
		return CraftingComponent;
	}
	
private:
	// Quest log that regulates quests.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quests", meta = (AllowPrivateAccess = "true"))
	UQuestLogComponent* QuestLogNewComponent;

	// Crafting component regulates items crafting.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crafting", meta = (AllowPrivateAccess = "true"))
	UCraftingComponent* CraftingComponent;

#pragma endregion ~Components
};
