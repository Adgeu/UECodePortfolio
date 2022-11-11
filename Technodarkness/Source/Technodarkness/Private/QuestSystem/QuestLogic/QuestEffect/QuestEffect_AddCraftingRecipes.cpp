// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/QuestLogic/QuestEffect/QuestEffect_AddCraftingRecipes.h"

#include "Characters/TDPlayerCharacter.h"
#include "CraftingSystem/Components/CraftingComponent.h"
#include "CraftingSystem/Recipes/CraftingBaseRecipe.h"
#include "Types/QuestTypes.h"

void UQuestEffect_AddCraftingRecipes::ApplyEffect()
{
	Super::ApplyEffect();

	if (const ATDPlayerCharacter* PlayerCharacter = Cast<ATDPlayerCharacter>(GetPlayerCharacter()))
	{
		UCraftingComponent* CraftingComponent = PlayerCharacter->GetCraftingComponent();
		for (UCraftingBaseRecipe* Recipe : RecipesToAdd)
		{
			if (Recipe)
			{
				CraftingComponent->AddCraftingRecipe(Recipe);
			}
		}
	}
	else
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Player character is not valid!"), *FString(__FUNCTION__));
	}
}
