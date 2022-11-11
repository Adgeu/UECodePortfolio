// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestLogicBase.h"

#include "Interfaces/QuestTarget.h"
#include "Kismet/GameplayStatics.h"
#include "QuestData/Quest.h"
#include "Components/QuestLogComponent.h"

FString UQuestLogicBase::GetDescription(int32 IndentLevel) const
{
	return GetClass()->GetName();
}

UQuest* UQuestLogicBase::GetQuest() const
{
	return GetTypedOuter<UQuest>();
}

UQuestLogComponent* UQuestLogicBase::GetQuestLog() const
{
	return GetTypedOuter<UQuestLogComponent>();
}

AActor* UQuestLogicBase::FindQuestTargetActorWithId(const FGameplayTag& TargetId) const
{
	if (!TargetId.IsValid())
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Trying to find quest target with not valid id."), *FString(__FUNCTION__));
		return nullptr;
	}
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UQuestTarget::StaticClass(), FoundActors);

	AActor** FoundActorWithId = FoundActors.FindByPredicate([&](const AActor* Actor)
	{
		return IQuestTarget::Execute_GetQuestTargetId(Actor) == TargetId;
	});
	
	return FoundActorWithId ? *FoundActorWithId : nullptr;
}

ACharacter* UQuestLogicBase::GetPlayerCharacter() const
{
	return UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}
