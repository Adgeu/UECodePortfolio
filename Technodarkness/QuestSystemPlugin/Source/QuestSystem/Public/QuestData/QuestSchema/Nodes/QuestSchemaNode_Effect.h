// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaNode_Base.h"
#include "QuestSchemaNode_Effect.generated.h"

class UQuestSchemaTransition;
class UQuestEffect;

/**
 * Node that applies quest effects.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaNode_Effect : public UQuestSchemaNode_Base
{
	GENERATED_BODY()

public:
	static UQuestSchemaNode_Effect* CreateNode(UQuestSchema* OwningSchema);
	
	/** Enter point of the node. */
	virtual void EnterNode() override;

#if WITH_EDITORONLY_DATA
	virtual FString GetNodeDetails(int32 IndentLevel) const override;
	
	virtual void AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex) override;
	virtual void RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex) override;
	virtual void ClearAllTransitions(int32 PinIndex) override;
#endif
	
protected:
	/** Exit point of the node. */
	virtual void ExitNode() override;

	// Effects that we want to apply when the schema flow is entering the node.
	UPROPERTY(EditAnywhere, Instanced, Category = "Effect Settings")
	TArray<UQuestEffect*> Effects;

private:
	// Transition to next node.
	UPROPERTY()
	const UQuestSchemaTransition* Transition;
};
