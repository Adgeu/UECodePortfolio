// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestSystem/QuestLogic/QuestEffect/QuestEffect_EditActorWithPlaceableMeshes.h"

#include "Actors/Interactive/ActorWithPlaceableMeshes.h"
#include "Types/QuestTypes.h"

void UQuestEffect_EditActorWithPlaceableMeshes::ApplyEffect()
{
	Super::ApplyEffect();

	AActor* TargetActor = FindQuestTargetActorWithId(ActorTargetId);
	if (!TargetActor)
	{
		return;
	}
	
	AActorWithPlaceableMeshes* ActorWithPlaceableMeshes = Cast<AActorWithPlaceableMeshes>(TargetActor);
	if (!ActorWithPlaceableMeshes)
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Can't find ActorWithPlaceableMeshes with id \"%s\""), *FString(__FUNCTION__), *ActorTargetId.ToString());
		return;
	}

	for (const FEditActorWithPlaceableMeshesOperation& Operation : Operations)
	{
		switch (Operation.OperationSign)
		{
			case EEditActorWithPlaceableMeshesOperationSign::AddStaticMeshes:
				ActorWithPlaceableMeshes->OnShouldAddStaticMeshes(Operation.StaticMeshes);
				break;

			case EEditActorWithPlaceableMeshesOperationSign::RemoveStaticMeshes:
				ActorWithPlaceableMeshes->OnShouldRemoveStaticMeshes(Operation.StaticMeshes);
				break;
		}
	}
}
