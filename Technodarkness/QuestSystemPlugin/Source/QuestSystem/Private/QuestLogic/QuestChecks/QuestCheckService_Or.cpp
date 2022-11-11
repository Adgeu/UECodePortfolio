// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestChecks/QuestCheckService_Or.h"

#include "Utils/StringUtils.h"

bool UQuestCheckService_Or::IsChecked(const UQuestLogComponent* InQuestLog) const
{
	for (const UQuestCheck* Check : Checks)
	{
		if (Check && Check->IsChecked(InQuestLog))
		{
			return true;
		}
	}
	return false;
}

FString UQuestCheckService_Or::GetDescription(int32 IndentLevel) const
{
	FString Result(TEXT("Or:\n"));
	
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
