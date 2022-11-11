// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestFactory.h"

#include "QuestData/Quest.h"

UQuestFactory::UQuestFactory()
{
	SupportedClass = UQuest::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UQuestFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(InClass->IsChildOf(UQuest::StaticClass()));
	return NewObject<UQuest>(InParent, InClass, InName, Flags, Context);
}

