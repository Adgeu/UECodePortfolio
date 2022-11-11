// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaGraphNode.h"
#include "QuestSchemaGraphNode_Entry.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEMEDITOR_API UQuestSchemaGraphNode_Entry : public UQuestSchemaGraphNode
{
	GENERATED_BODY()

public:
	UQuestSchemaGraphNode_Entry();
	
	virtual void AllocateDefaultPins() override;
	virtual bool CanDuplicateNode() const override { return false; }
	virtual bool CanUserDeleteNode() const override { return false; }
	virtual bool GetCanRenameNode() const override { return false; }
	virtual FName GetNameIcon() const override;
	virtual FText GetTooltipText() const override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
};
