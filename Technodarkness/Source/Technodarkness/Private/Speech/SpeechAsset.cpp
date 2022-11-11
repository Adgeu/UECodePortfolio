// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "Speech/SpeechAsset.h"

#include "Speech/SpeechTypes.h"

void USpeechAsset::PlayFromStart(UWorld* InWorld, float InFirstDelay)
{
	if (!InWorld)
	{
		UE_LOG(LogSpeech, Error, TEXT("%s() World is null!"), *FString(__FUNCTION__));
		return;
	}

	CurrentSpeechPhraseIndex = 0;

	if (!HasAnySpeechPhrasesData())
	{
		UE_LOG(LogSpeech, Warning, TEXT("%s() There is no any speech phrases data!"), *FString(__FUNCTION__));
		return;
	}

	if (InFirstDelay != 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &USpeechAsset::PlayFromCurrentPhrase, InWorld);
		InWorld->GetTimerManager().SetTimer(SpeechTimer, Delegate, InFirstDelay, false);
	}
	else
	{
		PlayFromCurrentPhrase(InWorld);
	}
}

void USpeechAsset::PlayFromCurrentPhrase(UWorld* InWorld)
{
	if (!InWorld)
	{
		UE_LOG(LogSpeech, Error, TEXT("%s() World is null!"), *FString(__FUNCTION__));
		return;
	}

	FSpeechPhrase SpeechPhrase;
	if (!TryGetCurrentSpeechPhrase(SpeechPhrase))
	{
		return;
	}
	
	OnSpeechPhraseStartInternal.ExecuteIfBound(SpeechPhrase);

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &USpeechAsset::OnSpeechPhraseEnd, InWorld);
	const float Duration = SpeechPhrase.GetDuration();
	InWorld->GetTimerManager().SetTimer(SpeechTimer, Delegate, Duration, false);
}

void USpeechAsset::OnSpeechPhraseEnd(UWorld* InWorld)
{
	OnSpeechPhraseEndInternal.ExecuteIfBound();

	++CurrentSpeechPhraseIndex;
	if (IsCurrentSpeechPhraseIndexInBounds())
	{
		if (GeneralDelay != 0.0f)
		{
			FTimerDelegate Delegate;
			Delegate.BindUObject(this, &USpeechAsset::PlayFromCurrentPhrase, InWorld);
			InWorld->GetTimerManager().SetTimer(SpeechTimer, Delegate, GeneralDelay, false);
		}
		else
		{
			PlayFromCurrentPhrase(InWorld);
		}
	}
	else
	{
		OnSpeechEndInternal.ExecuteIfBound();
	}
}

bool USpeechAsset::TryGetCurrentSpeechPhrase(FSpeechPhrase& SpeechPhrase) const
{
	if (!IsCurrentSpeechPhraseIndexInBounds())
	{
		UE_LOG(LogSpeech, Error, TEXT("%s() CurrentSpeechPhraseIndex is greater or equal speech phrases count. Index: %d, Phrases count: %d"), *FString(__FUNCTION__), CurrentSpeechPhraseIndex, GetSpeechPhrasesDataCount());
		return false;
	}
	
	if (SpeechPhrasesDataTable)
	{
		TArray<FSpeechPhraseRow*> PhraseRows;
		SpeechPhrasesDataTable->GetAllRows<FSpeechPhraseRow>(TEXT("Get Current Speech Phrase"), PhraseRows);

		const FSpeechPhraseRow* SpeechPhraseRow = PhraseRows[CurrentSpeechPhraseIndex];
		SpeechPhrase.Text = SpeechPhraseRow->Text;
		SpeechPhrase.Sound = SpeechPhraseRow->Sound;
		SpeechPhrase.DurationMethod = SpeechPhraseRow->DurationMethod;
		SpeechPhrase.Duration = SpeechPhraseRow->ManualDuration;
	}	
	else
	{
		SpeechPhrase = SpeechPhrases[CurrentSpeechPhraseIndex];
	}
	
	return true;
}
