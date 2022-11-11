// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestEffects/QuestEffect_Random.h"

#include "Algo/Copy.h"
#include "Types/QuestTypes.h"
#include "Utils/ArrayUtils.h"
#include "Utils/StringUtils.h"

void UQuestEffect_Random::ApplyEffect()
{
	Super::ApplyEffect();

	TArray<float> Weights;
	ArrayUtils::Select(EffectsToRandomize, Weights, [](const FWeightableEffect& WeightableEffect)
	{
		return WeightableEffect.Weight;
	});

	const int32 Index = ArrayUtils::GetRandomIndexWithWeight(Weights);
	UQuestEffect* EffectToApply = EffectsToRandomize[Index].Effect;
	if (EffectToApply)
	{
		EffectToApply->ApplyEffect();
	}
	else
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Effect with index %d is null!"), *FString(__FUNCTION__), Index);
	}
}

FString UQuestEffect_Random::GetDescription(int32 IndentLevel) const
{
	FString Result(FString::Printf(TEXT("Random:\n")));
	
	if (EffectsToRandomize.Num() > 0)
	{
		for (const FWeightableEffect& WeightableEffect : EffectsToRandomize)
		{
			Result.Append(FString::Printf(TEXT("%s* W: %.2f | %s\n"), *StringUtils::MakeIndent(IndentLevel), WeightableEffect.Weight, WeightableEffect.Effect ? *WeightableEffect.Effect->GetDescription(IndentLevel + 1) : TEXT("NONE")));
		}
	}
	else
	{
		Result.Append(FString::Printf(TEXT("%s-"), *StringUtils::MakeIndent(IndentLevel)));	
	}
	
	return Result;
}
