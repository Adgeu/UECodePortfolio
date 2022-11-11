#include "Graph/QuestSchema/Nodes/Slates/SGraphNode_QuestSchemaTransition.h"

#include "ConnectionDrawingPolicy.h"
#include "QuestStyle.h"
#include "SGraphPanel.h"
#include "Graph/QuestSchema/QuestSchemaColors.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaGraphNode.h"
#include "Graph/QuestSchema/Nodes/QuestSchemaTransitionGraphNode.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransition.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransitionWithCheck.h"
#include "QuestData/QuestSchema/Transitions/QuestSchemaTransitionWithWeight.h"

void SGraphNode_QuestSchemaTransition::Construct(const FArguments& InArgs, UQuestSchemaTransitionGraphNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

void SGraphNode_QuestSchemaTransition::MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty)
{
	// Ignored; position is set by the location of the attached quest schema nodes
}

bool SGraphNode_QuestSchemaTransition::RequiresSecondPassLayout() const
{
	return true;
}

void SGraphNode_QuestSchemaTransition::PerformSecondPassLayout(const TMap<UObject*, TSharedRef<SNode>>& NodeToWidgetLookup) const
{
	const UQuestSchemaTransitionGraphNode* TransNode = CastChecked<UQuestSchemaTransitionGraphNode>(GraphNode);

	// Find the geometry of the state nodes we're connecting
	FGeometry StartGeom;
	FGeometry EndGeom;

	const UQuestSchemaGraphNode* PrevNode = TransNode->GetPreviousNode();
	const UQuestSchemaGraphNode* NextNode = TransNode->GetNextNode();
	if (PrevNode && NextNode)
	{
		const TSharedRef<SNode>* pPrevNodeWidget = NodeToWidgetLookup.Find(PrevNode);
		const TSharedRef<SNode>* pNextNodeWidget = NodeToWidgetLookup.Find(NextNode);
		if (pPrevNodeWidget && pNextNodeWidget)
		{
			const TSharedRef<SNode>& PrevNodeWidget = *pPrevNodeWidget;
			const TSharedRef<SNode>& NextNodeWidget = *pNextNodeWidget;

			StartGeom = FGeometry(FVector2D(PrevNode->NodePosX, PrevNode->NodePosY), FVector2D::ZeroVector, PrevNodeWidget->GetDesiredSize(), 1.0f);
			EndGeom = FGeometry(FVector2D(NextNode->NodePosX, NextNode->NodePosY), FVector2D::ZeroVector, NextNodeWidget->GetDesiredSize(), 1.0f);
		}
	}

	//Position Node
	PositionBetweenTwoNodes(StartGeom, EndGeom);
}

void SGraphNode_QuestSchemaTransition::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	ContentScale.Bind(this, &SGraphNode::GetContentScale);
	GetOrAddSlot(ENodeZone::Center)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
		.BorderBackgroundColor(FLinearColor::White)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.Visibility(EVisibility::SelfHitTestInvisible)
		[
			SNew(SBox)
			.HeightOverride(32.0f)
			.WidthOverride(32.0f)
			[
				SNew(SImage)
				.Image(this, &SGraphNode_QuestSchemaTransition::GetTransitionIcon)
				.ColorAndOpacity(this, &SGraphNode_QuestSchemaTransition::GetTransitionColor)
			]
		]
	];
}

void SGraphNode_QuestSchemaTransition::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UQuestSchemaTransitionGraphNode* TransNode = CastChecked<UQuestSchemaTransitionGraphNode>(GraphNode);
	if (UEdGraphPin* Pin = TransNode->Pins[0])
	{
		GetOwnerPanel()->AddPinToHoverSet(Pin);
	}

	SGraphNode::OnMouseEnter(MyGeometry, MouseEvent);
}

void SGraphNode_QuestSchemaTransition::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	UQuestSchemaTransitionGraphNode* TransNode = CastChecked<UQuestSchemaTransitionGraphNode>(GraphNode);
	if (UEdGraphPin* Pin = TransNode->Pins[0])
	{
		GetOwnerPanel()->RemovePinFromHoverSet(Pin);
	}

	SGraphNode::OnMouseLeave(MouseEvent);
}

void SGraphNode_QuestSchemaTransition::PositionBetweenTwoNodes(const FGeometry& StartGeom, const FGeometry& EndGeom) const
{
	// Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	// Find the (approximate) closest points between the two boxes
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	// Position ourselves halfway along the connecting line between the nodes, elevated away perpendicular to the direction of the line
	constexpr float Height = 20.0f;

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);

	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();
	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + (Height * Normal);

	GraphNode->NodePosX = NewCenter.X;
	GraphNode->NodePosY = NewCenter.Y;
}

FSlateColor SGraphNode_QuestSchemaTransition::GetTransitionColor() const
{
	const UQuestSchemaTransitionGraphNode* TransNode = CastChecked<UQuestSchemaTransitionGraphNode>(GraphNode);
	return
		IsHovered() ?  QuestSchemaColors::Transition::Hovered :
		TransNode->Transition->IsA<UQuestSchemaTransitionWithCheck>() ? QuestSchemaColors::Transition::Checkable :
		TransNode->Transition->IsA<UQuestSchemaTransitionWithWeight>() ? QuestSchemaColors::Transition::Weightable :
		QuestSchemaColors::Transition::Default;
}

const FSlateBrush* SGraphNode_QuestSchemaTransition::GetTransitionIcon() const
{
	const UQuestSchemaTransitionGraphNode* TransNode = CastChecked<UQuestSchemaTransitionGraphNode>(GraphNode);
	return
		TransNode->Transition->IsA<UQuestSchemaTransitionWithCheck>() ? FQuestStyle::Get().GetBrush("QuestIcons.Check") :
		TransNode->Transition->IsA<UQuestSchemaTransitionWithWeight>() ? FQuestStyle::Get().GetBrush("QuestIcons.Weight") :
		nullptr;
}
