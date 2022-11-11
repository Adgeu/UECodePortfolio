// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "QuestLogicBase.generated.h"

/**
 * Base class for classes that rules the quest behavior.
 * Contains common helper methods.
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class QUESTSYSTEM_API UQuestLogicBase : public UObject
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 IndentLevel = 0) const;

protected:
	/** Returns the quest that this logic item belongs to. */
	UFUNCTION(BlueprintPure)
	class UQuest* GetQuest() const;
	
	/** Returns quest log. We can get quest log only when we added quest to the quest log. */
	UFUNCTION(BlueprintPure)
	class UQuestLogComponent* GetQuestLog() const;
	
	/** Finds quest target actor in the world with specific id. */
	UFUNCTION(BlueprintPure)
	AActor* FindQuestTargetActorWithId(const FGameplayTag& TargetId) const;

	/** Returns player character. */
	UFUNCTION(BlueprintPure)
	ACharacter* GetPlayerCharacter() const;
};
