// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "QuestData/QuestObjective.h"

#include "QuestData/Quest.h"
#include "QuestLogic/QuestChecks/QuestCheck.h"
#include "QuestLogic/QuestEffects/QuestEffect.h"
#include "QuestLogic/QuestTriggers/QuestTrigger.h"

void UQuestObjective::StartObjective()
{
	UE_LOG(LogQuest, Display, TEXT("Starting objective \"%s\" in quest \"%s\""), *this->GetName(), *GetTypedOuter<UQuest>()->GetName());
	
	CurrentState = EQuestState::InProgress;
	BeginCompletionTriggerTrack();
}

void UQuestObjective::EndObjective(EQuestCompletionType WithCompletionType)
{
	UE_LOG(LogQuest, Display, TEXT("Ending objective \"%s\" with completion type %s"), *this->GetName(), *UEnum::GetValueAsString(WithCompletionType));
	checkf(WithCompletionType != EQuestCompletionType::None, TEXT("%s() Trying to end objective with completion type NONE!"), *FString(__FUNCTION__));
	
	CurrentState = EQuestState::Completed;
	CompletionType = WithCompletionType;
	EndCompletionTriggerTrack();
	OnObjectiveEndedDelegate.Broadcast(this);
}

void UQuestObjective::BeginCompletionTriggerTrack()
{
	UE_LOG(LogQuest, Log, TEXT("Begin completion trigger tracking in objective \"%s\" in quest \"%s\""), *GetName(), *GetTypedOuter<UQuest>()->GetName());

	if (SuccessTrigger)
	{
		SuccessTrigger->OnTriggeredStateChanged.BindUObject(this, &UQuestObjective::OnSuccessTriggerChangedState);
		SuccessTrigger->BeginTracking();
	}

	if (FailTrigger)
	{
		FailTrigger->OnTriggeredStateChanged.BindUObject(this, &UQuestObjective::OnFailTriggerChangedState);
		FailTrigger->BeginTracking();
	}
}

void UQuestObjective::OnSuccessTriggerChangedState(const UQuestTrigger* InTrigger, bool bNewTriggeredState)
{
	if (bNewTriggeredState)
	{
		EndObjective(EQuestCompletionType::Success);
	}
}

void UQuestObjective::OnFailTriggerChangedState(const UQuestTrigger* InTrigger, bool bNewTriggeredState)
{
	if (bNewTriggeredState)
	{
		EndObjective(EQuestCompletionType::Fail);
	}
}

void UQuestObjective::EndCompletionTriggerTrack()
{
	UE_LOG(LogQuest, Log, TEXT("End completion trigger tracking in objective \"%s\" in quest \"%s\""), *GetName(), *GetTypedOuter<UQuest>()->GetName());

	if (SuccessTrigger)
	{
		SuccessTrigger->EndTracking();
		SuccessTrigger->OnTriggeredStateChanged.Unbind();
	}

	if (FailTrigger)
	{
		FailTrigger->EndTracking();
		FailTrigger->OnTriggeredStateChanged.Unbind();
	}
}
