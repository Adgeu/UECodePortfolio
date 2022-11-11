// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "UI/Widgets/Crafting/CraftingRecipeRowWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CraftingSystem/Recipes/CraftingBaseRecipe.h"

void UCraftingRecipeRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RowButton->OnClicked.AddDynamic(this, &UCraftingRecipeRowWidget::OnButtonClicked);
}

void UCraftingRecipeRowWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	RowBorder->SetBrushColor(NormalColor);
}

void UCraftingRecipeRowWidget::Select()
{
	RowBorder->SetBrushColor(SelectedColor);
}

void UCraftingRecipeRowWidget::Deselect()
{
	RowBorder->SetBrushColor(NormalColor);
}

void UCraftingRecipeRowWidget::SetRecipe(const UCraftingBaseRecipe* Recipe_In)
{
	check(Recipe_In);
	Recipe = Recipe_In;

	NameText->SetText(Recipe->GetDisplayingName());
	IconImage->SetBrushFromTexture(Recipe->GetDisplayingIcon());
}

void UCraftingRecipeRowWidget::OnButtonClicked()
{
	OnCraftingRecipeRowClicked.ExecuteIfBound(this);
}
