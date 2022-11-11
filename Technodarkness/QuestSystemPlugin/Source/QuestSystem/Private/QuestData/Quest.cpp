// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestData/Quest.h"

#include "Components/QuestLogComponent.h"
#include "QuestData/QuestSchema/QuestSchema.h"

UQuest::UQuest()
{
	QuestSchema = CreateDefaultSubobject<UQuestSchema>("QuestSchema");
	QuestSchema->SetOwningQuest(this);
}

UQuestLogComponent* UQuest::GetQuestLogComponent() const
{
	return GetTypedOuter<UQuestLogComponent>();
}

void UQuest::StartQuest()
{
	UE_LOG(LogQuest, Display, TEXT("Starting new quest \"%s\""), *this->GetName());
	
	CurrentState = EQuestState::InProgress;
	OnQuestBeginsDelegate.Broadcast(this);
	QuestSchema->StartSchemaProcess();
}

bool UQuest::CanStartQuest(const UQuestLogComponent* InQuestLog) const
{
	return QuestSchema->CanStartSchema(InQuestLog);
}

void UQuest::CompleteQuest(EQuestCompletionType WithCompletionType)
{
	checkf(WithCompletionType != EQuestCompletionType::None, TEXT("%s() Trying to end quest with completion type NONE!"), *FString(__FUNCTION__));

	CurrentState = EQuestState::Completed;
	CompletionType = WithCompletionType;

	for (UQuestObjective* Objective : ActiveObjectives)
	{
		Objective->EndCompletionTriggerTrack();
	}
	
	UE_LOG(LogQuest, Display, TEXT("Quest \"%s\" ended with %s"), *GetName(), *UEnum::GetValueAsString(CompletionType));
	OnQuestEndedDelegate.Broadcast(this);
}

void UQuest::OnObjectiveBegins(UQuestObjective* Objective)
{
	if (!ActiveObjectives.Contains(Objective))
	{
		ActiveObjectives.Add(Objective);
		OnObjectiveBeginsDelegate.Broadcast(this, Objective);
	}
}

void UQuest::OnObjectiveEnded(UQuestObjective* Objective)
{
	ActiveObjectives.RemoveSwap(Objective);
	OnObjectiveEndedDelegate.Broadcast(this, Objective);
}
