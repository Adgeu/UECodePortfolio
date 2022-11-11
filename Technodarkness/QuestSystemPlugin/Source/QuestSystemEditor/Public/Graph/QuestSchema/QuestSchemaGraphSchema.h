// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "QuestSchemaGraphSchema.generated.h"

struct FCanCreateConnectionBetweenNodes
{
	static FORCEINLINE FPinConnectionResponse Check(const UEdGraphPin* A, const UEdGraphPin* B)
	{
		FPinConnectionResponse Result;
		if (!NotSameDirections(A, B, Result)) { return Result; }
		if (!NotSameNode(A, B, Result)) { return Result; }
		if (!CheckForMultipleConnections(A, B, Result)) { return Result; }
		if (!CheckForOnlyObjectivesPin(A, B, Result)) { return Result; }
		if (!CheckImportantOrderPins(A, B, Result)) { return Result; }
		if (!CheckForCycling(A, B, Result)) { return Result; }
		return Result;
	}

private:
	/** Make sure the input is connecting to an output. */
	static bool NotSameDirections(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse);
	/** Make sure not same node. */
	static bool NotSameNode(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse);
	/** Make sure we've got right linked nodes count. */
	static bool CheckForMultipleConnections(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse);
	/** Make sure we trying connect objective node if we can connect only objectives nodes. */
	static bool CheckForOnlyObjectivesPin(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse);
	/** Make sure we do not connecting output with important order to node that have already connection with important order. */
	static bool CheckImportantOrderPins(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse);
	/** Checks for cycling */
	static bool CheckForCycling(const UEdGraphPin* A, const UEdGraphPin* B, FPinConnectionResponse& OutResponse);
};

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * 
 */
UCLASS()
class UQuestSchemaGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	// UEdGraphSchema interface
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override;
	virtual int32 GetCurrentVisualizationCacheID() const override;
	virtual void ForceVisualizationCacheClear() const override;
	virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;

	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const override;
	virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;
	// ~UEdGraphSchema interface

	void UpdateExecutionIndexForLinkedNodes(UEdGraphPin* InPin) const;
	
private:
	// ID for checking dirty status of node titles against, increases whenever 
	static int32 CurrentCacheRefreshID;
};
