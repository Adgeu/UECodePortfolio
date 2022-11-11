// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Types/QuestTypes.h"
#include "UObject/Object.h"
#include "QuestObjective.generated.h"

class UQuestTrigger;
class UQuestEffect;
class UQuestCheck;

class UQuestObjectiveTransition;

/**
 * Objective of the quest.
 */
UCLASS(DefaultToInstanced, EditInlineNew)
class QUESTSYSTEM_API UQuestObjective : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestObjectiveEndedSignature, UQuestObjective* /*EndedObjective*/);

	// Notifies that the objective was ended.
	FOnQuestObjectiveEndedSignature OnObjectiveEndedDelegate;

	///////////////////////////////////////////////////////
	
public:
	/** Starts objective. */
	void StartObjective();

	/** Ends all completion triggers tracking. */
	void EndCompletionTriggerTrack();

	/** Ends objective with completion type. */
	void EndObjective(EQuestCompletionType WithCompletionType);
	
	/** Returns current objective state. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE EQuestState GetCurrentState() const
	{
		return CurrentState;
	}

	/** Returns quest complete type. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE EQuestCompletionType GetCompletionType() const
	{
		return CompletionType;
	}

	/** Is the objective should be hidden from outside? */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsObjectiveHidden() const
	{
		return bHiddenObjective;
	}

	/** Returns success completion trigger. */
	FORCEINLINE UQuestTrigger* GetSuccessTrigger() const
	{
		return SuccessTrigger;
	}

	/** Returns fail completion trigger. */
	FORCEINLINE UQuestTrigger* GetFailTrigger() const
	{
		return FailTrigger;
	}

protected:
	// Is the objective should be hidden from outside?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Settings")
	bool bHiddenObjective = false;
	
	// Objective description map (Key: localization key, Value: objective description texture).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective Story")
	TMap<FString, TSoftObjectPtr<UTexture2D>> Description;

	// Trigger for complete objective with success.
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Objective Completion")
	UQuestTrigger* SuccessTrigger;

	// Trigger for complete objective with fail.
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Objective Completion")
	UQuestTrigger* FailTrigger;

private:
	/** Begins all completion triggers tracking. */
	void BeginCompletionTriggerTrack();

	/** Calls when success trigger changed its triggered state. */
	void OnSuccessTriggerChangedState(const UQuestTrigger* InTrigger, bool bNewTriggeredState);
	/** Calls when fail trigger changed its triggered state. */
	void OnFailTriggerChangedState(const UQuestTrigger* InTrigger, bool bNewTriggeredState);
	
	// Current objective state.
	EQuestState CurrentState = EQuestState::Inactive;

	// Objective complete type (None if the objective is not completed).
	EQuestCompletionType CompletionType = EQuestCompletionType::None;
};
