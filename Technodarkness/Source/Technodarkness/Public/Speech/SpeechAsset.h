// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SpeechTypes.h"
#include "UObject/Object.h"
#include "SpeechAsset.generated.h"

DECLARE_DELEGATE_OneParam(FOnSpeechPhraseStartInternal, const FSpeechPhrase& /*SpeechPhrase*/);
DECLARE_DELEGATE(FOnSpeechPhraseEndInternal);
DECLARE_DELEGATE(FOnSpeechEndInternal);

/**
 * Speech asset contains speech phrases and functionality for playing this phrases.
 * Use GameplayTag for identify speech asset.
 * @see FSpeechPhrase
 */
UCLASS(BlueprintType)
class TECHNODARKNESS_API USpeechAsset : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Play speech asset from the first phrase.
	 * @param InWorld World.
	 * @param InFirstDelay Delay time before playing first speech phrase.
	 */
	void PlayFromStart(UWorld* InWorld, float InFirstDelay = 0.0f);

	/**
	 * Play speech asset from current phrase.
	 * @param InWorld World.
	 */
	void PlayFromCurrentPhrase(UWorld* InWorld);

	/** Returns speech asset tag. */
	FORCEINLINE const FGameplayTag& GetSpeechAssetTag() const
	{
		return SpeechAssetTag;
	}

	// On phrase start playing.
	FOnSpeechPhraseStartInternal OnSpeechPhraseStartInternal;
	
	// On phrase end playing.
	FOnSpeechPhraseEndInternal OnSpeechPhraseEndInternal;
	
	// On last phrase end playing.
	FOnSpeechEndInternal OnSpeechEndInternal;
	
protected:
	// Assigned speech asset tag for identifying.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speech: Settings")
	FGameplayTag SpeechAssetTag;

	// General delay between phrases.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speech: Settings", meta = (ClampMin = 0.0, Units = "s"))
	float GeneralDelay = 0.0f;

	// Speech phrases data table.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speech: Phrases")
	UDataTable* SpeechPhrasesDataTable;

	// Speech phrases array.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speech: Phrases", meta = (EditCondition = "SpeechPhrasesDataTable == nullptr"))
	TArray<FSpeechPhrase> SpeechPhrases;

private:
	/** On speech phrase end. */
	void OnSpeechPhraseEnd(UWorld* InWorld);

	/**
	 * Trying to get current speech phrase.
	 * If there is a SpeechPhrasesDataTable we getting speech phrase from this one, otherwise we getting speech phrase from SpeechPhrases array.
	 * 
	 * @param SpeechPhrase [OUT] Current speech phrase.
	 * @return Successful getting speech phrase.
	 * @see SpeechPhrasesDataTable, SpeechPhrases
	 */
	bool TryGetCurrentSpeechPhrase(OUT FSpeechPhrase& SpeechPhrase) const;

	/** Returns count of speech phrases (from table or array). */
	FORCEINLINE uint32 GetSpeechPhrasesDataCount() const
	{
		return SpeechPhrasesDataTable ? SpeechPhrasesDataTable->GetRowNames().Num() : SpeechPhrases.Num();
	}

	/** Has any speech phrases (in table or array). */
	FORCEINLINE bool HasAnySpeechPhrasesData() const
	{
		return GetSpeechPhrasesDataCount() > 0;
	}

	/** Is CurrentSpeechPhraseIndex in bounds (in table or array). */
	FORCEINLINE bool IsCurrentSpeechPhraseIndexInBounds() const
	{
		return CurrentSpeechPhraseIndex < GetSpeechPhrasesDataCount();
	}

	// Current speech phrase index.
	uint32 CurrentSpeechPhraseIndex = 0;

	// Speech timer handle.
	FTimerHandle SpeechTimer;
};
