// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSchemaGraphNode.h"
#include "QuestSchemaGraphNode_Random.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEMEDITOR_API UQuestSchemaGraphNode_Random : public UQuestSchemaGraphNode
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;

	virtual FName GetNameIcon() const override;
};
