// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "QuestTarget.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UQuestTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * Gives the opportunity to use the actor as a target for tracking various quest system conditions.
 */
class QUESTSYSTEM_API IQuestTarget
{
	GENERATED_BODY()

public:
	/** Returns quest target id. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FGameplayTag GetQuestTargetId() const;
	virtual FGameplayTag GetQuestTargetId_Implementation() const;
};
