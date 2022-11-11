// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "UI/Widgets/Crafting/CraftingWidget.h"

#include "Components/Button.h"
#include "CraftingSystem/Components/CraftingComponent.h"
#include "UI/Widgets/Crafting/CraftingRecipeListWidget.h"

void UCraftingWidget::SetCraftingComponent(UCraftingComponent* CraftingComponent)
{
	check(CraftingComponent);
	CachedCraftingComponent = CraftingComponent;
	CachedCraftingComponent->OnCraftingBegins.AddDynamic(this, &UCraftingWidget::OnStartCrafting);
	CachedCraftingComponent->OnCraftingEnded.AddDynamic(this, &UCraftingWidget::OnEndCrafting);
}

void UCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CraftButton->OnClicked.AddDynamic(this, &UCraftingWidget::OnCraftButtonClicked);
	ExitButton->OnClicked.AddDynamic(this, &UCraftingWidget::OnExitButtonClicked);
	RecipeList->OnRecipeSelected.BindUObject(this, &UCraftingWidget::OnRecipeSelected);
}

void UCraftingWidget::RefreshRecipesList()
{
	RecipeList->RefreshRecipesList(CachedCraftingComponent);
}

void UCraftingWidget::OnCraftButtonClicked()
{
	CachedCraftingComponent->StartCrafting(RecipeList->GetSelectedRecipe());
}

void UCraftingWidget::OnExitButtonClicked()
{
	CachedCraftingComponent->ExitCraftingMode();
}

void UCraftingWidget::OnStartCrafting(const UCraftingBaseRecipe* WithRecipe)
{
	CraftButton->SetIsEnabled(false);
	ExitButton->SetIsEnabled(false);
	RecipeList->SetIsEnabled(false);
}

void UCraftingWidget::OnEndCrafting(const UCraftingBaseRecipe* WithRecipe)
{
	UpdateCraftButtonEnable();
	ExitButton->SetIsEnabled(true);
	RecipeList->SetIsEnabled(true);
}

void UCraftingWidget::OnRecipeSelected()
{
	UpdateCraftButtonEnable();
}

void UCraftingWidget::UpdateCraftButtonEnable()
{
	const UCraftingBaseRecipe* SelectedRecipe = RecipeList->GetSelectedRecipe();
	const bool bEnabled = SelectedRecipe && CachedCraftingComponent->CanCraft(SelectedRecipe);
	UE_LOG(LogTemp, Display, TEXT("Button enbled: %d"), bEnabled);
	CraftButton->SetIsEnabled(bEnabled);
}
