// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestTrigger.h"
#include "QuestTrigger_Service_And.generated.h"

/**
 * Enters in triggered state if all the triggers contained in this trigger are entering in triggered state.
 */
UCLASS(DisplayName = "!And")
class QUESTSYSTEM_API UQuestTrigger_Service_And : public UQuestTrigger
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

	uint8 TargetTriggeredCount = 0;
	uint8 TriggeredCount = 0;

	bool bShouldHandleOthersUndoTriggered = true;
};
