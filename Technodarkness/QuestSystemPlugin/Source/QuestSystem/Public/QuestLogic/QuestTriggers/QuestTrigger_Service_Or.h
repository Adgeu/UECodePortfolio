// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestTrigger.h"
#include "QuestTrigger_Service_Or.generated.h"

/**
 * Enters in triggered state if one of the triggers contained in this trigger is entering in triggered state.
 */
UCLASS(DisplayName = "!Or")
class QUESTSYSTEM_API UQuestTrigger_Service_Or : public UQuestTrigger
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 IndentLevel) const override;
	
protected:
	virtual void OnTrackingBegins_Implementation() override;
	virtual void OnTrackingEnded_Implementation() override;

	virtual void OnUndoTriggeredState_Implementation() override;
	
	// List of triggers.
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Trigger Settings")
	TArray<UQuestTrigger*> Triggers;

private:
	void OnOtherTriggerChangeState(const UQuestTrigger* InTrigger, bool bNewTriggeredState);
};
