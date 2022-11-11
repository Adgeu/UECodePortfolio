// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "Characters/Controllers/TDPlayerController.h"


FGenericTeamId ATDPlayerController::GetGenericTeamId() const
{
	return FGenericTeamId(static_cast<uint8>(TeamId));
}
