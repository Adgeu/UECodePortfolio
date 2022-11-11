// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestLogic/QuestEffects/QuestEffect.h"
#include "QuestEffect_PlaySpeech.generated.h"

/**
 * Plays speech asset.
 */
UCLASS()
class TECHNODARKNESS_API UQuestEffect_PlaySpeech : public UQuestEffect
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect() override;
	
protected:
	// Actor that should play speech asset. Actor must implement IQuestTarget interface.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Settings")
	FGameplayTag ActorWithSpeechTargetId;

	// Tag of the speech asset that we want to play.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Settings")
	FGameplayTag SpeechToPlayTag;
};
