#pragma once

#include "SGraphNode.h"

class UQuestSchemaGraphNode;

class SGraphNode_QuestSchema : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_QuestSchema)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UQuestSchemaGraphNode* InNode);

	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual TArray<FOverlayWidgetInfo> GetOverlayWidgets(bool bSelected, const FVector2D& WidgetSize) const override;
	// ~SGraphNode interface

	// SNodePanel::SNode interface
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty) override;
	// ~SNodePanel::SNode interface

protected:
	TArray<UQuestSchemaGraphNode*> GetParentNodes() const;
	
	const FSlateBrush* GetNameIcon() const;

	FSlateColor GetBorderBackgroundColor() const;
	FSlateColor GetBackgroundColor() const;

	virtual FText GetClassName() const;
	virtual EVisibility GetClassNameVisibility() const;
	
	virtual FText GetDescription() const;
	virtual EVisibility GetDescriptionVisibility() const;

	/** Get the visibility of the index overlay */
	EVisibility GetIndexVisibility() const;

	/** Get the text to display in the index overlay */
	FText GetIndexText() const;

	/** Get the tooltip for the index overlay */
	FText GetIndexTooltipText() const;

	/** Get the color to display for the index overlay. This changes on hover state of sibling nodes */
	FSlateColor GetIndexColor(bool bHovered) const;

	/** Handle hover state changing for the index widget - we use this to highlight sibling nodes */
	void OnIndexHoverStateChanged(bool bHovered);

	EVisibility GetObjectiveHiddenVisibility() const;

	FText GetPinTooltip(UEdGraphPin* GraphPinObj) const;

protected:
	TSharedPtr<SHorizontalBox> OutputPinBox;

	/** The widget we use to display the index of the node */
	TSharedPtr<SWidget> IndexOverlay;

	/** The widget we use to display if the objective hidden */
	TSharedPtr<SWidget> HiddenObjectiveOverlay;
};
