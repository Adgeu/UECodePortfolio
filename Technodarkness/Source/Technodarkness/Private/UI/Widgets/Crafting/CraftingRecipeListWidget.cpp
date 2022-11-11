// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "UI/Widgets/Crafting/CraftingRecipeListWidget.h"

#include "Components/ScrollBox.h"
#include "CraftingSystem/Components/CraftingComponent.h"
#include "UI/Widgets/Crafting/CraftingRecipeRowWidget.h"
#include "UI/Widgets/Crafting/CraftingSelectedRecipeWidget.h"

void UCraftingRecipeListWidget::RefreshRecipesList(const UCraftingComponent* InCraftingComponent)
{
	if (!InCraftingComponent)
	{
		UE_LOG(LogCrafting, Error, TEXT("%s() Crafting component is null!"), *FString(__FUNCTION__));
		return;
	}

	check(CraftingRecipeRowWidgetClass);

	RecipesBox->ClearChildren();
	for (const UCraftingBaseRecipe* Recipe : InCraftingComponent->GetRecipesByWorkspaceType())
	{
		UCraftingRecipeRowWidget* RecipeRow = CreateWidget<UCraftingRecipeRowWidget>(RecipesBox, CraftingRecipeRowWidgetClass);
		RecipeRow->SetRecipe(Recipe);
		RecipeRow->OnCraftingRecipeRowClicked.BindUObject(this, &UCraftingRecipeListWidget::OnRecipeRowClicked);
		RecipesBox->AddChild(RecipeRow);
	}
}

const UCraftingBaseRecipe* UCraftingRecipeListWidget::GetSelectedRecipe() const
{
	return SelectedRecipe ? SelectedRecipe->GetRecipe() : nullptr;
}

void UCraftingRecipeListWidget::OnRecipeRowClicked(UCraftingRecipeRowWidget* Sender)
{
	if (CurrentSelectedRecipeRow == Sender)
	{
		return;
	}
	
	if (CurrentSelectedRecipeRow)
	{
		CurrentSelectedRecipeRow->Deselect();
	}

	if (SelectedRecipe->GetVisibility() == ESlateVisibility::Hidden)
	{
		SelectedRecipe->SetVisibility(ESlateVisibility::Visible);
	}

	CurrentSelectedRecipeRow = Sender;
	CurrentSelectedRecipeRow->Select();

	SelectedRecipe->SetRecipe(CurrentSelectedRecipeRow->GetRecipe());

	OnRecipeSelected.ExecuteIfBound();
}
