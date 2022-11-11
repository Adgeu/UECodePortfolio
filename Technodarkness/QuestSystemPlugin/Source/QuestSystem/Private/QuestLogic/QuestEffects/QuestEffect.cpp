// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestEffects/QuestEffect.h"

#include "Types/QuestTypes.h"

FString UQuestEffect::GetDescription(int32 IndentLevel) const
{
	return GetClass()->GetName()
		.Replace(TEXT("BP_"), TEXT(""))
		.Replace(TEXT("QuestEffect_"), TEXT(""))
		.Replace(TEXT("_C"), TEXT(""));
}

void UQuestEffect::ApplyEffect()
{
	UE_LOG(LogQuest, Display, TEXT("Applying effect \"%s\""), *GetName());
	ApplyEffectEvent();
}
