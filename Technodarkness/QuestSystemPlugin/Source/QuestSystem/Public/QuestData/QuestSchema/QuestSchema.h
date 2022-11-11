// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Types/QuestTypes.h"
#include "UObject/Object.h"
#include "QuestSchema.generated.h"

class UQuestSchemaNode_Entry;
class UObjectivesSchemaTransition;
class UQuest;
class UQuestLogComponent;

/**
 * Schema for controlling quest flow.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchema : public UObject
{
	GENERATED_BODY()

public:
	/** Starts processing quest flow. */
	void StartSchemaProcess();
	/** Ends processing quest flow. */
	void EndSchemaProcess(EQuestCompletionType WithQuestCompletionType);

	/** Can we start processing quest flow. */
	bool CanStartSchema(const UQuestLogComponent* InQuestLog) const;

	/** Sets quest to which the schema belongs to. */
	FORCEINLINE void SetOwningQuest(UQuest* InQuest)
	{
		OwningQuest = InQuest;
	}

	/** Returns the quest of this schema. */
	FORCEINLINE UQuest* GetOwningQuest() const
	{
		return OwningQuest;
	}

	/** Sets entry node of the schema. */
	FORCEINLINE void SetEntryNode(UQuestSchemaNode_Entry* InEntryNode)
	{
		EntryNode = InEntryNode;
	}

private:
	// First entry schema node.
	UPROPERTY()
	UQuestSchemaNode_Entry* EntryNode;

	// Cached owning quest.
	UPROPERTY()
	UQuest* OwningQuest;

	// We already ends schema process so we don't need to do it again.
	bool bSchemaProcessWasEnded = false;
};
