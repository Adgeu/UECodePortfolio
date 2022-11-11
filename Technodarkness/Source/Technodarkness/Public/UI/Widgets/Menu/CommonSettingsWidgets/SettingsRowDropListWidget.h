// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsRowDropListWidget.generated.h"

class UTextBlock;
class UComboBoxString;

DECLARE_DELEGATE_TwoParams(FOnRowDropListChanged, const FString& /*SelectedItem*/, int32 /*SelectedItemIndex*/);

/**
 * WIP
 */
UCLASS()
class TECHNODARKNESS_API USettingsRowDropListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void InitializeDropListIndex(int32 Index);
	
	FOnRowDropListChanged OnRowDropListChanged;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RowName;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* DropListOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Row Name")
	FText DisplayRowName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Options")
	TArray<FText> OptionsList;

private:
	UFUNCTION()
	void OnDropListChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
