// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestLogic/QuestChecks/QuestCheck.h"
#include "QuestCheck_ActorWasInteracted.generated.h"

/**
 * Checks if the interactable actor was interacted at least once. Actor must be inherited from AInteractableActor.
 * @see AInteractableActor.
 */
UCLASS()
class TECHNODARKNESS_API UQuestCheck_ActorWasInteracted : public UQuestCheck
{
	GENERATED_BODY()

public:
	virtual bool IsChecked(const UQuestLogComponent* InQuestLog) const override;
	
protected:
	/**
	 * Id of the interactable actor that we want check. Actor must be AInteractableActor.
	 * @see AInteractableActor
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checker Settings")
	FGameplayTag ActorTargetId;
};
