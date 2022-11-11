// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingRecipeListWidget.generated.h"

class UScrollBox;
class UCraftingRecipeRowWidget;
class UCraftingSelectedRecipeWidget;

DECLARE_DELEGATE(FOnRecipeSelected);

/**
 * Widget for recipe list. (TEMP!)
 */
UCLASS()
class TECHNODARKNESS_API UCraftingRecipeListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void RefreshRecipesList(const class UCraftingComponent* InCraftingComponent);

	const class UCraftingBaseRecipe* GetSelectedRecipe() const;
	
	FOnRecipeSelected OnRecipeSelected;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UCraftingSelectedRecipeWidget* SelectedRecipe;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* RecipesBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCraftingRecipeRowWidget> CraftingRecipeRowWidgetClass;

private:
	void OnRecipeRowClicked(UCraftingRecipeRowWidget* Sender);

	UPROPERTY()
	UCraftingRecipeRowWidget* CurrentSelectedRecipeRow;
};
