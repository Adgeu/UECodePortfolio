// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestLogComponent.generated.h"

class UQuest;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewQuestAddedSignature, UQuest*, NewQuest);

/**
 * Quest log manages quests.
 * Contains quests that player has taken and completed quests.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestLogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// New Quest was added.
	UPROPERTY(BlueprintAssignable)
	FOnNewQuestAddedSignature OnNewQuestAddedDelegate;

public:
	/**
	 * Add new quest to the active quest list.
	 * @param QuestTemplate Template of the quest that we want to add.
	 * @return We have successfully added the quest.
	 */
	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName="Success"))
	bool AddNewQuest(const UQuest* QuestTemplate);

	/**
	 * Can we add and start the quest to the Quest Log based on quest requirements.
	 * @param Quest Quest that we want to check.
	 * @return Can we add and start quest or not.
	 */
	UFUNCTION(BlueprintPure)
	bool CanStartQuest(const UQuest* Quest) const;

	/**
	 * Returns quest from the active quests list that matches quest template.
	 * @param QuestTemplate Template of the quest that we want to get.
	 * @return Quest from the active quests list that matches template, or nullptr if there is no such quest.
	 */
	UFUNCTION(BlueprintPure)
	UQuest* GetActiveQuestByTemplate(const UQuest* QuestTemplate) const;

	/**
	 * Returns quest from the completed quests list that matches quest template.
	 * @param QuestTemplate Template of the quest that we want to get.
	 * @return Quest from the completed quests list that matches template, or nullptr if there is no such quest.
	 */
	UFUNCTION(BlueprintPure)
	UQuest* GetCompletedQuestByTemplate(const UQuest* QuestTemplate) const;

	/** Returns completed quests that marked as Successful. */
	UFUNCTION(BlueprintPure)
	TArray<UQuest*> GetSuccessfulCompletedQuests() const;

	/** Returns completed quests that marked as Failed. */
	UFUNCTION(BlueprintPure)
	TArray<UQuest*> GetFailedCompletedQuests() const;

	/** Checks if the quest is in the active quests list. */
	UFUNCTION(BlueprintPure)
	bool IsQuestActive(const UQuest* Quest) const;

	/** Checks if the quest is in the completed quests list. */
	UFUNCTION(BlueprintPure)
	bool IsQuestCompleted(const UQuest* Quest) const;

	/** Checks if all quests from the list are in the active quests list. */
	UFUNCTION(BlueprintPure)
	bool AreQuestsActive(const TArray<UQuest*>& Quests) const;
	
	/** Checks if all quests from the list are in the completed quests list. */
	UFUNCTION(BlueprintPure)
	bool AreQuestsCompleted(const TArray<UQuest*>& Quests) const;

	/** Returns active quests. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<UQuest*> GetActiveQuests() const
	{
		return ActiveQuests;
	}

	/** Returns completed quests. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<UQuest*> GetCompletedQuests() const
	{
		return CompletedQuests;
	}
	
private:
	/** One of the active quests was completed. */
	UFUNCTION()
	void OnQuestCompleted(UQuest* Quest);

	// The list of active quests.
	UPROPERTY()
	TArray<UQuest*> ActiveQuests;

	// The list of completed quests.
	UPROPERTY()
	TArray<UQuest*> CompletedQuests;
};
