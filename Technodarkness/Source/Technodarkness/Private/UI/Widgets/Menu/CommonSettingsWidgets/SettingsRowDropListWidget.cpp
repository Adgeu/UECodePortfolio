// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Menu/CommonSettingsWidgets/SettingsRowDropListWidget.h"

#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"

void USettingsRowDropListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (const FText& Option : OptionsList)
	{
		DropListOptions->AddOption(Option.ToString());
	}

	DropListOptions->OnSelectionChanged.AddDynamic(this, &USettingsRowDropListWidget::OnDropListChanged);
}

void USettingsRowDropListWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	RowName->SetText(DisplayRowName);
}

void USettingsRowDropListWidget::InitializeDropListIndex(int32 Index)
{
	DropListOptions->SetSelectedIndex(Index);
}

void USettingsRowDropListWidget::OnDropListChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType != ESelectInfo::Direct)
	{
		OnRowDropListChanged.ExecuteIfBound(SelectedItem, DropListOptions->FindOptionIndex(SelectedItem));
	}
}
