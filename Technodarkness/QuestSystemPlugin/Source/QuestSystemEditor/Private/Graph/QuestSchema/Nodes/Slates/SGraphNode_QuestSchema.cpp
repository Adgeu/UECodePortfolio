#include "Graph/QuestSchema/Nodes/Slates/SGraphNode_QuestSchema.h"

#include "GraphEditorSettings.h"
#include "QuestStyle.h"
#include "SCommentBubble.h"
#include "SGraphPanel.h"
#include "Graph/QuestSchema/Nodes/Slates/SGraphPin_QuestSchema.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "SLevelOfDetailBranchNode.h"
#include "Graph/QuestSchema/QuestSchemaColors.h"
#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode_Entry.h"
#include "QuestData/QuestObjective.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Base.h"
#include "QuestData/QuestSchema/Nodes/QuestSchemaNode_Objective.h"

#define LOCTEXT_NAMESPACE "QuestSchemaEditor"

/////////////////////////////////////////////////////////////////////////////////////
// SBehaviorTreeIndex

/** Widget for overlaying an execution-order index onto a node */
class SQuestSchemaIndex : public SCompoundWidget
{
public:
	/** Delegate event fired when the hover state of this widget changes */
	DECLARE_DELEGATE_OneParam(FOnHoverStateChanged, bool /* bHovered */);

	/** Delegate used to receive the color of the node, depending on hover state and state of other siblings */
	DECLARE_DELEGATE_RetVal_OneParam(FSlateColor, FOnGetIndexColor, bool /* bHovered */);

	SLATE_BEGIN_ARGS(SQuestSchemaIndex){}
		SLATE_ATTRIBUTE(FText, Text)
		SLATE_EVENT(FOnHoverStateChanged, OnHoverStateChanged)
		SLATE_EVENT(FOnGetIndexColor, OnGetIndexColor)
	SLATE_END_ARGS()

	void Construct( const FArguments& InArgs )
	{
		OnHoverStateChangedEvent = InArgs._OnHoverStateChanged;
		OnGetIndexColorEvent = InArgs._OnGetIndexColor;

		const FSlateBrush* IndexBrush = FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Index"));

		ChildSlot
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				// Add a dummy box here to make sure the widget doesnt get smaller than the brush
				SNew(SBox)
				.WidthOverride(IndexBrush->ImageSize.X)
				.HeightOverride(IndexBrush->ImageSize.Y)
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage(IndexBrush)
				.BorderBackgroundColor(this, &SQuestSchemaIndex::GetColor)
				.Padding(FMargin(4.0f, 0.0f, 4.0f, 1.0f))
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Text(InArgs._Text)
					.Font(FEditorStyle::GetFontStyle("BTEditor.Graph.BTNode.IndexText"))
				]
			]
		];
	}

	virtual void OnMouseEnter( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent ) override
	{
		OnHoverStateChangedEvent.ExecuteIfBound(true);
		SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
	}

	virtual void OnMouseLeave( const FPointerEvent& MouseEvent ) override
	{
		OnHoverStateChangedEvent.ExecuteIfBound(false);
		SCompoundWidget::OnMouseLeave(MouseEvent);
	}

	/** Get the color we use to display the rounded border */
	FSlateColor GetColor() const
	{
		if(OnGetIndexColorEvent.IsBound())
		{
			return OnGetIndexColorEvent.Execute(IsHovered());
		}

		return FSlateColor::UseForeground();
	}

private:
	/** Delegate event fired when the hover state of this widget changes */
	FOnHoverStateChanged OnHoverStateChangedEvent;

	/** Delegate used to receive the color of the node, depending on hover state and state of other siblings */
	FOnGetIndexColor OnGetIndexColorEvent;
};

/////////////////////////////////////////////////////////////////////////////////////
// SGraphNode_QuestSchema

void SGraphNode_QuestSchema::Construct(const FArguments& InArgs, UQuestSchemaGraphNode* InNode)
{
	SetCursor(EMouseCursor::Default);
	
	GraphNode = InNode;
	UpdateGraphNode();
}

void SGraphNode_QuestSchema::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	LeftNodeBox.Reset();
	RightNodeBox.Reset();

	const FSlateBrush* NodeTypeIcon = GetNameIcon();
	
	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	TWeakPtr<SNodeTitle> WeakNodeTitle = NodeTitle;
	auto GetNodeTitlePlaceholderWidth = [WeakNodeTitle]() -> FOptionalSize
	{
		TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
		const float DesiredWidth = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().X : 0.0f;
		return FMath::Max(75.0f, DesiredWidth);
	};
	auto GetNodeTitlePlaceholderHeight = [WeakNodeTitle]() -> FOptionalSize
	{
		TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
		const float DesiredHeight = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().Y : 0.0f;
		return FMath::Max(22.0f, DesiredHeight);
	};

	const FMargin NodePadding(8.0f, 20.0f);
	
	IndexOverlay = SNew(SQuestSchemaIndex)
			.ToolTipText(this, &SGraphNode_QuestSchema::GetIndexTooltipText)
			.Visibility(this, &SGraphNode_QuestSchema::GetIndexVisibility)
			.Text(this, &SGraphNode_QuestSchema::GetIndexText)
			.OnHoverStateChanged(this, &SGraphNode_QuestSchema::OnIndexHoverStateChanged)
			.OnGetIndexColor(this, &SGraphNode_QuestSchema::GetIndexColor);

	HiddenObjectiveOverlay = SNew(SImage)
			.Image(FQuestStyle::Get().GetBrush("QuestIcons.Hidden"))
			.Visibility(this, &SGraphNode_QuestSchema::GetObjectiveHiddenVisibility);


	ContentScale.Bind(this, &SGraphNode::GetContentScale);
	GetOrAddSlot(ENodeZone::Center)
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Center)
	[
		SNew(SBorder)
		.BorderImage( FEditorStyle::GetBrush( "Graph.StateNode.Body" ) )
		.Padding(0.0f)
		.BorderBackgroundColor(this, &SGraphNode_QuestSchema::GetBorderBackgroundColor)
		// .OnMouseButtonDown(this, &SGraphNode_QuestSchema::OnMouseDown)
		[
			SNew(SOverlay)

			// Pins and node details
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)

				// INPUT PIN AREA
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SBox)
					.MinDesiredHeight(NodePadding.Top)
					[
						SAssignNew(LeftNodeBox, SVerticalBox)
					]
				]

				// STATE NAME AREA
				+SVerticalBox::Slot()
				.Padding(FMargin(NodePadding.Left, 0.0f, NodePadding.Right, 0.0f))
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
					.BorderBackgroundColor(this, &SGraphNode_QuestSchema::GetBackgroundColor)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.Visibility(EVisibility::SelfHitTestInvisible)
					[
						SNew(SOverlay)
						+SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SVerticalBox)
							+SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SHorizontalBox)
								+SHorizontalBox::Slot()
								.AutoWidth()
								[
									// POPUP ERROR MESSAGE
									SAssignNew(ErrorText, SErrorText)
									.BackgroundColor(this, &SGraphNode_QuestSchema::GetErrorColor)
									.ToolTipText(this, &SGraphNode_QuestSchema::GetErrorMsgToolTip)
								]
								
								+SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SLevelOfDetailBranchNode)
									.UseLowDetailSlot(this, &SGraphNode_QuestSchema::UseLowDetailNodeTitles)
									.LowDetail()
									[
										SNew(SBox)
										.WidthOverride_Lambda(GetNodeTitlePlaceholderWidth)
										.HeightOverride_Lambda(GetNodeTitlePlaceholderHeight)
									]
									.HighDetail()
									[
										SNew(SHorizontalBox)
										+SHorizontalBox::Slot()
										.AutoWidth()
										.VAlign(VAlign_Center)
										[
											SNew(SBox)
											.HeightOverride(40.0f)
											.WidthOverride(40.0f)
											[
												SNew(SImage)
												.Image(this, &SGraphNode_QuestSchema::GetNameIcon)
											]
										]
										+SHorizontalBox::Slot()
										.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
										[
											SNew(SBox)
											.MinDesiredWidth(100.0f)
											[
												SNew(SVerticalBox)
												+SVerticalBox::Slot()
												.AutoHeight()
												[
													SAssignNew(InlineEditableText, SInlineEditableTextBlock)
													.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
													.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
													.OnVerifyTextChanged(this, &SGraphNode_QuestSchema::OnVerifyNameTextChanged)
													.OnTextCommitted(this, &SGraphNode_QuestSchema::OnNameTextCommited)
													.IsReadOnly(this, &SGraphNode_QuestSchema::IsNameReadOnly)
													.IsSelected(this, &SGraphNode_QuestSchema::IsSelectedExclusively)
												]
												+SVerticalBox::Slot()
												.AutoHeight()
												[
													NodeTitle.ToSharedRef()
												]
												+SVerticalBox::Slot()
												.AutoHeight()
												[
													// CLASS NAME
													SNew(STextBlock)
													.Visibility(this, &SGraphNode_QuestSchema::GetClassNameVisibility)
													.Text(this, &SGraphNode_QuestSchema::GetClassName)
												]
											]
										]
									]
								]
							]
							+SVerticalBox::Slot()
							.Padding(FMargin(0.0f, 5.0f, 0.0f, 0.0f))
							.AutoHeight()
							[
								// DESCRIPTION
								SNew(STextBlock)
								.Visibility(this, &SGraphNode_QuestSchema::GetDescriptionVisibility)
								.Text(this, &SGraphNode_QuestSchema::GetDescription)
							]
						]
					]
				]

				// OUTPUT PIN AREA
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SBox)
					.MinDesiredHeight(NodePadding.Bottom)
					[
						SAssignNew(RightNodeBox, SVerticalBox)
						+SVerticalBox::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Padding(20.0f,0.0f)
						.FillHeight(1.0f)
						[
							SAssignNew(OutputPinBox, SHorizontalBox)
						]
					]
				]
			]
		]
	];

	// Create comment bubble
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

	SAssignNew(CommentBubble, SCommentBubble)
	.GraphNode(GraphNode)
	.Text(this, &SGraphNode::GetNodeComment)
	.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
	.ColorAndOpacity(CommentColor)
	.AllowPinning(true)
	.EnableTitleBarBubble(true)
	.EnableBubbleCtrls(true)
	.GraphLOD(this, &SGraphNode::GetCurrentLOD)
	.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

	GetOrAddSlot(ENodeZone::TopCenter)
	.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
	.SlotSize( TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
	.AllowScaling( TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
	.VAlign(VAlign_Top)
	[
		CommentBubble.ToSharedRef()
	];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

void SGraphNode_QuestSchema::CreatePinWidgets()
{
	for (int32 PinIdx = 0; PinIdx < GraphNode->Pins.Num(); ++PinIdx)
	{
		UEdGraphPin* MyPin = GraphNode->Pins[PinIdx];
		if (!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SGraphPin_QuestSchema, MyPin)
				.ToolTipText( this, &SGraphNode_QuestSchema::GetPinTooltip, MyPin);

			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SGraphNode_QuestSchema::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility( TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced) );
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.0f)
		.Padding(20.0f,0.0f)
		[
			PinToAdd
		];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{
		float LeftPadding = 0.0f;
		float RightPadding = 0.0f;
		if (PinObj->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_ObjectiveOnSuccess)
		{
			RightPadding = 5.0f;
		}
		else if (PinObj->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_ObjectiveOnFail)
		{
			LeftPadding = 5.0f;
			RightPadding = 5.0f;
		}
		else if (PinObj->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_ObjectiveOnAny)
        {
        	LeftPadding = 5.0f;
        }
	
		OutputPinBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillWidth(1.0f)
		.Padding(LeftPadding, 0.0f, RightPadding, 0.0f)
		[
			PinToAdd
		];
		OutputPins.Add(PinToAdd);
	}
}

TArray<FOverlayWidgetInfo> SGraphNode_QuestSchema::GetOverlayWidgets(bool bSelected, const FVector2D& WidgetSize) const
{
	TArray<FOverlayWidgetInfo> Widgets;

	check(IndexOverlay.IsValid());
	FOverlayWidgetInfo IndexOverlayInfo(IndexOverlay);
	IndexOverlayInfo.OverlayOffset = FVector2D(WidgetSize.X - (IndexOverlay->GetDesiredSize().X * 0.5f), 0.0f);
	Widgets.Add(IndexOverlayInfo);

	check(HiddenObjectiveOverlay.IsValid())
	FOverlayWidgetInfo HiddenObjectiveOverlayInfo(HiddenObjectiveOverlay);
	HiddenObjectiveOverlayInfo.OverlayOffset = FVector2D(0.0f - HiddenObjectiveOverlay->GetDesiredSize().X * 0.5f, 0.0f);
	Widgets.Add(HiddenObjectiveOverlayInfo);

	return Widgets;
}

void SGraphNode_QuestSchema::MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty)
{
	SGraphNode::MoveTo(NewPosition, NodeFilter);

	UQuestSchemaGraphNode* QSGraphNode = CastChecked<UQuestSchemaGraphNode>(GraphNode);
	QSGraphNode->UpdateExecutionOrder();
}

TArray<UQuestSchemaGraphNode*> SGraphNode_QuestSchema::GetParentNodes() const
{
	const UQuestSchemaGraphNode* QSGraphNode = CastChecked<UQuestSchemaGraphNode>(GraphNode);
	
	TArray<UQuestSchemaGraphNode*> Result;
	for (const UEdGraphPin* Pin : QSGraphNode->GetInputPin()->LinkedTo)
	{
		if (UQuestSchemaGraphNode* ParentNode = Cast<UQuestSchemaGraphNode>(Pin->GetOwningNode()))
		{
			Result.Add(ParentNode);
		}
	}

	return Result;
}

const FSlateBrush* SGraphNode_QuestSchema::GetNameIcon() const
{
	UQuestSchemaGraphNode* QSGraphNode = Cast<UQuestSchemaGraphNode>(GraphNode);
	return QSGraphNode != nullptr ? FQuestStyle::Get().GetBrush(QSGraphNode->GetNameIcon()) : FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

FSlateColor SGraphNode_QuestSchema::GetBorderBackgroundColor() const
{
	return QuestSchemaColors::NodeBorder::Inactive;
}

FSlateColor SGraphNode_QuestSchema::GetBackgroundColor() const
{
	return GraphNode->IsA<UQuestSchemaGraphNode_Entry>() ? QuestSchemaColors::NodeBody::Entry : QuestSchemaColors::NodeBody::Default;
}

FText SGraphNode_QuestSchema::GetClassName() const
{
	const UQuestSchemaGraphNode* MyNode = CastChecked<UQuestSchemaGraphNode>(GraphNode);
	return MyNode ? MyNode->GetClassName() : FText::GetEmpty();
}

EVisibility SGraphNode_QuestSchema::GetClassNameVisibility() const
{
	const UQuestSchemaGraphNode* MyNode = CastChecked<UQuestSchemaGraphNode>(GraphNode);

	// LOD this out once things get too small
	const TSharedPtr<SGraphPanel> MyOwnerPanel = GetOwnerPanel();
	return !MyOwnerPanel.IsValid() || MyOwnerPanel->GetCurrentLOD() > EGraphRenderingLOD::LowDetail ? EVisibility::Visible : EVisibility::Collapsed;
}

FText SGraphNode_QuestSchema::GetDescription() const
{
	const UQuestSchemaGraphNode* MyNode = CastChecked<UQuestSchemaGraphNode>(GraphNode);
	return MyNode ? MyNode->GetDescription() : FText::GetEmpty();
}

EVisibility SGraphNode_QuestSchema::GetDescriptionVisibility() const
{
	const UQuestSchemaGraphNode* MyNode = CastChecked<UQuestSchemaGraphNode>(GraphNode);
	const bool bShouldShow = !MyNode->GetDescription().IsEmpty();

	// LOD this out once things get too small
	const TSharedPtr<SGraphPanel> MyOwnerPanel = GetOwnerPanel();
	return bShouldShow && (!MyOwnerPanel.IsValid() || MyOwnerPanel->GetCurrentLOD() > EGraphRenderingLOD::LowDetail) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility SGraphNode_QuestSchema::GetIndexVisibility() const
{
	const UQuestSchemaGraphNode* MyNode = CastChecked<UQuestSchemaGraphNode>(GraphNode);

	bool bShowIndex = false;
	if (const UEdGraphPin* InputPin = MyNode->GetInputPin())
	{
		for (const UEdGraphPin* Pin : InputPin->LinkedTo)
		{
			bShowIndex |= UQuestSchemaEditorTypes::IsNodeOrderImportant(Pin->PinType.PinCategory);
		}
	}

	// LOD this out once things get too small
	const TSharedPtr<SGraphPanel> MyOwnerPanel = GetOwnerPanel();
	return bShowIndex && (!MyOwnerPanel.IsValid() || MyOwnerPanel->GetCurrentLOD() > EGraphRenderingLOD::LowDetail) ? EVisibility::Visible : EVisibility::Collapsed;
}

FText SGraphNode_QuestSchema::GetIndexText() const
{
	return FText::AsNumber(CastChecked<UQuestSchemaGraphNode>(GraphNode)->NodeInstance->ExecutionIndex);
}

FText SGraphNode_QuestSchema::GetIndexTooltipText() const
{
	return LOCTEXT("ExecutionIndexTooltip", "Execution index: this shows the order in which nodes are executed.");
}

FSlateColor SGraphNode_QuestSchema::GetIndexColor(bool bHovered) const
{
	bool bHighlightHover = false;
	for (const UQuestSchemaGraphNode* ParentNode : GetParentNodes())
	{
		bHighlightHover |= bHovered || (ParentNode && ParentNode->bHighlightChildNodeIndices);
	}
	
	constexpr FLinearColor HoveredColor(1.0f, 0.0f, 0.0f);
	constexpr FLinearColor DefaultColor(0.15f, 0.15f, 0.15f);

	return bHighlightHover ? HoveredColor : DefaultColor;
}

void SGraphNode_QuestSchema::OnIndexHoverStateChanged(bool bHovered)
{
	for (UQuestSchemaGraphNode* ParentNode : GetParentNodes())
	{
		ParentNode->bHighlightChildNodeIndices = bHovered;
	}
}

EVisibility SGraphNode_QuestSchema::GetObjectiveHiddenVisibility() const
{
	const UQuestSchemaNode_Objective* ObjectiveNode = Cast<UQuestSchemaNode_Objective>(CastChecked<UQuestSchemaGraphNode>(GraphNode)->NodeInstance);
	return ObjectiveNode && ObjectiveNode->GetObjective()->IsObjectiveHidden() ? EVisibility::Visible : EVisibility::Collapsed;
}

FText SGraphNode_QuestSchema::GetPinTooltip(UEdGraphPin* GraphPinObj) const
{
	FText HoverText = FText::GetEmpty();

	check(GraphPinObj);
	UEdGraphNode* OwningGraphNode = GraphPinObj->GetOwningNode();
	if (OwningGraphNode)
	{
		FString HoverStr;
		OwningGraphNode->GetPinHoverText(*GraphPinObj, /*out*/HoverStr);
		if (!HoverStr.IsEmpty())
		{
			HoverText = FText::FromString(HoverStr);
		}
	}

	return HoverText;
}

#undef LOCTEXT_NAMESPACE
