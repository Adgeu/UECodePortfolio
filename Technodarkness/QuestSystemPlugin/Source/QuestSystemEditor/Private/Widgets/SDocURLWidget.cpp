#include "Widgets/SDocURLWidget.h"

#include "Kismet/KismetSystemLibrary.h"

void SDocURLWidget::Construct(const FArguments& InArgs)
{
	OnURLTextChangedDelegate = InArgs._OnURLTextChanged;
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		
		+SHorizontalBox::Slot()
		[
			SAssignNew(TextBox, SEditableTextBox)
			.Text(FText::FromString(InArgs._InitialItem.URL))
			.OnTextChanged(this, &SDocURLWidget::OnURLTextChanged)
		]
		
		+SHorizontalBox::Slot()
		[
			SNew(SButton)
			.OnClicked(this, &SDocURLWidget::OnButtonClicked)
			.HAlign(HAlign_Center)
			.Content()
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Open")))
			]
		]
	];
}

void SDocURLWidget::OnURLTextChanged(const FText& NewURL)
{
	OnURLTextChangedDelegate.ExecuteIfBound(NewURL.ToString());
}

FReply SDocURLWidget::OnButtonClicked()
{
	UKismetSystemLibrary::LaunchURL(TextBox->GetText().ToString());
	return FReply::Handled();
}
