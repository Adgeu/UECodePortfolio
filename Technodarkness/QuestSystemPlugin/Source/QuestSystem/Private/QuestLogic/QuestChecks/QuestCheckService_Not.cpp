// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestChecks/QuestCheckService_Not.h"

#include "Utils/StringUtils.h"

bool UQuestCheckService_Not::IsChecked(const UQuestLogComponent* InQuestLog) const
{
	return CheckToInvert ? !CheckToInvert->IsChecked(InQuestLog) : true;
}

FString UQuestCheckService_Not::GetDescription(int32 IndentLevel) const
{
	return FString::Printf(TEXT("Not %s"), CheckToInvert ? *CheckToInvert->GetDescription(IndentLevel + 1) : *FString("NONE"));
}
