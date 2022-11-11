// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaTransition.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Base.h"
#include "QuestSchemaTransitionWithCheck.generated.h"

/**
 * Transition between quest schema nodes with possibility to contain an additional check.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaTransitionWithCheck : public UQuestSchemaTransition
{
	GENERATED_BODY()

public:
	/** Checks for transition is allowed based on transition check. If there is no transition check, returns TRUE. */
	bool CanMakeTransition() const;

	/** Has transition check. */
	FORCEINLINE bool HasTransitionCheck() const
	{
		return TransitionCheck != nullptr;
	}
	
protected:
	// Check for transition is allowed.
	UPROPERTY(EditAnywhere, Category = "Transition Settings")
	UQuestCheck* TransitionCheck;
};
