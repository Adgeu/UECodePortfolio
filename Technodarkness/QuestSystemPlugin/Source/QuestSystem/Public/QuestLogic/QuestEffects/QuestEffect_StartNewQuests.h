// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestEffect.h"
#include "QuestEffect_StartNewQuests.generated.h"

/**
 * Starts quests from the list.
 */
UCLASS()
class QUESTSYSTEM_API UQuestEffect_StartNewQuests : public UQuestEffect
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect() override;
	
protected:
	// List of quests that we want to start.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Result Settings")
	TArray<UQuest*> QuestsToStart;
};
