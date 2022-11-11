// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestLogic/QuestLogicBase.h"
#include "QuestEffect.generated.h"

/**
 * Quest effect changes states of outer systems.
 */
UCLASS(Abstract, Blueprintable)
class QUESTSYSTEM_API UQuestEffect : public UQuestLogicBase
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 IndentLevel) const override;
	
	virtual void ApplyEffect();

protected:
	/** Calls when the quest or the objective was ended. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ApplyEffect"))
	void ApplyEffectEvent();
};
