// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "Components/QuestLogComponent.h"

#include "QuestData/Quest.h"

bool UQuestLogComponent::AddNewQuest(const UQuest* QuestTemplate)
{
	if (!QuestTemplate)
	{
		UE_LOG(LogQuest, Error, TEXT("%s() Given quest is invalid!"), *FString(__FUNCTION__));
		return false;
	}
	
	if (!QuestTemplate->CanStartQuest(this))
	{
		UE_LOG(LogQuest, Display, TEXT("Quest \"%s\" can not be started because of starting requirenments!"), *QuestTemplate->GetName());
		return false;
	}
	
	UQuest* Quest = DuplicateObject(QuestTemplate, this);
	
	ActiveQuests.Add(Quest);
	OnNewQuestAddedDelegate.Broadcast(Quest);

	Quest->OnQuestEndedDelegate.AddDynamic(this, &UQuestLogComponent::OnQuestCompleted);
	Quest->StartQuest();

	return true;
}

bool UQuestLogComponent::CanStartQuest(const UQuest* Quest) const
{
	return Quest && Quest->CanStartQuest(this);
}

UQuest* UQuestLogComponent::GetActiveQuestByTemplate(const UQuest* QuestTemplate) const
{
	if (!QuestTemplate)
	{
		return nullptr;
	}

	const auto FindedQuest = ActiveQuests.FindByPredicate([QuestTemplate](const UQuest* ActiveQuest)
	{
		return ActiveQuest->GetQuestUniqueName() == QuestTemplate->GetQuestUniqueName();
	});

	return FindedQuest ? *FindedQuest : nullptr;
}

UQuest* UQuestLogComponent::GetCompletedQuestByTemplate(const UQuest* QuestTemplate) const
{
	if (!QuestTemplate)
	{
		return nullptr;
	}

	const auto FindedQuest = CompletedQuests.FindByPredicate([QuestTemplate](const UQuest* CompletedQuest)
	{
		return CompletedQuest->GetQuestUniqueName() == QuestTemplate->GetQuestUniqueName();
	});

	return FindedQuest ? *FindedQuest : nullptr;
}

TArray<UQuest*> UQuestLogComponent::GetSuccessfulCompletedQuests() const
{
	return CompletedQuests.FilterByPredicate([](const UQuest* Quest)
	{
		return Quest->GetCompletionType() == EQuestCompletionType::Success;
	});
}

TArray<UQuest*> UQuestLogComponent::GetFailedCompletedQuests() const
{
	return CompletedQuests.FilterByPredicate([](const UQuest* Quest)
	{
		return Quest->GetCompletionType() == EQuestCompletionType::Fail;
	});
}

bool UQuestLogComponent::IsQuestActive(const UQuest* Quest) const
{
	return Quest && ActiveQuests.ContainsByPredicate([Quest](const UQuest* ActiveQuest)
	{
		return ActiveQuest->GetQuestUniqueName() == Quest->GetQuestUniqueName();
	});
}

bool UQuestLogComponent::IsQuestCompleted(const UQuest* Quest) const
{
	return Quest && CompletedQuests.ContainsByPredicate([Quest](const UQuest* CompletedQuest)
	{
		return CompletedQuest->GetQuestUniqueName() == Quest->GetQuestUniqueName();
	});
}

bool UQuestLogComponent::AreQuestsActive(const TArray<UQuest*>& Quests) const
{
	for (const UQuest* Quest : Quests)
	{
		if (!IsQuestActive(Quest))
		{
			return false;
		}
	}
	return true;
}

bool UQuestLogComponent::AreQuestsCompleted(const TArray<UQuest*>& Quests) const
{
	for (const UQuest* Quest : Quests)
	{
		if (!IsQuestCompleted(Quest))
		{
			return false;
		}
	}
	return true;
}

void UQuestLogComponent::OnQuestCompleted(UQuest* Quest)
{
	ActiveQuests.RemoveSwap(Quest);
	CompletedQuests.Add(Quest);
}
