// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomDeveloperSettings/InputRebindSettings.h"
#include "GameFramework/PlayerInput.h"
#include "InputRowWidget.generated.h"

class UTextBlock;
class UInputKeySelector;
class UCPPGameInstance;

class UInputRowWidget;

// New key was selected.
DECLARE_DELEGATE_TwoParams(FOnNewKeySelected, UInputRowWidget* /*InInputRow*/, const FKey& /*NewKey*/);

/**
 * Type of the input that was setted to the input row.
 */
UENUM()
enum class EInputType : uint8
{
	Action,
	Axis
};

/**
 * InputListWidget row.
 * @see UInputListWidget
 */
UCLASS()
class TECHNODARKNESS_API UInputRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	/** Initializing this row with the action input. */
	void InitializeAction(const FActionRebindRow& ActionRow_In);
	
	/** Initializing this row with the axis input. */
	void InitializeAxis(const FAxisRebindRow& AxisRow_In);

	/** Sets has conflict. */
	void SetHasConflict(bool bHasConflict_In);

	/** Returns true if there are any binded keys. */
	FORCEINLINE bool HasKey(const FKey& Key) const
	{
		return Key == FirstSlotKey || Key == SecondSlotKey;
	}

	/** Returns first slot cached key. */
	FORCEINLINE const FKey& GetFirstKey() const
	{
		return FirstSlotKey;
	}

	/** Returns second slot cached key. */
	FORCEINLINE const FKey& GetSecondKey() const
	{
		return SecondSlotKey;
	}

	// New key was selected.
	FOnNewKeySelected OnNewKeySelected;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InputNameText;

	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* FirstSlotKeySelector;

	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* SecondSlotKeySelector;

	// Conflict state color of the captions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateColor ConflictColor;

private:
	/** New key was selected in first slot key selector. */
	UFUNCTION()
	void OnFirstSlotKeySelected(FInputChord SelectedKey);

	/** New key was selected in second slot key selector. */
	UFUNCTION()
	void OnSecondSlotKeySelected(FInputChord SelectedKey);

	/** Rebind old key with new one. */
	void RebindKey(FKey& OldKey, const FKey& NewKey);

	/** Returns input rebinds subsystem. */
	class UInputRebindSubsystem* GetInputRebindSubsystem() const;

	// Assigned action rebind row (if we initialize row as action input).
	FActionRebindRow ActionRow;
	// Assigned axis rebind row (if we initialize row as axis input).
	FAxisRebindRow AxisRow;
	
	// Type of the input that was setted to the input row.
	EInputType InputType;

	// Cached key of the first slot.
	FKey FirstSlotKey;
	// Cached key of the second slot.
	FKey SecondSlotKey;

	// Cached normal state color of the captions.
	FSlateColor NormalColor;
};
