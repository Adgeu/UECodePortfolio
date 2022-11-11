// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestTrigger.h"
#include "Types/QuestTypes.h"
#include "QuestTrigger_AnyQuestsWasCompleted.generated.h"

class UQuest;

/**
 * Enters in triggered state when any other quest in QuestsForTracking list is completed.
 */
UCLASS()
class QUESTSYSTEM_API UQuestTrigger_AnyQuestsWasCompleted : public UQuestTrigger
{
	GENERATED_BODY()

protected:
	virtual void OnTrackingBegins_Implementation() override;
	virtual void OnTrackingEnded_Implementation() override;
	
	// The quest template that we want to follow.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition Settings")
	TArray<UQuest*> QuestsForTracking;

	// Should filter by completion type.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition Settings")
	bool bShouldFilterByCompletionType = false;
	
	// Filter by complete type.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition Settings")
	EQuestCompletionType FilterCompletionType = EQuestCompletionType::Success;

private:
	UFUNCTION()
	void OnQuestCompleted(UQuest* InQuest);
};
