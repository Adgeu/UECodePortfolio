#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "Nodes/QuestSchemaGraphNode.h"
#include "QuestSchemaGraphSchemaActions.generated.h"

class UQuestSchema;

///////////////////////////////////////////////////////////////////////////////////

/** Action to add a comment to the graph */
USTRUCT()
struct FQuestSchemaGraphSchemaAction_AddComment : public FEdGraphSchemaAction
{
	GENERATED_BODY()
	
	FQuestSchemaGraphSchemaAction_AddComment() : FEdGraphSchemaAction() {}
	FQuestSchemaGraphSchemaAction_AddComment(FText InDescription, FText InToolTip)
		: FEdGraphSchemaAction(FText(), MoveTemp(InDescription), MoveTemp(InToolTip), 0)
	{
	}

	// FEdGraphSchemaAction interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override final;
	// End of FEdGraphSchemaAction interface
};

///////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct FQuestSchemaGraphSchemaAction_CreateObjectiveNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FQuestSchemaGraphSchemaAction_CreateObjectiveNode() {};

	FQuestSchemaGraphSchemaAction_CreateObjectiveNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, UQuestSchema* InQuestSchema)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping), QuestSchema(InQuestSchema)
	{}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

private:
	UQuestSchema* QuestSchema = nullptr;
};

///////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct FQuestSchemaGraphSchemaAction_CreateEffectNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FQuestSchemaGraphSchemaAction_CreateEffectNode() {};

	FQuestSchemaGraphSchemaAction_CreateEffectNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, UQuestSchema* InQuestSchema)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping), QuestSchema(InQuestSchema)
	{}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

private:
	UQuestSchema* QuestSchema = nullptr;
};

///////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct FQuestSchemaGraphSchemaAction_CreateExitNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FQuestSchemaGraphSchemaAction_CreateExitNode() {};

	FQuestSchemaGraphSchemaAction_CreateExitNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, UQuestSchema* InQuestSchema)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping), QuestSchema(InQuestSchema)
	{}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

private:
	UQuestSchema* QuestSchema = nullptr;
};

///////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct FQuestSchemaGraphSchemaAction_CreateSequenceNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FQuestSchemaGraphSchemaAction_CreateSequenceNode() {};

	FQuestSchemaGraphSchemaAction_CreateSequenceNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, UQuestSchema* InQuestSchema)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping), QuestSchema(InQuestSchema)
	{}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

private:
	UQuestSchema* QuestSchema = nullptr;
};

///////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct FQuestSchemaGraphSchemaAction_CreateSelectorNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FQuestSchemaGraphSchemaAction_CreateSelectorNode() {};

	FQuestSchemaGraphSchemaAction_CreateSelectorNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, UQuestSchema* InQuestSchema)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping), QuestSchema(InQuestSchema)
	{}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

private:
	UQuestSchema* QuestSchema = nullptr;
};

///////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct FQuestSchemaGraphSchemaAction_CreateRandomNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FQuestSchemaGraphSchemaAction_CreateRandomNode() {};

	FQuestSchemaGraphSchemaAction_CreateRandomNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, UQuestSchema* InQuestSchema)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping), QuestSchema(InQuestSchema)
	{}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

private:
	UQuestSchema* QuestSchema = nullptr;
};

///////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct FQuestSchemaGraphSchemaAction_CreateWaitForAllFlowsNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FQuestSchemaGraphSchemaAction_CreateWaitForAllFlowsNode() {};

	FQuestSchemaGraphSchemaAction_CreateWaitForAllFlowsNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, UQuestSchema* InQuestSchema)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping), QuestSchema(InQuestSchema)
	{}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

private:
	UQuestSchema* QuestSchema = nullptr;
};

///////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct FQuestSchemaGraphSchemaAction_CreateObjectiveXORCompletionNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FQuestSchemaGraphSchemaAction_CreateObjectiveXORCompletionNode() {};

	FQuestSchemaGraphSchemaAction_CreateObjectiveXORCompletionNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, UQuestSchema* InQuestSchema)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping), QuestSchema(InQuestSchema)
	{}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

private:
	UQuestSchema* QuestSchema = nullptr;
};