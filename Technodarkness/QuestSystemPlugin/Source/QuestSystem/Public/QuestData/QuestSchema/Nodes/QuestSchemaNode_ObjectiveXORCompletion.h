// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaNode_Base.h"
#include "Types/QuestTypes.h"
#include "QuestSchemaNode_ObjectiveXORCompletion.generated.h"

class UQuestSchemaTransition;
class UQuestObjective;

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Helper struct for containing tracking objective and delegate handle on objective ended.
 */
USTRUCT()
struct FTrackedObjective
{
	GENERATED_BODY()

	UPROPERTY()
	UQuestObjective* Objective;

	FDelegateHandle ObjectiveEndedDelegate;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This node tracks objectives completion.
 * If one of the connected objective was completed, other connected objectives automatically completes.
 *
 * @note Only objectives can be connected to the output of this node.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaNode_ObjectiveXORCompletion : public UQuestSchemaNode_Base
{
	GENERATED_BODY()

public:
	static UQuestSchemaNode_ObjectiveXORCompletion* CreateNode(UQuestSchema* OwningSchema);

	/** Enter point of the node. */
	virtual void EnterNode() override;

#if WITH_EDITORONLY_DATA
	virtual void AddTransition(UQuestSchemaTransition* TransitionToAdd, int32 PinIndex) override;
	virtual void RemoveTransition(UQuestSchemaNode_Base* ToNode, int32 PinIndex) override;
	virtual void ClearAllTransitions(int32 PinIndex) override;
#endif
	
protected:
	/** Exit point of the node. */
	virtual void ExitNode() override;

	// If one of the objectives was completed with success, complete other objectives with this completion type.
	UPROPERTY(EditAnywhere, Category = "Completion Settins")
	EQuestCompletionType OnSuccessCompleteOtherWithType = EQuestCompletionType::Fail;

	// If one of the objectives was completed with fail, complete other objectives with this completion type.
	UPROPERTY(EditAnywhere, Category = "Completion Settins")
	EQuestCompletionType OnFailCompleteOtherWithType = EQuestCompletionType::Success;
	
private:
	// One of the connected objectives was ended.
	void OnObjectiveEnded(UQuestObjective* EndedObjective);
	
	UPROPERTY()
	TArray<const UQuestSchemaTransition*> Transitions;

	UPROPERTY()
	TArray<FTrackedObjective> TrackedObjectives;
};
