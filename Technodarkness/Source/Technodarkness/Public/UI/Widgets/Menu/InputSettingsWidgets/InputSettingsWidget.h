// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputSettingsWidget.generated.h"

class UButton;
class UInputListWidget;
class UInputSettingsAsset;
class UInputSettingsKeyboardWidget;

UENUM()
enum class EInputBindingTab : uint8
{
	None,
	BindingList,
	Keyboard
};

/**
 * Main widget for the input rebinding.
 */
UCLASS()
class TECHNODARKNESS_API UInputSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* OpenInputListTabButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OpenKeyboardTabButton;

	UPROPERTY(meta = (BindWidget))
	UInputListWidget* InputList;

	UPROPERTY(meta = (BindWidget))
	UInputSettingsKeyboardWidget* Keyboard;

private:
	UFUNCTION()
	void OnOpenBindingListTabButtonPressed();

	UFUNCTION()
	void OnOpenKeyboardTabButtonPressed();

	UFUNCTION()
	void OnInputListVisibilityChanged(ESlateVisibility InVisibility);

	UFUNCTION()
	void OnKeyboardVisibilityChanged(ESlateVisibility InVisibility);

	/** Open new tab and close the old one. */
	void ChangeTab(EInputBindingTab NewTab);
	
	// Current opened tab.
	EInputBindingTab CurrentTab = EInputBindingTab::BindingList;
};
