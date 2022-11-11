// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestCheck.h"
#include "QuestCheckService_Not.generated.h"

/**
 * Inverts check result.
 */
UCLASS(DisplayName = "!Not")
class QUESTSYSTEM_API UQuestCheckService_Not : public UQuestCheck
{
	GENERATED_BODY()

public:
	virtual bool IsChecked(const UQuestLogComponent* InQuestLog) const override;

	virtual FString GetDescription(int32 IndentLevel) const override;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category = "Check Settings")
	UQuestCheck* CheckToInvert;
};
