#include "Graph/QuestSchema/Nodes/Slates/SGraphPin_QuestSchema.h"

#include "Graph/QuestSchema/QuestSchemaColors.h"
#include "Graph/QuestSchema/QuestSchemaEditorTypes.h"

void SGraphPin_QuestSchema::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SetCursor(EMouseCursor::Default);

	bShowLabel = true;
	IsEditable = true;

	GraphPinObj = InPin;
	check(GraphPinObj);

	const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
	check(Schema);

	// Set up a hover for pins that is tinted the color of the pin.
	SBorder::Construct(SBorder::FArguments()
		.BorderImage(this, &SGraphPin_QuestSchema::GetPinBorder)
		.BorderBackgroundColor(this, &SGraphPin_QuestSchema::GetPinColor)
		.OnMouseButtonDown(this, &SGraphPin_QuestSchema::OnPinMouseDown)
		.Cursor(this, &SGraphPin_QuestSchema::GetPinCursor)
		.Padding(FMargin(5.0f))
		);
}

TSharedRef<SWidget> SGraphPin_QuestSchema::GetDefaultValueWidget()
{
	return SNew(STextBlock);
}

FSlateColor SGraphPin_QuestSchema::GetPinColor() const
{
	return
		IsHovered() ? QuestSchemaColors::Pin::Hover :
		GraphPinObj->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_MultipleWithCheck ? QuestSchemaColors::Pin::MultipleWithCheck :
		GraphPinObj->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_MultipleWithWeight ? QuestSchemaColors::Pin::MultipleWithWeight :
		GraphPinObj->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_MultipleObjectivesOnly ? QuestSchemaColors::Pin::MultipleObjectivesOnly :
		GraphPinObj->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_SingleNode ? QuestSchemaColors::Pin::SingleNode :
		GraphPinObj->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_ObjectiveOnAny ? QuestSchemaColors::Pin::ObjectiveAny :
		GraphPinObj->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_ObjectiveOnSuccess ? QuestSchemaColors::Pin::ObjectiveSuccess :
		GraphPinObj->PinType.PinCategory == UQuestSchemaEditorTypes::PinCategory_ObjectiveOnFail ? QuestSchemaColors::Pin::ObjectiveFail :
		QuestSchemaColors::Pin::Default;
}

const FSlateBrush* SGraphPin_QuestSchema::GetPinBorder() const
{
	return FEditorStyle::GetBrush(TEXT("Graph.StateNode.Body"));
}

