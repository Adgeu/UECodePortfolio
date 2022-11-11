// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "CustomDeveloperSettings/InputRebindSettings.h"

const UInputRebindSettings* UInputRebindSettings::Get()
{
	return GetDefault<UInputRebindSettings>();
}
