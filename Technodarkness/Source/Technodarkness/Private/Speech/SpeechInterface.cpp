// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "Speech/SpeechInterface.h"


// Add default functionality here for any ISpeechInterface functions that are not pure virtual.
USpeechComponent* ISpeechInterface::GetSpeechComponent_Implementation() const
{
	return nullptr;
}
