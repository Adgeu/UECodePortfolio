// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestSchemaTransition.generated.h"

class UQuestSchemaNode_Base;
class UQuestCheck;

/**
 * Base transition between quest schema nodes.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSchemaTransition : public UObject
{
	GENERATED_BODY()

public:
	/** Initialize transition. */
	void InitTransition(UQuestSchemaNode_Base* ToNode);

	/** Returns the node to which the transition leads.  */
	FORCEINLINE UQuestSchemaNode_Base* GetNextNode() const
	{
		return NextNode;	
	}

private:
	// The node to which the transition leads.
	UPROPERTY()
	UQuestSchemaNode_Base* NextNode;
};

