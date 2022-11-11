// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaNode_Base.h"
#include "QuestSchemaNode_Entry.generated.h"

class UQuestSchemaTransition;
class UQuestCheck;
class UQuestLogComponent;

/**
 * Entry node. Also contains requirements for the quest start.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaNode_Entry : public UQuestSchemaNode_Base
{
	GENERATED_BODY()

public:
	static UQuestSchemaNode_Entry* CreateNode(UQuestSchema* OwningSchema);
	
	/** Enter point of the node. */
	virtual void EnterNode() override;

#if WITH_EDITORONLY_DATA
	virtual FString GetNodeDetails(int32 IndentLevel) const override;
	
	virtual void AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex) override;
	virtual void RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex) override;
	virtual void ClearAllTransitions(int32 PinIndex) override;
#endif

	/** Can we start the quest or not. */
	bool IsStartRequirementChecked(const UQuestLogComponent* InQuestLog) const;
	
protected:
	/** Exit point of the node. */
	virtual void ExitNode() override;

	// Requirements for quest starting.
	UPROPERTY(EditAnywhere, Category = "Quest Start")
	UQuestCheck* StartRequirement;
	
private:
	// Transition to next node.
	UPROPERTY()
	UQuestSchemaTransition* Transition;
};
