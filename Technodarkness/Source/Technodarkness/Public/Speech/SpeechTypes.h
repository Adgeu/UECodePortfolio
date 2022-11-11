// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "Engine/DataTable.h"
#include "SpeechTypes.generated.h"

///////////////////////////////////////////////////////

// Speech log category.
DEFINE_LOG_CATEGORY_STATIC(LogSpeech, All, All);

///////////////////////////////////////////////////////

/** Phrase duration method. */
UENUM(BlueprintType)
enum class ESpeechPhraseDuration : uint8
{
	// Set phrase duration manually.
	Manual,
	
	// Set phrase duration based on sound duration (sound must not be null!)
	FromSound
};

///////////////////////////////////////////////////////

/** One speech phrase. */
USTRUCT(BlueprintType)
struct FSpeechPhrase
{
	GENERATED_BODY()

	// Text of the speech.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Text;

	// Sound of the speech.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> Sound;

	// Duration method. Sets the time to play the next phrase.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESpeechPhraseDuration DurationMethod = ESpeechPhraseDuration::FromSound;

	// Manual phrase duration.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "DurationMethod == ESpeechPhraseDuration::Manual", ClampMin = 0.0, Units = "s"))
	float Duration = 0.0f;

	/** Returns duration based on phrase duration method. */
	float GetDuration() const;
};

///////////////////////////////////////////////////////

/** Row for speech phrase. */
USTRUCT(BlueprintType)
struct FSpeechPhraseRow : public FTableRowBase
{
	GENERATED_BODY()

	// Text of the speech.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Text;

	// Sound of the speech.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> Sound;

	// Duration method. Sets the time to play the next phrase.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESpeechPhraseDuration DurationMethod = ESpeechPhraseDuration::FromSound;

	// Manual phrase duration.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "DurationMethod == ESpeechPhraseDuration::Manual", EditConditionHides, ClampMin = 0.0, Units = "s"))
	float ManualDuration = 0.0f;
};
