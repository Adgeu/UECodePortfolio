// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestTrigger.h"
#include "QuestTrigger_Service_AndInOrder.generated.h"

/**
 * Enters in triggered state if all the conditions contained in this trigger are entering in triggered state in specific order.
 * While first trigger in list is not in triggered state, second trigger is not tracking and etc.
 */
UCLASS(DisplayName = "!And In Order")
class QUESTSYSTEM_API UQuestTrigger_Service_AndInOrder : public UQuestTrigger
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 IndentLevel) const override;

protected:
	virtual void OnTrackingBegins_Implementation() override;
	virtual void OnTrackingEnded_Implementation() override;

	virtual void OnUndoTriggeredState_Implementation() override;
	
	// List of triggers . Order is important.
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Trigger Settings")
	TArray<UQuestTrigger*> Triggers;

private:
	void OnOtherTriggerChangeState(const UQuestTrigger* InTrigger, bool bNewTriggeredState);

	void HandleOtherTriggerOnUndoTriggered(const UQuestTrigger* InTrigger);
	
	void TryTrackNextOrExit();

	// Index of the trigger that we currently tracking.
	int32 CurrentTrackingTriggerIndex = -1;

	bool bShouldHandleOthersUndoTriggered = true;
};
