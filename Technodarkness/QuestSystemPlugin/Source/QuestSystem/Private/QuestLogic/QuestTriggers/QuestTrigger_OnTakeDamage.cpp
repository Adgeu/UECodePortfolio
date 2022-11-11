// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestTriggers/QuestTrigger_OnTakeDamage.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Types/QuestTypes.h"

void UQuestTrigger_OnTakeDamage::OnTrackingBegins_Implementation()
{
	if (bShouldFilterByInstigator)
	{
		switch (InstigatorType)
		{
		case ETakeDamageActorType::Player:
			InstigatorActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			break;

		case ETakeDamageActorType::QuestTargetActor:
			InstigatorActor = FindQuestTargetActorWithId(InstigatorTargetId);
			break;
		}

		if (!IsValid(InstigatorActor))
		{
			UE_LOG(LogQuest, Error, TEXT("%s() We want to filter by instigator, but instigator actor is not valid!"), *FString(__FUNCTION__))
		}
	}

	switch (VictimType)
	{
	case ETakeDamageActorType::Player:
		VictimActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		break;

	case ETakeDamageActorType::QuestTargetActor:
		VictimActor = FindQuestTargetActorWithId(VictimTargetId);
		break;
	}

	if (IsValid(VictimActor))
	{
		VictimActor->OnTakeAnyDamage.AddDynamic(this, &UQuestTrigger_OnTakeDamage::OnTakeAnyDamage);
	}
	else
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Victim actor is not valid!"), *FString(__FUNCTION__))
	}
}

void UQuestTrigger_OnTakeDamage::OnTrackingEnded_Implementation()
{
	if (IsValid(VictimActor))
	{
		VictimActor->OnTakeAnyDamage.RemoveDynamic(this, &UQuestTrigger_OnTakeDamage::OnTakeAnyDamage);
	}

	InstigatorActor = nullptr;
	VictimActor = nullptr;
}

void UQuestTrigger_OnTakeDamage::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bShouldFilterByInstigator && DamageCauser != InstigatorActor)
	{
		return;
	}

	EnterTriggeredState();
}
