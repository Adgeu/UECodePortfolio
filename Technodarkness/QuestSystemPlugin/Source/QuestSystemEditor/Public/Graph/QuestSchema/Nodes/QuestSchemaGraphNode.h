// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "QuestSchemaGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class UQuestSchemaGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UQuestSchemaGraphNode();
	
	/** Returns the input pin for this state. */
	virtual UEdGraphPin* GetInputPin(int32 InputIndex = 0) const;
	/** Returns the output pin for this state. */
	virtual UEdGraphPin* GetOutputPin(int32 InputIndex = 0) const;

	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual void DestroyNode() override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetClassName() const;
	virtual FText GetDescription() const;

	/** Gets icon resource name for title bar */
	virtual FName GetNameIcon() const;

	void UpdateExecutionOrder();

	void PostCopyNode();

	UPROPERTY()
	class UQuestSchemaNode_Base* NodeInstance;
	
	// Highlight other child node indexes when hovering over a child.
	uint32 bHighlightChildNodeIndices : 1;

protected:
	void ResetNodeOwner();
	
	// For the node we want to update execution order
	uint32 bShouldUpdateExecutionOrder : 1;
};
