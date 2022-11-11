// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "UI/Widgets/Menu/InputSettingsWidgets/KeyboardKeyWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UKeyboardKeyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	KeyButton->OnClicked.AddDynamic(this, &UKeyboardKeyWidget::OnButtonClicked);
}

void UKeyboardKeyWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	ButtonSize->SetWidthOverride(ButtonWidth);
	ButtonSize->SetHeightOverride(ButtonHeight);
	
	KeyText->SetFont(CaptionFont);

	if (bOverrideCaptionText)
	{
		KeyText->SetText(CaptionText);
	}
	else
	{
		KeyText->SetText(FText::FromString(Key.ToString()));
	}

	
	if (CaptionImage)
	{
		KeyImage->SetBrushFromTexture(CaptionImage);
		KeyImage->SetRenderTransformAngle(CaptionImageRotationAngle);
	}
	else
	{
		FSlateBrush SlateBrush;
		SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
		KeyImage->SetBrush(SlateBrush);
	}

	KeyButton->SetStyle(NormalStyle);
}

void UKeyboardKeyWidget::Deselect()
{
	bSelected = false;
	ResetButtonStyle();
}

void UKeyboardKeyWidget::SetHasConflict(bool bHasConflict_In)
{
	bHasConflict = bHasConflict_In;
	ResetButtonStyle();
}

void UKeyboardKeyWidget::SetHasBindedInput(bool bHasBindedInput_In)
{
	bHasBindedInput = bHasBindedInput_In;
	ResetButtonStyle();
}

void UKeyboardKeyWidget::OnButtonClicked()
{
	if (!bSelected)
	{
		bSelected = true;
		ResetButtonStyle();
		OnSelected.ExecuteIfBound(this);
	}
}

void UKeyboardKeyWidget::ResetButtonStyle()
{
	if (bHasConflict)
	{
		KeyButton->SetStyle(bSelected ? SelectedConflictStyle : NormalConflictStyle);
	}
	else if (bHasBindedInput)
	{
		KeyButton->SetStyle(bSelected ? SelectedBindedStyle : NormalBindedStyle);
	}
	else
	{
		KeyButton->SetStyle(bSelected ? SelectedStyle : NormalStyle);
	}
}
