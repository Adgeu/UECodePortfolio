// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestSystem/QuestLogic/QuestTriggers/QuestTrigger_CraftEnded.h"

#include "Characters/TDPlayerCharacter.h"
#include "CraftingSystem/Components/CraftingComponent.h"
#include "Kismet/GameplayStatics.h"

void UQuestTrigger_CraftEnded::OnTrackingBegins_Implementation()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	checkf(PlayerCharacter->IsA<ATDPlayerCharacter>(), TEXT("%s() Player character must be TDPlayerCharacter!"), *FString(__FUNCTION__));
	CraftingComponent = StaticCast<ATDPlayerCharacter*>(PlayerCharacter)->GetCraftingComponent();

	check(CraftingComponent);
	CraftingComponent->OnCraftingEnded.AddDynamic(this, &UQuestTrigger_CraftEnded::OnCraftEnded);
}

void UQuestTrigger_CraftEnded::OnTrackingEnded_Implementation()
{
	CraftingComponent->OnCraftingEnded.RemoveDynamic(this, &UQuestTrigger_CraftEnded::OnCraftEnded);
}

void UQuestTrigger_CraftEnded::OnCraftEnded(const UCraftingBaseRecipe* WithRecipe)
{
	if (!Recipe || Recipe == WithRecipe)
	{
		EnterTriggeredState();
	}
}
