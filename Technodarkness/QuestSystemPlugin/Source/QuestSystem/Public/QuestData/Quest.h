// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestObjective.h"
#include "Types/QuestTypes.h"
#include "UObject/Object.h"
#include "Quest.generated.h"

class UQuestSchema;
class UQuestLogComponent;
class UQuestObjective;
class UQuestCheck;
class UQuestEffect;

/**
 * Quest asset.
 *
 * Create this asset only in editor and give it to the QuestLog.
 * @see UQuestLogComponent
 */
UCLASS(BlueprintType)
class QUESTSYSTEM_API UQuest : public UObject
{
	GENERATED_BODY()

public:
	UQuest();

	///////////////////////////////////////////////////////

#pragma region Quest Delegates
	
public:
	/**
	 * The quest was started.
	 * @param Quest				The quest that was started.
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestBeginsSignature, UQuest*, Quest);

	/**
	 * The quest was completed.
	 * @param Quest				The quest that was completed.
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestEndedSignature, UQuest*, Quest);

	/**
	 * The objective of the quest was started.
	 * @param InQuest			The quest in which the objective began.
	 * @param Objective			The objective that was started.
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveBeginsSignature, UQuest*, InQuest, UQuestObjective*, Objective);

	/**
	 * The objective of the quest was completed.
	 * @param InQuest			The quest in which the objective ended.
	 * @param Objective			The objective that was completed.
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveEndedSignature, UQuest*, InQuest, UQuestObjective*, Objective);

	// The quest was started.
	UPROPERTY(BlueprintAssignable)
	FOnQuestBeginsSignature OnQuestBeginsDelegate;

	// The quest was completed.
	UPROPERTY(BlueprintAssignable)
	FOnQuestEndedSignature OnQuestEndedDelegate;

	// The objective was started.
	UPROPERTY(BlueprintAssignable)
	FOnObjectiveBeginsSignature OnObjectiveBeginsDelegate;

	// The objective was completed.
	UPROPERTY(BlueprintAssignable)
	FOnObjectiveEndedSignature OnObjectiveEndedDelegate;

#pragma endregion ~Quest Delegates

	///////////////////////////////////////////////////////

#pragma region Quest Getters

public:
	/** Returns quest log component that owns this quest. We can get quest log only when we added quest to the quest log. */
	UQuestLogComponent* GetQuestLogComponent() const;
	
	/** Returns quest description. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE UTexture2D* GetDescription(const FString& LocalizationKey) const
	{
		return Description[LocalizationKey].LoadSynchronous();
	}
	
	/** Returns textures of the lore by localization key. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<FLoreDisplayingElement> GetLoreDisplayingElements(const FString& LocalizationKey) const
	{
		return LoreContainer.GetDisplayingElements(LocalizationKey);
	}
	
	/** Returns texture of the lore by localization key and index. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FLoreDisplayingElement GetLoreDisplayingElementByIndex(const FString& LocalizationKey, int32 ElementIndex) const
	{
		return LoreContainer.GetDisplayingElement(LocalizationKey, ElementIndex);
	}
	
	/** Returns current quest state. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE EQuestState GetCurrentState() const
	{
		return CurrentState;
	}

	/** Returns completion type (None if quest is not completed). */
	UFUNCTION(BlueprintPure)
	FORCEINLINE EQuestCompletionType GetCompletionType() const
	{
		return CompletionType;
	}

	/** Returns quest schema. */
	FORCEINLINE UQuestSchema* GetQuestSchema() const
	{
		return QuestSchema;
	}

	/** Returns quest unique name. */
	FORCEINLINE FName GetQuestUniqueName() const
	{
		return QuestUniqueName;
	}

#pragma endregion ~Quest Getters
	
	///////////////////////////////////////////////////////
	
public:
	/** Starts running quest. */
	void StartQuest();

	/** Can we add quest to the player QuestLogComponent and then start it.  */
	bool CanStartQuest(const UQuestLogComponent* InQuestLog) const;

	/** Completes the quest with complete type. */
	void CompleteQuest(EQuestCompletionType WithCompletionType);

	/** Completes all active objectives with completion type. */
	void CompleteActiveObjectives(EQuestCompletionType WithCompletionType);

	/** Objective was begin. */
	void OnObjectiveBegins(UQuestObjective* Objective);
	/** Objective was ended. */
	void OnObjectiveEnded(UQuestObjective* Objective);

protected:
	// Name of the quest. This should be unique for quest identification.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Settings")
	FName QuestUniqueName;

#if WITH_EDITORONLY_DATA
	// URL of the quest document.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Settings")
	FDocURL QuestDocURL;
#endif
	
	// The category of the quest.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Settings")
	EQuestCategory QuestCategory = EQuestCategory::None;

	// Quest description map (Key: localization key, Value: quest description texture).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Story")
	TMap<FString, TSoftObjectPtr<UTexture2D>> Description;
	
	// Container for lore textures with localization keys.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Story")
	FLoreContainer LoreContainer;
	
private:
	// Schema of the quest.
	UPROPERTY()
	UQuestSchema* QuestSchema;

	// List of objectives that currently being performed by the player.
	UPROPERTY()
	TArray<UQuestObjective*> ActiveObjectives;

	// Current quest state.
	EQuestState CurrentState = EQuestState::Inactive;
	
	// Quest complete type (None if the quest is not completed).
	EQuestCompletionType CompletionType = EQuestCompletionType::None;

#if WITH_EDITORONLY_DATA
public:
	// Cached quest graph for quest editor.
	UPROPERTY()
	UEdGraph* QuestGraph;
#endif
};
