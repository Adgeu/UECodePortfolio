// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "UI/Widgets/Menu/InputSettingsWidgets/InputRowWidget.h"

#include "CPPGameInstance.h"
#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"
#include "Subsystems/InputRebindSubsystem.h"


void UInputRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FirstSlotKeySelector->OnKeySelected.AddDynamic(this, &UInputRowWidget::OnFirstSlotKeySelected);
	SecondSlotKeySelector->OnKeySelected.AddDynamic(this, &UInputRowWidget::OnSecondSlotKeySelected);

	NormalColor = InputNameText->ColorAndOpacity;
}

void UInputRowWidget::InitializeAction(const FActionRebindRow& ActionRow_In)
{
	InputType = EInputType::Action;
	ActionRow = ActionRow_In;
	
	InputNameText->SetText(ActionRow.DisplayText);
	
	if (ActionRow.ActionNames.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() There is no any action names!"), *FString(__FUNCTION__))
		return;
	}

	GetInputRebindSubsystem()->GetActionKeys(ActionRow.ActionNames[0], FirstSlotKey, SecondSlotKey);
	FirstSlotKeySelector->SetSelectedKey(FirstSlotKey);
	SecondSlotKeySelector->SetSelectedKey(SecondSlotKey);
}

void UInputRowWidget::InitializeAxis(const FAxisRebindRow& AxisRow_In)
{
	InputType = EInputType::Axis;
	AxisRow = AxisRow_In;

	InputNameText->SetText(AxisRow.DisplayText);

	if (AxisRow.Axes.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() There is no any axis names!"), *FString(__FUNCTION__))
		return;
	}

	GetInputRebindSubsystem()->GetAxisKeys(AxisRow.Axes[0].AxisName, AxisRow.Axes[0].Scale, FirstSlotKey, SecondSlotKey);
	FirstSlotKeySelector->SetSelectedKey(FirstSlotKey);
	SecondSlotKeySelector->SetSelectedKey(SecondSlotKey);
}

void UInputRowWidget::SetHasConflict(bool bHasConflict_In)
{
	InputNameText->SetColorAndOpacity(bHasConflict_In ? ConflictColor : NormalColor);
	
	FirstSlotKeySelector->TextStyle.ColorAndOpacity = bHasConflict_In ? ConflictColor : NormalColor;
	FirstSlotKeySelector->SynchronizeProperties();

	SecondSlotKeySelector->TextStyle.ColorAndOpacity = bHasConflict_In ? ConflictColor : NormalColor;
	SecondSlotKeySelector->SynchronizeProperties();
}

void UInputRowWidget::OnFirstSlotKeySelected(FInputChord SelectedKey)
{
	if (SelectedKey.Key == FirstSlotKey)
	{
		return;
	}

	if (SelectedKey.Key == SecondSlotKey)
	{
		FirstSlotKeySelector->SetSelectedKey(FInputChord(FirstSlotKey));
		return;
	}
	
	RebindKey(FirstSlotKey, SelectedKey.Key);
}

void UInputRowWidget::OnSecondSlotKeySelected(FInputChord SelectedKey)
{
	if (SelectedKey.Key == SecondSlotKey)
	{
		return;
	}

	if (SelectedKey.Key == FirstSlotKey)
	{
		SecondSlotKeySelector->SetSelectedKey(FInputChord(SecondSlotKey));
		return;
	}
	
	RebindKey(SecondSlotKey, SelectedKey.Key);
}

void UInputRowWidget::RebindKey(FKey& OldKey, const FKey& NewKey)
{
	if (InputType == EInputType::Action)
	{
		GetInputRebindSubsystem()->RebindKeyInActions(ActionRow.ActionNames, OldKey, NewKey);
	}
	else
	{
		GetInputRebindSubsystem()->RebindKeyInAxes(AxisRow.Axes, OldKey, NewKey);
	}

	OldKey = NewKey;
	OnNewKeySelected.ExecuteIfBound(this, NewKey);
}

UInputRebindSubsystem* UInputRowWidget::GetInputRebindSubsystem() const
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	return  GameInstance->GetSubsystem<UInputRebindSubsystem>();
}
