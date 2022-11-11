// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingSelectedRecipeWidget.generated.h"

class UTextBlock;
class UImage;
class UScrollBox;

class UCraftingBaseRecipe;

/**
 * Widget for selected recipe. (TEMP!)
 */
UCLASS()
class TECHNODARKNESS_API UCraftingSelectedRecipeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetRecipe(const UCraftingBaseRecipe* Recipe_In);
	
	FORCEINLINE const UCraftingBaseRecipe* GetRecipe() const
	{
		return Recipe;
	}
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText;

	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* IngredientsBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UCraftingIngredientRowWidget> CraftingIngredientRowWidgetClass;

private:
	UPROPERTY()
	const UCraftingBaseRecipe* Recipe;
};
