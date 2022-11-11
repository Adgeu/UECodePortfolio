// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingWidget.generated.h"

class UCraftingRecipeListWidget;
class UButton;

class UCraftingComponent;
class UCraftingBaseRecipe;

/**
 * Main widget for crafting system. (TEMP!)
 */
UCLASS()
class TECHNODARKNESS_API UCraftingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCraftingComponent(UCraftingComponent* CraftingComponent);
	virtual void NativeConstruct() override;

	void RefreshRecipesList();

protected:
	UPROPERTY(meta = (BindWidget))
	UCraftingRecipeListWidget* RecipeList;

	UPROPERTY(meta = (BindWidget))
	UButton* CraftButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

private:
	UFUNCTION()
	void OnCraftButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnStartCrafting(const UCraftingBaseRecipe* WithRecipe);

	UFUNCTION()
	void OnEndCrafting(const UCraftingBaseRecipe* WithRecipe);

	void OnRecipeSelected();

	void UpdateCraftButtonEnable();

	UPROPERTY()
	UCraftingComponent* CachedCraftingComponent;
};
