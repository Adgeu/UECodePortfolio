// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingRecipeRowWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UBorder;
class UCraftingRecipeRowWidget;

class UCraftingBaseRecipe;

DECLARE_DELEGATE_OneParam(FOnCraftingRecipeRowClicked, UCraftingRecipeRowWidget* /*Sender*/);

/**
 * Widget for recipe list row. (TEMP!)
 */
UCLASS()
class TECHNODARKNESS_API UCraftingRecipeRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	
	void Select();
	void Deselect();
	
	void SetRecipe(const UCraftingBaseRecipe* Recipe_In);

	const UCraftingBaseRecipe* GetRecipe() const
	{
		return Recipe;
	}

	FOnCraftingRecipeRowClicked OnCraftingRecipeRowClicked;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	UBorder* RowBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selection Appearence")
	FLinearColor NormalColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selection Appearence")
	FLinearColor SelectedColor;

private:
	UFUNCTION()
	void OnButtonClicked();
	
	UPROPERTY()
	const UCraftingBaseRecipe* Recipe;
};
