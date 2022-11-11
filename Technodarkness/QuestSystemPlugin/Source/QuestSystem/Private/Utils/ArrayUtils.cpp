#include "Utils/ArrayUtils.h"

#include "Algo/Accumulate.h"

int32 ArrayUtils::GetRandomIndexWithWeight(const TArray<float>& Weights)
{
	float WeightSum = Algo::Accumulate(Weights, 0.0f);

	int32 Index = 0;
	for (; Index < Weights.Num(); ++Index)
	{
		const float RandomValue = FMath::RandRange(0.0f, WeightSum);
		if (RandomValue < Weights[Index])
		{
			return Index;
		}
		WeightSum -= Weights[Index];
	}

	return Index;
}
