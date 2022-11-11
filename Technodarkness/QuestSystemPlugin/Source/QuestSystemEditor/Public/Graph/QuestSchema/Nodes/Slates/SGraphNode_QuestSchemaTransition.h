#pragma once

#include "SGraphNode.h"

class UQuestSchemaTransitionGraphNode;

class SGraphNode_QuestSchemaTransition : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_QuestSchemaTransition)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UQuestSchemaTransitionGraphNode* InNode);
	
	// SNodePanel::SNode interface
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty) override;
	virtual bool RequiresSecondPassLayout() const override;
	virtual void PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const override;
	// End of SNodePanel::SNode interface
	
	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	// End of SGraphNode interface

	// SWidget interface
	void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	// End of SWidget interface

private:
	/** Calculate position for node to be placed between a start and end point. */
	void PositionBetweenTwoNodes(const FGeometry& StartGeom, const FGeometry& EndGeom) const;

	FSlateColor GetTransitionColor() const;
	const FSlateBrush* GetTransitionIcon() const;
};
