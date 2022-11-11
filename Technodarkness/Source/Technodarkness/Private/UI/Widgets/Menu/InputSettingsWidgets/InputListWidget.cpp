// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "UI/Widgets/Menu/InputSettingsWidgets/InputListWidget.h"

#include "Components/ScrollBox.h"
#include "CustomDeveloperSettings/InputRebindSettings.h"
#include "GameFramework/InputSettings.h"
#include "UI/Widgets/Menu/InputSettingsWidgets/InputRowWidget.h"

void UInputListWidget::RefreshList()
{
	InputList->ClearChildren();
	
	if (!InputBindingRowWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() InputBindingRowWidgetClass is null!"), *FString(__FUNCTION__))
		return;
	}

	AddActionsToList();
	AddAxesToList();
	
	CheckAllConflicts();
}

void UInputListWidget::OnNewKeySelected(UInputRowWidget* InInputRow, const FKey& NewKey)
{
	CheckAllConflicts();
}

void UInputListWidget::AddActionsToList()
{
	TArray<FActionRebindRow> ActionRows = UInputRebindSettings::Get()->ActionRows;
	for (const FActionRebindRow& ActionRow : ActionRows)
	{
		UInputRowWidget* InputRow = CreateWidget<UInputRowWidget>(this, InputBindingRowWidgetClass);
		InputRow->InitializeAction(ActionRow);
		InputRow->OnNewKeySelected.BindUObject(this, &UInputListWidget::OnNewKeySelected);

		InputList->AddChild(InputRow);
	}
}

void UInputListWidget::AddAxesToList()
{
	TArray<FAxisRebindRow> AxisRows = UInputRebindSettings::Get()->AxisRows;
	for (const FAxisRebindRow& AxisRow : AxisRows)
	{
		UInputRowWidget* InputRow = CreateWidget<UInputRowWidget>(this, InputBindingRowWidgetClass);
		InputRow->InitializeAxis(AxisRow);
		InputRow->OnNewKeySelected.BindUObject(this, &UInputListWidget::OnNewKeySelected);

		InputList->AddChild(InputRow);
	}
}

void UInputListWidget::CheckAllConflicts()
{
	check(InputListHasOnlyInputRowWidgets());
	
	TArray<UWidget*> Children = InputList->GetAllChildren();

	for (UWidget* Child : Children)
	{
		if (UInputRowWidget* InputRow = StaticCast<UInputRowWidget*>(Child))
		{
			InputRow->SetHasConflict(false);
		}
	}
	
	for (UWidget* Child : Children)
	{
		if (UInputRowWidget* InputRow = StaticCast<UInputRowWidget*>(Child))
		{
			if (CheckConflict(InputRow->GetFirstKey(), InputRow))
			{
				InputRow->SetHasConflict(true);
			}
			if (CheckConflict(InputRow->GetSecondKey(), InputRow))
			{
				InputRow->SetHasConflict(true);
			}
		}
	}
}

bool UInputListWidget::CheckConflict(const FKey& Key, const UInputRowWidget* IgnoreRow) const
{
	bool bResult = false;
	
	if (Key == FKey())
	{
		return bResult;
	}
	
	TArray<UWidget*> Children = InputList->GetAllChildren();
	for (UWidget* Child : Children)
	{
		if (UInputRowWidget* InputRow = Cast<UInputRowWidget>(Child))
		{
			if (!IgnoreRow || InputRow != IgnoreRow)
			{
				if (InputRow->HasKey(Key))
				{
					InputRow->SetHasConflict(true);
					bResult |= true;
				}
			}
		}
	}

	return bResult;
}

bool UInputListWidget::InputListHasOnlyInputRowWidgets() const
{
	TArray<UWidget*> Children = InputList->GetAllChildren();
	for (const UWidget* Child : Children)
	{
		if (!Child->IsA<UInputRowWidget>())
		{
			return false;
		}
	}
	return true;
}
