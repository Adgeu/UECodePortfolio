// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestLogic/QuestTriggers/QuestTrigger.h"
#include "QuestTrigger_SpeechEnded.generated.h"

/**
 * Enters in triggered state when the speech asset was ended.
 */
UCLASS()
class TECHNODARKNESS_API UQuestTrigger_SpeechEnded : public UQuestTrigger
{
	GENERATED_BODY()

protected:
	virtual void OnTrackingBegins_Implementation() override;
	virtual void OnTrackingEnded_Implementation() override;
	
	// Actor that plays speech asset that we need. Actor must implement IQuestTarget interface.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Settings")
	FGameplayTag ActorWithSpeechTargetId;

	// Speech asset tag that we want to track.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Settings")
	FGameplayTag SpeechTag;

private:
	UFUNCTION()
	void OnSpeechAssetEnded(const class USpeechAsset* SpeechAsset);

	UPROPERTY()
	class USpeechComponent* SpeechComponent;
};
