// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestSystem/QuestLogic/QuestTriggers/QuestTrigger_SpeechEnded.h"

#include "Speech/SpeechAsset.h"
#include "Speech/SpeechComponent.h"
#include "Speech/SpeechInterface.h"
#include "Types/QuestTypes.h"

void UQuestTrigger_SpeechEnded::OnTrackingBegins_Implementation()
{
	if (const AActor* Actor = FindQuestTargetActorWithId(ActorWithSpeechTargetId))
	{
		if (Actor->Implements<USpeechInterface>())
		{
			SpeechComponent = ISpeechInterface::Execute_GetSpeechComponent(Actor);
			if (SpeechComponent)
			{
				SpeechComponent->OnSpeechAssetEnd.AddDynamic(this, &UQuestTrigger_SpeechEnded::OnSpeechAssetEnded);
			}
		}
		else
		{
			UE_LOG(LogQuest, Error, TEXT("%s() Actor \"%s\" is not implemented ISpeechInterface!"), *FString(__FUNCTION__), *Actor->GetName());
		}
	}
}

void UQuestTrigger_SpeechEnded::OnTrackingEnded_Implementation()
{
	if (SpeechComponent)
	{
		SpeechComponent->OnSpeechAssetEnd.RemoveDynamic(this, &UQuestTrigger_SpeechEnded::OnSpeechAssetEnded);
	}
}

void UQuestTrigger_SpeechEnded::OnSpeechAssetEnded(const USpeechAsset* SpeechAsset)
{
	if (SpeechAsset->GetSpeechAssetTag() == SpeechTag)
	{
		EnterTriggeredState();
	}
}
