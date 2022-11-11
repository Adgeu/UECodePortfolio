// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaNode_Base.h"
#include "QuestSchemaNode_Selector.generated.h"

class UQuestSchemaTransitionWithCheck;

/**
 * Checks all transitions and running first matching transition.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaNode_Selector : public UQuestSchemaNode_Base
{
	GENERATED_BODY()

public:
	static UQuestSchemaNode_Selector* CreateNode(UQuestSchema* OwningSchema);

#if WITH_EDITORONLY_DATA
	virtual void AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex) override;
	virtual void RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex) override;
	virtual void ClearAllTransitions(int32 PinIndex) override;
	virtual void SortTransitions(int32 PinIndex) override;
#endif

	/** Enter point of the node. */
	virtual void EnterNode() override;

protected:
	/** Exit point of the node. */
	virtual void ExitNode() override;
	
private:
	// Transitions to next nodes.
	UPROPERTY()
	TArray<const UQuestSchemaTransitionWithCheck*> Transitions;
};
