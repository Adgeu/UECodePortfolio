// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestSystem/QuestLogic/QuestTriggers/QuestTrigger_CharacterOverlapLocationMarker.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/QuestWorldActors/QuestLocationMarker.h"
#include "Types/QuestTypes.h"

void UQuestTrigger_CharacterOverlapLocationMarker::OnTrackingBegins_Implementation()
{
	// Getting marker
	TargetLocationMarker = Cast<AQuestLocationMarker>(FindQuestTargetActorWithId(LocationMarkerTargetId));
	if (!TargetLocationMarker)
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Target location marker with id \"%s\" is not found!"), *FString(__FUNCTION__), *LocationMarkerTargetId.ToString());
		return;
	}

	// Adding character to the marker
	if (bOverlapWithPlayer)
	{
		TargetLocationMarker->AddOverlappingCharacter(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	else
	{
		if (ACharacter* Character = Cast<ACharacter>(FindQuestTargetActorWithId(OverlapCharacterId)))
		{
			TargetLocationMarker->AddOverlappingCharacter(Character);
		}
		else
		{
			UE_LOG(LogQuest, Error, TEXT("%s() Overlapping character with id \"%s\" is not found!"), *FString(__FUNCTION__), *OverlapCharacterId.ToString());
			return;
		}
	}

	// Binding marker overlap delegates
	if (HasEnterFlag())
	{
		TargetLocationMarker->OnBeginOverlapWithCharacter.AddDynamic(this, &UQuestTrigger_CharacterOverlapLocationMarker::OnBeginOverlapWithCharacter);
	}
	if (HasExitFlag())
	{
		TargetLocationMarker->OnEndOverlapWithCharacter.AddDynamic(this, &UQuestTrigger_CharacterOverlapLocationMarker::OnEndOverlapWithCharacter);
	}
}

void UQuestTrigger_CharacterOverlapLocationMarker::OnTrackingEnded_Implementation()
{
	if (TargetLocationMarker)
	{
		if (HasEnterFlag())
		{
			TargetLocationMarker->OnBeginOverlapWithCharacter.RemoveDynamic(this, &UQuestTrigger_CharacterOverlapLocationMarker::OnBeginOverlapWithCharacter);
		}
		if (HasExitFlag())
		{
			TargetLocationMarker->OnEndOverlapWithCharacter.RemoveDynamic(this, &UQuestTrigger_CharacterOverlapLocationMarker::OnEndOverlapWithCharacter);
		}
	}
}

void UQuestTrigger_CharacterOverlapLocationMarker::OnBeginOverlapWithCharacter(ACharacter* Character)
{
	EnterTriggeredState();
}

void UQuestTrigger_CharacterOverlapLocationMarker::OnEndOverlapWithCharacter(ACharacter* Character)
{
	EnterTriggeredState();
}
