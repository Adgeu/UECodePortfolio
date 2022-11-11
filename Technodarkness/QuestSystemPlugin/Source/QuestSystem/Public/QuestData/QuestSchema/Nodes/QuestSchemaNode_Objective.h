// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaNode_Base.h"
#include "QuestSchemaNode_Objective.generated.h"

class UQuestSchema;
class UQuestObjective;
class UQuestSchemaTransition;

/**
 * The node contains objective and controls quest flow after objective completion.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaNode_Objective : public UQuestSchemaNode_Base
{
	GENERATED_BODY()

public:
	static UQuestSchemaNode_Objective* CreateNode(UQuestSchema* OwningSchema);

#if WITH_EDITORONLY_DATA
	virtual FString GetNodeDetails(int32 IndentLevel) const override;
	
	virtual void AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex) override;
	virtual void RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex) override;
	virtual void ClearAllTransitions(int32 PinIndex) override;
#endif
	
	/** Enter point of the node. */
	virtual void EnterNode() override;

	/** Returns objective item. */
	FORCEINLINE UQuestObjective* GetObjective() const
	{
		return Objective;
	}

protected:
	/** Exit point of the node. */
	virtual void ExitNode() override;

	// Objective item.
	UPROPERTY(EditAnywhere, Category = "Objective Settings")
	UQuestObjective* Objective;
	
private:
	/** Objective was ended. */
	void OnObjectiveEnded(UQuestObjective* EndedObjective);
	
	// Transition to next node only on success completion (index 0).
	UPROPERTY()
	const UQuestSchemaTransition* TransitionOnSuccessCompletion;

	// Transition to next node only on fail completion (index 1).
	UPROPERTY()
	const UQuestSchemaTransition* TransitionOnFailCompletion;

	// Transition to next node on any completion (index 2).
	UPROPERTY()
	const UQuestSchemaTransition* TransitionOnAnyCompletion;
};
