// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestEffect.h"
#include "Types/QuestTypes.h"
#include "QuestEffect_CompleteQuest.generated.h"

/**
 * Complete specific quests with fail.
 */
UCLASS()
class QUESTSYSTEM_API UQuestEffect_CompleteQuest : public UQuestEffect
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect() override;

protected:
	// Quest that we want to complete with fail.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Result Settings")
	UQuest* QuestToComplete;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Result Settings")
	EQuestCompletionType CompletionType = EQuestCompletionType::Success;
};
