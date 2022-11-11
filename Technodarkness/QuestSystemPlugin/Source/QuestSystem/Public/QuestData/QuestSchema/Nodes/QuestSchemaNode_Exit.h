// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaNode_Base.h"
#include "Types/QuestTypes.h"
#include "QuestSchemaNode_Exit.generated.h"

/**
 * This node finish quest with specified completion type.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaNode_Exit : public UQuestSchemaNode_Base
{
	GENERATED_BODY()

public:
	static UQuestSchemaNode_Exit* CreateNode(UQuestSchema* OwningSchema);
	
	/** Enter point of the node. */
	virtual void EnterNode() override;

#if WITH_EDITORONLY_DATA
	virtual FString GetNodeDetails(int32 IndentLevel) const override;
#endif

protected:
	/** Exit point of the node. */
	virtual void ExitNode() override;

	// Completion type with which we want to finish quest.
	UPROPERTY(EditAnywhere, Category = "Quest Completion")
	EQuestCompletionType QuestCompletionType = EQuestCompletionType::Success;
};
