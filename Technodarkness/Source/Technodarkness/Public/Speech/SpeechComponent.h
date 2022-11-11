// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SpeechTypes.h"
#include "Components/ActorComponent.h"
#include "SpeechComponent.generated.h"

class USpeechAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeechAssetStart, const USpeechAsset*, SpeechAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeechSpeechEnd, const USpeechAsset*, SpeechAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeechPhraseStart, const FSpeechPhrase&, SpeechPhrase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpeechPhraseEnd);

/**
 * Component for containing and playing speech assets.
 * Use GameplayTag for searching and playing speech asset that you need.
 *
 * @note Actors with SpeechComponent must implement ISpeechInterface.
 *
 * @see USpeechAsset, ISpeechInterface
 */
UCLASS(ClassGroup=(Custom), Category = "Speech", meta=(BlueprintSpawnableComponent))
class TECHNODARKNESS_API USpeechComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Find speech asset with tag and play it.
	 * The speech asset should be in the SpeechAssets array.
	 * 
	 * @param SpeechAssetTag Speech asset tag that we want to play.
	 * @param InFirstDelay Delay time before playing first speech phrase.
	 * 
	 * @see SpeechAssets
	 */
	UFUNCTION(BlueprintCallable, Category = "Speech")
	void PlaySpeechAssetWithTag(FGameplayTag SpeechAssetTag, float InFirstDelay = 0.0f);

	/** If current speech is playing, stop it. */
	UFUNCTION(BlueprintCallable, Category = "Speech")
	void StopCurrentSpeechAsset();

	/** Is speech asset with tag playing. */
	UFUNCTION(BlueprintPure, Category = "Speech")
	bool IsSpeechAssetPlaying(FGameplayTag SpeechAssetTag);

	/** Is any speech asset playing. */
	UFUNCTION(BlueprintPure, Category = "Speech")
	bool IsAnySpeechAssetPlaying() const;

	// On speech asset start playing.
	UPROPERTY(BlueprintAssignable)
	FOnSpeechAssetStart OnSpeechAssetStart;

	// On speech asset end playing.
	UPROPERTY(BlueprintAssignable)
	FOnSpeechSpeechEnd OnSpeechAssetEnd;

	// On speech phrase start playing.
	UPROPERTY(BlueprintAssignable)
	FOnSpeechPhraseStart OnSpeechPhraseStart;

	// On speech phrase start playing.
	UPROPERTY(BlueprintAssignable)
	FOnSpeechPhraseEnd OnSpeechPhraseEnd;
	
protected:
	virtual void BeginPlay() override;
	
	// Array of the available speech assets.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speech")
	TArray<USpeechAsset*> SpeechAssets;

private:
	/**
	 * Play speech asset from first phrase
	 * @param SpeechAsset Speech asset that we want to play.
	 * @param InFirstDelay Delay time before playing first speech phrase.
	 */
	void PlaySpeechAsset(USpeechAsset* SpeechAsset, float InFirstDelay);

	/** On phrase start playing. */
	void OnSpeechPhraseStart_Internal(const FSpeechPhrase& SpeechPhrase);
	/** On phrase end playing. */
	void OnSpeechPhraseEnd_Internal();

	/** Returns speech asset from SpeechAssets array with tag or null if it is not found. */
	USpeechAsset* GetSpeechAssetWithTag(const FGameplayTag& SpeechAssetTag);

	// Current playing speech asset.
	UPROPERTY()
	USpeechAsset* CurrentSpeechAsset = nullptr;
};
