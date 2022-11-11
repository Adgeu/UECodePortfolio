// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/QuestLogic/QuestEffect/QuestEffect_PlaySpeech.h"

#include "Speech/SpeechComponent.h"
#include "Speech/SpeechInterface.h"
#include "Types/QuestTypes.h"

void UQuestEffect_PlaySpeech::ApplyEffect()
{
	Super::ApplyEffect();

	if (const AActor* Actor = FindQuestTargetActorWithId(ActorWithSpeechTargetId))
	{
		if (Actor->Implements<USpeechInterface>())
		{
			if (USpeechComponent* SpeechComponent = ISpeechInterface::Execute_GetSpeechComponent(Actor))
			{
				SpeechComponent->PlaySpeechAssetWithTag(SpeechToPlayTag);
			}
		}
		else
		{
			UE_LOG(LogQuest, Error, TEXT("%s() Actor \"%s\" is not implemented ISpeechInterface!"), *FString(__FUNCTION__), *Actor->GetName());
		}
	}
}
