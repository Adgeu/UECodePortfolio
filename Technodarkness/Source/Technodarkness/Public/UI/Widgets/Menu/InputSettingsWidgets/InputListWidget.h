// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputListWidget.generated.h"

struct FAxisBindingRow;
struct FActionRebindRow;
class UScrollBox;
class UInputSettingsAsset;

/**
 * Widget with the list of inputs for which we want to give the opportunity to change the binding.
 */
UCLASS()
class TECHNODARKNESS_API UInputListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Refresh list of the input row widgets. */
	void RefreshList();
	
protected:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* InputList;

	// Class of the input row widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Settings")
	TSubclassOf<class UInputRowWidget> InputBindingRowWidgetClass;

private:
	/** New key was selected in one of the input rows. */
	void OnNewKeySelected(UInputRowWidget* InInputRow, const FKey& NewKey);

	/**
	 * Add all actions from InputRebindSettings to input list.
	 * @see UInputRebindSettings
	 */
	void AddActionsToList();

	/**
	 * Add all axes from InputRebindSettings to input list.
	 * @see UInputRebindSettings
	 */
	void AddAxesToList();

	/** Checks all conflicts in the input list. */
	void CheckAllConflicts();

	/** Checks conflicts with the key. */
	bool CheckConflict(const FKey& Key, const UInputRowWidget* IgnoreRow = nullptr) const;

	/** Checks all input list children is an InputRowWidget. */
	bool InputListHasOnlyInputRowWidgets() const;
};
