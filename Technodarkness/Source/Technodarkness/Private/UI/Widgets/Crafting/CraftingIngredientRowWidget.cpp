// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "UI/Widgets/Crafting/CraftingIngredientRowWidget.h"

#include "Components/TextBlock.h"
#include "CraftingSystem/Interfaces/CraftingIngredientItem.h"

void UCraftingIngredientRowWidget::SetIngredient(const FCraftingIngredient& Ingredient)
{
	const FCraftingIngredientInfo IngredientInfo = ICraftingIngredientItem::Execute_GetCraftingIngredientInfo(Ingredient.IngredientClass.GetDefaultObject());
	NameText->SetText(IngredientInfo.DisplayName);
	AmountText->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Ingredient.Amount)));
}
