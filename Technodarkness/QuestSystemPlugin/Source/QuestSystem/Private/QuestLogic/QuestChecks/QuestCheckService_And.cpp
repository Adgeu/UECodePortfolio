// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestChecks/QuestCheckService_And.h"

#include "Utils/StringUtils.h"

bool UQuestCheckService_And::IsChecked(const UQuestLogComponent* InQuestLog) const
{
	for (const UQuestCheck* Check : Checks)
	{
		if (Check && !Check->IsChecked(InQuestLog))
		{
			return false;
		}
	}
	return true;
}

FString UQuestCheckService_And::GetDescription(int32 IndentLevel) const
{
	FString Result(TEXT("And:\n"));
	
	if (Checks.Num() > 0)
	{
		for (const UQuestCheck* Check : Checks)
		{
			Result.Append(FString::Printf(TEXT("%s* %s\n"), *StringUtils::MakeIndent(IndentLevel), Check ? *Check->GetDescription(IndentLevel + 1) : TEXT("NONE")));
		}
	}
	else
	{
		Result.Append(FString::Printf(TEXT("%s-"), *StringUtils::MakeIndent(IndentLevel)));	
	}
	
	return Result;
}
