// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "QuestSchemaEditorTypes.generated.h"

struct FCompareNodeXLocation
{
	FORCEINLINE bool operator()(const UEdGraphPin& A, const UEdGraphPin& B) const
	{
		const UEdGraphNode* NodeA = A.GetOwningNode();
		const UEdGraphNode* NodeB = B.GetOwningNode();

		if (NodeA->NodePosX == NodeB->NodePosX)
		{
			return NodeA->NodePosY < NodeB->NodePosY;
		}

		return NodeA->NodePosX < NodeB->NodePosX;
	}
};

UCLASS()
class UQuestSchemaEditorTypes : public UObject
{
	GENERATED_BODY()

public:
	static const FName PinCategory_MultipleNodes;
	static const FName PinCategory_MultipleObjectivesOnly;
	static const FName PinCategory_MultipleWithCheck;
	static const FName PinCategory_MultipleWithWeight;
	static const FName PinCategory_SingleNode;
	static const FName PinCategory_ObjectiveOnAny;
	static const FName PinCategory_ObjectiveOnSuccess;
	static const FName PinCategory_ObjectiveOnFail;

	static bool AllowMultipleNodesInPin(const FName& InPinCategory);
	static bool ShouldCreateTransitionNode(const FName& InPinCategory);
	static bool IsNodeOrderImportant(const FName& ForPinCategory);
	static int32 GetPinIndexByPinCategory(const FName& InPinCategory);
};
