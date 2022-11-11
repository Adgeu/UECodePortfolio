// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestLogic/QuestEffects/QuestEffect.h"
#include "QuestEffect_AddCraftingRecipes.generated.h"

/**
 * Add new crafting recipe to the player crafting component.
 */
UCLASS()
class TECHNODARKNESS_API UQuestEffect_AddCraftingRecipes : public UQuestEffect
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect() override;

protected:
	// List of the recipes that we want to add to the player crafting component.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Settings")
	TArray<class UCraftingBaseRecipe*> RecipesToAdd;
};
