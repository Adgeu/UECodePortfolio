// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "UI/Widgets/Menu/InputSettingsWidgets/InputSettingsWidget.h"

#include "Components/Button.h"
#include "UI/Widgets/Menu/InputSettingsWidgets/InputListWidget.h"
#include "UI/Widgets/Menu/InputSettingsWidgets/InputSettingsKeyboardWidget.h"

void UInputSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OpenInputListTabButton->OnClicked.AddDynamic(this, &UInputSettingsWidget::OnOpenBindingListTabButtonPressed);
	OpenKeyboardTabButton->OnClicked.AddDynamic(this, &UInputSettingsWidget::OnOpenKeyboardTabButtonPressed);
	InputList->OnVisibilityChanged.AddDynamic(this, &UInputSettingsWidget::OnInputListVisibilityChanged);
	Keyboard->OnVisibilityChanged.AddDynamic(this, &UInputSettingsWidget::OnKeyboardVisibilityChanged);
	
	ChangeTab(EInputBindingTab::BindingList);
}

void UInputSettingsWidget::OnOpenBindingListTabButtonPressed()
{
	ChangeTab(EInputBindingTab::BindingList);
}

void UInputSettingsWidget::OnOpenKeyboardTabButtonPressed()
{
	ChangeTab(EInputBindingTab::Keyboard);
}

void UInputSettingsWidget::OnInputListVisibilityChanged(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		InputList->RefreshList();
	}
}

void UInputSettingsWidget::OnKeyboardVisibilityChanged(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		Keyboard->UpdateKeyboardKeyButtons();
	}
}

void UInputSettingsWidget::ChangeTab(EInputBindingTab NewTab)
{
	CurrentTab = NewTab;
	
	InputList->SetVisibility(NewTab == EInputBindingTab::BindingList ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	Keyboard->SetVisibility(NewTab == EInputBindingTab::Keyboard ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
