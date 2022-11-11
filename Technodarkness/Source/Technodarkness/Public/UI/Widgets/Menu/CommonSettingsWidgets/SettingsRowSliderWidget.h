// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsRowSliderWidget.generated.h"

class UTextBlock;
class USlider;

DECLARE_DELEGATE_OneParam(FOnRowSliderValueChanged, float /*NewValue*/);

/**
 * WIP
 */
UCLASS()
class TECHNODARKNESS_API USettingsRowSliderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void InitializeSliderValue(float Value);

	FOnRowSliderValueChanged OnRowSliderValueChanged;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RowName;

	UPROPERTY(meta = (BindWidget))
	USlider* RowSlider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Row Name")
	FText DisplayRowName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider Settings")
	float SliderMinValue = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider Settings")
	float SliderMaxValue = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider Settings", meta = (ClampMin = 0.0))
	float SliderStepSize = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider Settings")
	bool bShowValueInName = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider Settings", meta = (EditCondition = "bShowValueInName", ClampMin = 0))
	int32 ValueFractionalDigits = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider Settings", meta = (EditCondition = "bShowValueInName"))
	bool bIsPercentageValue = false;

private:
	UFUNCTION()
	void OnSliderValueChanged(float NewValue);
	
	FText GetRowDisplayText() const;

	bool bIsValueInitialized = false;
};
