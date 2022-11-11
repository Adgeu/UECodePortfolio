// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestChecks/QuestCheck.h"

bool UQuestCheck::IsChecked(const UQuestLogComponent* InQuestLog) const
{
	return IsCheckedEvent(InQuestLog);
}

FString UQuestCheck::GetDescription(int32 IndentLevel) const
{
	return GetClass()->GetName()
		.Replace(TEXT("BP_"), TEXT(""))
		.Replace(TEXT("QuestCheck_"), TEXT(""))
		.Replace(TEXT("_C"), TEXT(""));
}
