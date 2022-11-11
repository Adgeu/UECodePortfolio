// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "Interfaces/QuestTarget.h"

// Add default functionality here for any IQuestTargetId functions that are not pure virtual.
FGameplayTag IQuestTarget::GetQuestTargetId_Implementation() const
{
	return FGameplayTag();
}
