// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestTriggers/QuestTrigger_TimeElapsed.h"

void UQuestTrigger_TimeElapsed::OnTrackingBegins_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UQuestTrigger_TimeElapsed::EnterTriggeredState, Time);
}

void UQuestTrigger_TimeElapsed::OnTrackingEnded_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UQuestTrigger_TimeElapsed::OnUndoTriggeredState_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UQuestTrigger_TimeElapsed::EnterTriggeredState, Time);
}
