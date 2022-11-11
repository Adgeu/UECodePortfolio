// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "Speech/SpeechComponent.h"

#include "Speech/SpeechAsset.h"
#include "Speech/SpeechInterface.h"
#include "Speech/SpeechTypes.h"

void USpeechComponent::PlaySpeechAssetWithTag(FGameplayTag SpeechAssetTag, float InFirstDelay)
{
	if (USpeechAsset* SpeechAsset = GetSpeechAssetWithTag(SpeechAssetTag))
	{
		PlaySpeechAsset(SpeechAsset, InFirstDelay);
		return;
	}
	
	UE_LOG(LogSpeech, Error, TEXT("%s() Speech asset with tag \"%s\" not found!"), *FString(__FUNCTION__), *SpeechAssetTag.ToString());
}

void USpeechComponent::StopCurrentSpeechAsset()
{
	if (!CurrentSpeechAsset)
	{
		return;
	}

	CurrentSpeechAsset->OnSpeechPhraseStartInternal.Unbind();
	CurrentSpeechAsset->OnSpeechPhraseEndInternal.Unbind();
	CurrentSpeechAsset->OnSpeechEndInternal.Unbind();

	OnSpeechAssetEnd.Broadcast(CurrentSpeechAsset);
	
	CurrentSpeechAsset = nullptr;
}

bool USpeechComponent::IsSpeechAssetPlaying(FGameplayTag SpeechAssetTag)
{
	const USpeechAsset* SpeechAsset = GetSpeechAssetWithTag(SpeechAssetTag);
	return IsValid(CurrentSpeechAsset) && CurrentSpeechAsset == SpeechAsset;
}

bool USpeechComponent::IsAnySpeechAssetPlaying() const
{
	return IsValid(CurrentSpeechAsset);
}

void USpeechComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->Implements<USpeechInterface>(), TEXT("%s() Actor with SpeechComponent must implement ISpeechInterface."), *FString(__FUNCTION__));
}

void USpeechComponent::PlaySpeechAsset(USpeechAsset* SpeechAsset, float InFirstDelay)
{
	CurrentSpeechAsset = SpeechAsset;

	CurrentSpeechAsset->OnSpeechPhraseStartInternal.BindUObject(this, &USpeechComponent::OnSpeechPhraseStart_Internal);
	CurrentSpeechAsset->OnSpeechPhraseEndInternal.BindUObject(this, &USpeechComponent::OnSpeechPhraseEnd_Internal);
	CurrentSpeechAsset->OnSpeechEndInternal.BindUObject(this, &USpeechComponent::StopCurrentSpeechAsset);

	OnSpeechAssetStart.Broadcast(CurrentSpeechAsset);
	CurrentSpeechAsset->PlayFromStart(GetWorld(), InFirstDelay);
}

void USpeechComponent::OnSpeechPhraseStart_Internal(const FSpeechPhrase& SpeechPhrase)
{
	OnSpeechPhraseStart.Broadcast(SpeechPhrase);
}

void USpeechComponent::OnSpeechPhraseEnd_Internal()
{
	OnSpeechPhraseEnd.Broadcast();
}

USpeechAsset* USpeechComponent::GetSpeechAssetWithTag(const FGameplayTag& SpeechAssetTag)
{
	USpeechAsset** FoundSpeechAsset = SpeechAssets.FindByPredicate([SpeechAssetTag](const USpeechAsset* SpeechAsset)
	{
		return SpeechAsset && SpeechAsset->GetSpeechAssetTag() == SpeechAssetTag;
	});

	return FoundSpeechAsset ? *FoundSpeechAsset : nullptr;
}
