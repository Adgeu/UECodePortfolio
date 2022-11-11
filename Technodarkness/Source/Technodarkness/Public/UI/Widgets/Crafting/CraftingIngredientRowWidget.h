// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingIngredientRowWidget.generated.h"

class UTextBlock;

/**
 * Widget for ingredient row. (TEMP!)
 */
UCLASS()
class TECHNODARKNESS_API UCraftingIngredientRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetIngredient(const struct FCraftingIngredient& Ingredient);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmountText;
};
