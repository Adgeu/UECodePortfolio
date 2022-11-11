// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestCheck.h"
#include "QuestCheckService_Or.generated.h"

/**
 * Any checks in list must be TRUE.
 */
UCLASS(DisplayName = "!Or")
class QUESTSYSTEM_API UQuestCheckService_Or : public UQuestCheck
{
	GENERATED_BODY()

public:
	virtual bool IsChecked(const UQuestLogComponent* InQuestLog) const override;

	virtual FString GetDescription(int32 IndentLevel) const override;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category = "Check Settings")
	TArray<UQuestCheck*> Checks;
};
