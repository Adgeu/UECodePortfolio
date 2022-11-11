// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestCheck.h"
#include "QuestCheck_QuestsInActiveList.generated.h"

/**
 * Are quests in active list in the quest log check.
 */
UCLASS()
class QUESTSYSTEM_API UQuestCheck_QuestsInActiveList : public UQuestCheck
{
	GENERATED_BODY()

public:
	virtual bool IsChecked(const UQuestLogComponent* InQuestLog) const override;
	
protected:
	// All this quests must be in the available list in the player QuestLog.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checker Settings")
	TArray<UQuest*> RequireQuests;
};
