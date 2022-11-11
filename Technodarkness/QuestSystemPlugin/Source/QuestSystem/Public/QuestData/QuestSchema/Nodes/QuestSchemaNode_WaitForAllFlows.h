// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaNode_Base.h"
#include "QuestSchemaNode_WaitForAllFlows.generated.h"

/**
 * Waiting all flows that connected to this node and then entering next node.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaNode_WaitForAllFlows : public UQuestSchemaNode_Base
{
	GENERATED_BODY()

public:
	static UQuestSchemaNode_WaitForAllFlows* CreateNode(UQuestSchema* OwningSchema);
	
	virtual void EnterNode() override;
	
	FORCEINLINE void SetFlowCountToWait(int32 NewCount)
	{
		FlowCountToWait = NewCount;
	}

#if WITH_EDITORONLY_DATA
	virtual void AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex) override;
	virtual void RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex) override;
	virtual void ClearAllTransitions(int32 PinIndex) override;
#endif

protected:
	virtual void ExitNode() override;

private:
	int32 FlowCountToWait = 0;

	// Transition to next node.
	UPROPERTY()
	const UQuestSchemaTransition* Transition;
};
