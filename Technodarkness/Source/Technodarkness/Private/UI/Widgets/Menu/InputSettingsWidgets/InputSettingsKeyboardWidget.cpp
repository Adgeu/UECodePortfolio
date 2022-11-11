// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "UI/Widgets/Menu/InputSettingsWidgets/InputSettingsKeyboardWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/InputRebindSubsystem.h"
#include "UI/Widgets/Menu/InputSettingsWidgets/KeyboardKeyWidget.h"

void UInputSettingsKeyboardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyButtons = GetKeyboardKeyWidgets();

	for (UKeyboardKeyWidget* KeyButton : KeyButtons)
	{
		KeyButton->OnSelected.BindUObject(this, &UInputSettingsKeyboardWidget::OnKeyButtonClicked);
	}
}

void UInputSettingsKeyboardWidget::UpdateKeyboardKeyButtons()
{
	if (SelectedKeyboardKey)
	{
		SelectedKeyboardKey->Deselect();
	}
	
	const UInputRebindSubsystem* InputRebindSubsystem = GetInputRebindSubsystem();
	for (UKeyboardKeyWidget* KeyButton : KeyButtons)
	{
		KeyButton->SetHasConflict(CheckConflicts(InputRebindSubsystem, KeyButton->GetKey()));
		KeyButton->SetHasBindedInput(InputRebindSubsystem->IsKeyHasAnyBindings(KeyButton->GetKey()));
	}
}

void UInputSettingsKeyboardWidget::OnKeyButtonClicked(UKeyboardKeyWidget* Sender)
{
	if (SelectedKeyboardKey)
	{
		SelectedKeyboardKey->Deselect();
	}

	InputNamesScrollBox->ClearChildren();
	
	SelectedKeyboardKey = Sender;
	if (SelectedKeyboardKey)
	{
		TArray<FText> InputDisplayNames;
		if (GetInputRebindSubsystem()->GetInputDisplayNamesByKey(SelectedKeyboardKey->GetKey(), InputDisplayNames))
		{
			for (const FText& InputDisplayName : InputDisplayNames)
			{
				UTextBlock* TextBlock =  WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
				TextBlock->SetText(InputDisplayName);

				InputNamesScrollBox->AddChild(TextBlock);
			}
		}
	}
}

TArray<UKeyboardKeyWidget*> UInputSettingsKeyboardWidget::GetKeyboardKeyWidgets() const
{
	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);

	TArray<UKeyboardKeyWidget*> Result;
	Result.Reserve(110);

	for (UWidget* Widget : Widgets)
	{
		if (UKeyboardKeyWidget* KeyboardKeyWidget = Cast<UKeyboardKeyWidget>(Widget))
		{
			Result.Emplace(KeyboardKeyWidget);
		}
	}

	return Result;
}

bool UInputSettingsKeyboardWidget::CheckConflicts(const UInputRebindSubsystem* InInputRebindSubsystem, const FKey& Key) const
{
	TArray<FText> InputDisplayNames;
	InInputRebindSubsystem->GetInputDisplayNamesByKey(Key, InputDisplayNames);
	return InputDisplayNames.Num() > 1;
}

UInputRebindSubsystem* UInputSettingsKeyboardWidget::GetInputRebindSubsystem() const
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	return GameInstance->GetSubsystem<UInputRebindSubsystem>();
}
