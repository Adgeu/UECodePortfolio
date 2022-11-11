// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyboardKeyWidget.generated.h"

class UKeyboardKeyWidget;

// Key button was selected.
DECLARE_DELEGATE_OneParam(FOnKeyboardButtonSelected, UKeyboardKeyWidget* /*Sender*/);

class UButton;
class UTextBlock;
class USizeBox;
class UImage;

/**
 * Key button for virtual keyboard widget.
 * @see UInputSettingsKeyboardWidget
 */
UCLASS()
class TECHNODARKNESS_API UKeyboardKeyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	/** Deselect the key button. */
	void Deselect();

	/** Sets associated key has conflict. */
	void SetHasConflict(bool bHasConflict_In);

	/** Sets associated key has binded input. */
	void SetHasBindedInput(bool bHasBindedInput_In);

	/** Returns key that associated with the key button. */
	FORCEINLINE const FKey& GetKey() const
	{
		return Key;
	}

	// Key button was selected.
	FOnKeyboardButtonSelected OnSelected;
	
protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ButtonSize;
	
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KeyText;

	UPROPERTY(meta = (BindWidget))
	UImage* KeyImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FKey Key;
	
	// Button size properties.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Size")
	float ButtonWidth = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Size")
	float ButtonHeight = 60.0f;
	// ~Button size properties.

	// Button caption properties.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Caption")
	bool bOverrideCaptionText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Caption", meta = (EditCondition = "bOverrideCaptionText", EditConditionHides))
	FText CaptionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Caption", meta = (EditCondition = "bOverrideCaptionFont", EditConditionHides))
	FSlateFontInfo CaptionFont;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Caption | Image")
	UTexture2D* CaptionImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Caption | Image")
	float CaptionImageRotationAngle = 0.0f;
	// ~Button caption properties.

	// Button styles for different states.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button Appeareance | Normal")
	FButtonStyle NormalStyle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button Appeareance | Normal")
	FButtonStyle NormalBindedStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button Appeareance | Normal")
	FButtonStyle NormalConflictStyle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button Appeareance | Selected")
	FButtonStyle SelectedStyle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button Appeareance | Selected")
	FButtonStyle SelectedBindedStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button Appeareance | Selected")
	FButtonStyle SelectedConflictStyle;
	// ~Button styles for different states.

private:
	UFUNCTION()
	void OnButtonClicked();
	
	/** Reset button style based on states. */
	void ResetButtonStyle();

	// Is button selected
	uint8 bSelected : 1;

	// Is button has binded input with associated key.
	uint8 bHasBindedInput : 1;

	// Is associated key has any conflicts in inputs.
	uint8 bHasConflict : 1;
};
