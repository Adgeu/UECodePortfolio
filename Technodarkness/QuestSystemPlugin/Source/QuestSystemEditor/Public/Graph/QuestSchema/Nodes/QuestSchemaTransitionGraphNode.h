// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "QuestSchemaTransitionGraphNode.generated.h"

class UQuestSchemaGraphNode;

/**
 * 
 */
UCLASS()
class QUESTSYSTEMEDITOR_API UQuestSchemaTransitionGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual bool CanDuplicateNode() const override { return false; }
	virtual void DestroyNode() override;
	//~ End UEdGraphNode Interface

	UQuestSchemaGraphNode* GetPreviousNode() const;
	UQuestSchemaGraphNode* GetNextNode() const;
	void CreateConnections(UEdGraphPin* PreviousPin, UEdGraphPin* NextPin);

	UPROPERTY()
	class UQuestSchemaTransition* Transition;
};
