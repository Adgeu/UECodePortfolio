// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestLogic/QuestLogicBase.h"
#include "QuestCheck.generated.h"

class UQuest;
class UQuestLogComponent;

/**
 * Base class for quest check.
 */
UCLASS(Abstract, Blueprintable)
class QUESTSYSTEM_API UQuestCheck : public UQuestLogicBase
{
	GENERATED_BODY()

public:
	/** Is checker checked. */
	virtual bool IsChecked(const UQuestLogComponent* InQuestLog) const;

	virtual FString GetDescription(int32 IndentLevel) const override;

protected:
	/** Performs a check. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "IsChecked"))
	bool IsCheckedEvent(const UQuestLogComponent* InQuestLog) const;
};
