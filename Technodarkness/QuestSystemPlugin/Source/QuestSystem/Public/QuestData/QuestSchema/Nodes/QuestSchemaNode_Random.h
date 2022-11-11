// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaNode_Base.h"
#include "QuestSchemaNode_Random.generated.h"

class UQuestSchemaTransitionWithWeight;

/**
 * Randomizes quest flow.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaNode_Random : public UQuestSchemaNode_Base
{
	GENERATED_BODY()

public:
	static UQuestSchemaNode_Random* CreateNode(UQuestSchema* OwningSchema);

#if WITH_EDITORONLY_DATA
	virtual void AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex) override;
	virtual void RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex) override;
	virtual void ClearAllTransitions(int32 PinIndex) override;
#endif

	/** Enter point of the node. */
	virtual void EnterNode() override;

protected:
	/** Exit point of the node. */
	virtual void ExitNode() override;

private:
	// Transitions to next nodes. Chooses one transition randomly.
	UPROPERTY()
	TArray<const UQuestSchemaTransitionWithWeight*> Transitions;
};
