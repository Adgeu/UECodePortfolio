// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaGraphNode.h"
#include "QuestSchemaGraphNode_Objective.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEMEDITOR_API UQuestSchemaGraphNode_Objective : public UQuestSchemaGraphNode
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;
	virtual void GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const override;

	virtual FName GetNameIcon() const override;
};
