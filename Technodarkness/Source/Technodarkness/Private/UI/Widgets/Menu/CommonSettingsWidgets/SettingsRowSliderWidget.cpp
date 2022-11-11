// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Menu/CommonSettingsWidgets/SettingsRowSliderWidget.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"

void USettingsRowSliderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RowSlider->OnValueChanged.AddDynamic(this, &USettingsRowSliderWidget::OnSliderValueChanged);
}

void USettingsRowSliderWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	RowName->SetText(GetRowDisplayText());

	RowSlider->SetMinValue(SliderMinValue);
	RowSlider->SetMaxValue(SliderMaxValue);
	RowSlider->SetStepSize(SliderStepSize);
}

void USettingsRowSliderWidget::InitializeSliderValue(float Value)
{
	RowSlider->SetValue(Value);
	bIsValueInitialized = true;
}

void USettingsRowSliderWidget::OnSliderValueChanged(float NewValue)
{
	RowName->SetText(GetRowDisplayText());

	if (bIsValueInitialized)
	{
		OnRowSliderValueChanged.ExecuteIfBound(NewValue);
	}
}

FText USettingsRowSliderWidget::GetRowDisplayText() const
{
	FText Result;
	
	if (!bShowValueInName)
	{
		Result = DisplayRowName;
	}
	else
	{
		FString SliderValueText = FString::SanitizeFloat(RowSlider->GetValue(), ValueFractionalDigits);
		if (bIsPercentageValue)
		{
			SliderValueText.AppendChar('%');
		}
		Result = FText::FromString(FString::Printf(TEXT("%s (%s)"), *DisplayRowName.ToString(), *SliderValueText));
	}

	return Result;
}
