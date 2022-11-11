#pragma once

#include "SGraphPin.h"

class SGraphPin_QuestSchema : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SGraphPin_QuestSchema) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
	
protected:
	// SGraphPin interface
	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;
	virtual FSlateColor GetPinColor() const override;
	// ~SGraphPin interface

	const FSlateBrush* GetPinBorder() const;
};
