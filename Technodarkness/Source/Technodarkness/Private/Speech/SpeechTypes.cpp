// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "Speech/SpeechTypes.h"

float FSpeechPhrase::GetDuration() const
{
	if (DurationMethod == ESpeechPhraseDuration::Manual)
	{
		return Duration;
	}

	checkf(Sound, TEXT("%s() Duration method setted as \"FromSound\", but sound asset is null!"), *FString(__FUNCTION__))
	return Sound->GetDuration();
}
