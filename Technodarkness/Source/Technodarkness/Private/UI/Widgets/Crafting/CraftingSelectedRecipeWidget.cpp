// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "UI/Widgets/Crafting/CraftingSelectedRecipeWidget.h"

#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "CraftingSystem/Recipes/CraftingBaseRecipe.h"
#include "UI/Widgets/Crafting/CraftingIngredientRowWidget.h"

void UCraftingSelectedRecipeWidget::SetRecipe(const UCraftingBaseRecipe* Recipe_In)
{
	check(Recipe_In);
	check(CraftingIngredientRowWidgetClass);
	
	Recipe = Recipe_In;

	NameText->SetText(Recipe->GetDisplayingName());
	DescriptionText->SetText(Recipe->GetDescription());
	IconImage->SetBrushFromTexture(Recipe->GetDisplayingIcon());

	IngredientsBox->ClearChildren();
	for (const FCraftingIngredient& Ingredient : Recipe->GetIngredients())
	{
		UCraftingIngredientRowWidget* IngredientRow = CreateWidget<UCraftingIngredientRowWidget>(this, CraftingIngredientRowWidgetClass);
		IngredientRow->SetIngredient(Ingredient);
		IngredientsBox->AddChild(IngredientRow);
	}
}
