// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestCheck.h"
#include "Types/QuestTypes.h"
#include "QuestCheck_QuestsInCompletedList.generated.h"

/**
 * Are quests in completed list in the quest log check.
 */
UCLASS()
class QUESTSYSTEM_API UQuestCheck_QuestsInCompletedList : public UQuestCheck
{
	GENERATED_BODY()
	
public:
	virtual bool IsChecked(const UQuestLogComponent* InQuestLog) const override;
	
protected:
	// All this quests must be in the complete list in the player QuestLog.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checker Settings")
	TArray<UQuest*> RequireQuests;

	// Should filter by completion type.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checker Settings")
	bool bShouldFilterByCompletionType = false;

	// Filter by complete type.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checker Settings", meta = (EditCondition = "bShouldFilterByCompletionType", EditConditionHides))
	EQuestCompletionType FilterCompletionType = EQuestCompletionType::Success;

private:
	/**
	 * Gets complete quests from Quest Log based on completed type filter.
	 *
	 * @param InQuestLog			Quest log from what we want to get completed quests.
	 * @param OutCompletedQuests	[OUT] Result list of the completed quests.
	 * @return						There is at least one quest in completed quests list.
	 */
	bool GetCompletedQuests(const UQuestLogComponent* InQuestLog, TArray<UQuest*>& OutCompletedQuests) const;

	/** Checks if all the required quests are in completed quests */
	bool CheckRequiresInCompletedQuests(const TArray<UQuest*>& CompletedQuests) const;
};
