// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaTransition.h"
#include "QuestSchemaTransitionWithWeight.generated.h"

/**
 * Transition between quest schema nodes with weight for randomization.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaTransitionWithWeight : public UQuestSchemaTransition
{
	GENERATED_BODY()

public:
	/** Returns weight of the randomization. */
	FORCEINLINE float GetWeight() const
	{
		return Weight;
	}
	
protected:
	// Weight of the randomization.
	UPROPERTY(EditAnywhere, Category = "Transition Settings", meta = (ClampMin = 0.0, ClampMax = 1.0))
	float Weight = 1.0f;
};
