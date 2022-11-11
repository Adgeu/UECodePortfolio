// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestTrigger.h"
#include "QuestTrigger_TimeElapsed.generated.h"

/**
 * Enters in triggered state after the time has elapsed.
 */
UCLASS()
class QUESTSYSTEM_API UQuestTrigger_TimeElapsed : public UQuestTrigger
{
	GENERATED_BODY()

protected:
	virtual void OnTrackingBegins_Implementation() override;
	virtual void OnTrackingEnded_Implementation() override;

	virtual void OnUndoTriggeredState_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Settings", meta = (ClampMin = 0.0, ForceUnits = "s"))
	float Time = 0.0f;

private:
	FTimerHandle TimerHandle;
};
