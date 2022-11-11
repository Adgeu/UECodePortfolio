// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestCheck.h"
#include "QuestCheckService_And.generated.h"

/**
 * All checks in list must be TRUE.
 */
UCLASS(DisplayName = "!And")
class QUESTSYSTEM_API UQuestCheckService_And : public UQuestCheck
{
	GENERATED_BODY()

public:
	virtual bool IsChecked(const UQuestLogComponent* InQuestLog) const override;

	virtual FString GetDescription(int32 IndentLevel) const override;
	
protected:
	UPROPERTY(EditAnywhere, Instanced, Category = "Check Settings")
	TArray<UQuestCheck*> Checks;
};
