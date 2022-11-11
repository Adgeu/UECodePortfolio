// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestGiverComponent.generated.h"

class UQuest;
class UQuestLogComponent;

//////////////////////////////////////////////////////////////////////

/**
 * Contains quest that we want to give and settings for specify behavior for quest give.
 */
USTRUCT(BlueprintType)
struct FQuestGiverContainer
{
	GENERATED_BODY()

	// Quest that we want to give.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UQuest* Quest;

	/**
	 * If the quest was not added to the quest log on any reason,
	 * should we go to the next quest in available quests list
	 * or stay and try to give this quest later.
	 *
	 * Works with GiveNextQuest().
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldGoToNextOnGivingFailure = false;
};

//////////////////////////////////////////////////////////////////////

/**
 * Provides functionality for give quest to the quest log component.
 * @see UQuest, UQuestLogComponent
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestGiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Gives all quests in available quests list to the Quest Log.
	 * @param ToQuestLog Quest log to which we want to give the quests.
	 * @return We successfully added at least one quest to the quest log.
	 */
	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName="Giving Successfully"))
	bool GiveAllQuests(UQuestLogComponent* ToQuestLog);
	
	/**
	 * Gives next quest in available quests list to the Quest Log.
	 * @param ToQuestLog Quest log to which we want to give the quest.
	 * @return We successfully added quest to the quest log.
	 */
	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName="Giving Successfully"))
	bool GiveNextQuest(UQuestLogComponent* ToQuestLog);

	/**
	 * Gives quest with index from available quests list to the Quest Log.
	 * @param ToQuestLog Quest log to which we want to give the quest.
	 * @param QuestIndex Index of the quest that we want to give. This must be in bounds [0, available quest num - 1].
	 * @return We successfully added quest to the quest log.
	 */
	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName="Giving Successfully"))
	bool GiveQuestWithIndex(UQuestLogComponent* ToQuestLog, int32 QuestIndex);
	
protected:
	// If TRUE when we give all quests from available list, we will give quests again from the list start.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Giving Settings")
	bool bLoopQuestGiving = false;

	// List of the available quests that this giver can give.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quests", meta = (TitleProperty = "Quest"))
	TArray<FQuestGiverContainer> AvailableQuests;

private:
	// Current given quest index. Works with GiveNextQuest().
	int32 CurrentQuestIndex = -1;
};
