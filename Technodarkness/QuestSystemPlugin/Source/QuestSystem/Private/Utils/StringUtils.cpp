#include "Utils/StringUtils.h"

FString StringUtils::MakeIndent(int32 IndentLevel)
{
	check(IndentLevel >= 0);
	FString Indent("");
	for (int32 i = 0; i <= IndentLevel; ++i)
	{
		Indent.Append("    ");
	}
	return Indent;
}
