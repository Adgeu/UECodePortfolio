// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpeechInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USpeechInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * All actors that contain SpeechComponent must implement this interface.
 * @see USpeechComponent
 */
class TECHNODARKNESS_API ISpeechInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Returns speech component. */
	UFUNCTION(BlueprintNativeEvent)
	class USpeechComponent* GetSpeechComponent() const;
	virtual USpeechComponent* GetSpeechComponent_Implementation() const;
};
