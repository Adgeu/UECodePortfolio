// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestEffect.h"
#include "QuestEffect_Random.generated.h"

USTRUCT(BlueprintType)
struct FWeightableEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.0, ClampMax = 1.0))
	float Weight = 1.0f;

	UPROPERTY(EditAnywhere, Instanced)
	UQuestEffect* Effect;
};

/**
 * Applies random effect in list.
 */
UCLASS(DisplayName = "!Random")
class QUESTSYSTEM_API UQuestEffect_Random : public UQuestEffect
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect() override;

	virtual FString GetDescription(int32 IndentLevel) const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Settings")
	TArray<FWeightableEffect> EffectsToRandomize;
};
