// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestLogic/QuestTriggers/QuestTrigger.h"
#include "QuestTrigger_CraftEnded.generated.h"

class UCraftingBaseRecipe;
class UCraftingComponent;

/**
 * Enters in triggered state when something was crafted by player.
 */
UCLASS()
class TECHNODARKNESS_API UQuestTrigger_CraftEnded : public UQuestTrigger
{
	GENERATED_BODY()

protected:
	virtual void OnTrackingBegins_Implementation() override;
	virtual void OnTrackingEnded_Implementation() override;
	
	// Specify recipe that we want to track. Leave it empty, if we want to track any crafting.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Settings")
	UCraftingBaseRecipe* Recipe;

private:
	UFUNCTION()
	void OnCraftEnded(const UCraftingBaseRecipe* WithRecipe);
	
	UPROPERTY()
	UCraftingComponent* CraftingComponent;
};
