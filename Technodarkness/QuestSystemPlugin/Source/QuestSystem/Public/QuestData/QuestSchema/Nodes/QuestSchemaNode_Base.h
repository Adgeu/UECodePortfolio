// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestSchemaNode_Base.generated.h"

class UQuestSchemaTransition;
class UQuestSchemaTransitionWithCheck;

/**
 * Contains base functionality for quest schema node.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaNode_Base : public UObject
{
	GENERATED_BODY()

public:
	UQuestSchemaNode_Base();
	
	/** Enter point of the node. Override this for specify the node action on entering. */
	virtual void EnterNode();

#if WITH_EDITORONLY_DATA
	/**
	 * Adds the transition to next node.
	 * @param TransitionToAdd Transition that we want to add.
	 * @param PinIndex If the node has several output pins, specify index of the pin to which we want to add transition.
	 */
	virtual void AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex = 0);

	/**
	 * Removes transition to the next node.
	 * @param ToNode The node to which transition we want to remove.
	 * @param PinIndex If the node has several output pins, specify index of the pin from which we want to remove transition.
	 */
	virtual void RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex = 0);

	/**
	 * If output pin contains several transitions, sorts it.
	 * @param PinIndex If the node has several output pins, specify index of the pin that we want to sort.
	 */
	virtual void SortTransitions(int32 PinIndex = 0);

	/**
	 * Clears all transition in pin.
	 * @param PinIndex If the node has several output pins, specify index of the pin that we want to clear.
	 */
	virtual void ClearAllTransitions(int32 PinIndex = 0);

	/** Returns name of the node. */
	FString GetNodeName() const;
	/** Returns nice class name of the node. */
	FString GetNiceClassName() const;
	/** Returns details of the node. */
	virtual FString GetNodeDetails(int32 IndentLevel = 0) const;

	// Index for sorting transitions to that node for nodes like Sequence, Selector, etc.
	int32 ExecutionIndex = 0;
#endif

	/** Returns owning quest schema. */
	class UQuestSchema* GetOwningSchema() const;

	/** Returns owning quest. */
	class UQuest* GetOwningQuest() const;
	
protected:
	/** Exit point of the node. Override this for specify the node action on exiting. */
	virtual void ExitNode();

#if WITH_EDITORONLY_DATA
	// Node name.
	UPROPERTY(EditAnywhere, Category = "Description")
	FString NodeName;
#endif

	bool bNodeWasEntered = false;
};
