#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Types/QuestTypes.h"

class SDocURLWidget : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnURLTextChangedDelegate, const FString& /*NewURL*/);

	SLATE_BEGIN_ARGS(SDocURLWidget)
	{}
	
	SLATE_ARGUMENT(FDocURL, InitialItem)

	SLATE_EVENT(FOnURLTextChangedDelegate, OnURLTextChanged)

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

private:
	void OnURLTextChanged(const FText& NewURL);
	FReply OnButtonClicked();

	FOnURLTextChangedDelegate OnURLTextChangedDelegate;

	TSharedPtr<SEditableTextBox> TextBox;
};

