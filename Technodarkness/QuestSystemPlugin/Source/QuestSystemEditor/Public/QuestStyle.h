#pragma once

#include "SlateBasics.h"

class FQuestStyle
{
public:
	// Initializes the value of QuestStyleInstance and registers it with the Slate Style Registry.
	static void Initialize();

	// Unregisters the Slate Style Set and then resets the QuestStyleInstance pointer.
	static void Shutdown();

	// Retrieves a reference to the Slate Style pointed to by QuestStyleInstance.
	static const class ISlateStyle& Get();

	// Retrieves the name of the Style Set.
	static FName GetStyleSetName();

private:
	// Singleton instance used for our Style Set.
	static TSharedPtr<class FSlateStyleSet> QuestStyleInstance;
};
