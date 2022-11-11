#pragma once

namespace ArrayUtils
{
	/**
	 * Returns random index of the array. 
	 * The probability of getting a certain index is determined by the given weights.
	 */
	int32 GetRandomIndexWithWeight(const TArray<float>& Weights);

	/**
	 * Projects each element of an array into a new form.
	 * @param Source An array of values to invoke a transform function on.
	 * @param Out [OUT] An array whose elements are the result of invoking the transform function on each element of source.
	 * @param Pred A transform function to apply to each source element.
	 */
	template <typename TSource, typename TOut, typename TPredicate>
	void Select(const TArray<TSource>& Source, TArray<TOut>& Out, TPredicate Pred)
	{
		Out.Reserve(Source.Num());
		for (const TSource& Val : Source)
		{
			Out.Emplace(Pred(Val));
		}
	}
}
