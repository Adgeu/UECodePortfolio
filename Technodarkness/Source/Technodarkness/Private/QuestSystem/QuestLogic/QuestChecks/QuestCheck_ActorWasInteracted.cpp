// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "QuestSystem/QuestLogic/QuestChecks/QuestCheck_ActorWasInteracted.h"

#include "Actors/Interactive/InteractableActor.h"
#include "Types/QuestTypes.h"

bool UQuestCheck_ActorWasInteracted::IsChecked(const UQuestLogComponent* InQuestLog) const
{
	if (const AInteractableActor* InteractableActor = Cast<AInteractableActor>(FindQuestTargetActorWithId(ActorTargetId)))
	{
		return InteractableActor->WasInteracted();
	}

	UE_LOG(LogQuest, Error, TEXT("%s() Interactable actor not found with id \"%s\""), *FString(__FUNCTION__), *ActorTargetId.ToString());
	return false;
}
