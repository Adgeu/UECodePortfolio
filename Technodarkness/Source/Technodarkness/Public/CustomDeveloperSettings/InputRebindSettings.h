// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "Engine/DeveloperSettings.h"
#include "InputRebindSettings.generated.h"

/**
 * Helper row that contains input display text that should showing in the widget,
 * action names that will be rebinded on key change in the widget.
 */
USTRUCT(BlueprintType)
struct FActionRebindRow
{
	GENERATED_BODY()

	// Text that displays in the input settings list.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayText;

	// Action names that will be rebinded on key change in the widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> ActionNames;
};

/**
 * Input axis name and its scale.
 */
USTRUCT(BlueprintType)
struct FInputAxis
{
	GENERATED_BODY()

	// Name of the axis.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AxisName = NAME_None;

	// Scale of the axis.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Scale = 0.0f;

	FInputAxis() = default;
	FInputAxis(const FName& AxisName_In, float Scale_In) : AxisName(AxisName_In), Scale(Scale_In)
	{
	}

	bool operator ==(const FInputAxis& Other) const
	{
		return AxisName == Other.AxisName && Scale == Other.Scale;
	}

	bool operator !=(const FInputAxis& Other) const
	{
		return AxisName != Other.AxisName || Scale != Other.Scale;
	}
};

/**
 * Helper row that contains input display text that should showing in the widget,
 * axis names that will be rebinded on key change in the widget.
 */
USTRUCT(BlueprintType)
struct FAxisRebindRow
{
	GENERATED_BODY()

	// Text that displays in the input settings list.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayText;

	// Axes that will be rebinded on key change in the widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInputAxis> Axes;
};

/**
 * Settings that regulate the input rebind settings.
 */
UCLASS(Config=Game, DefaultConfig)
class TECHNODARKNESS_API UInputRebindSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UInputRebindSettings* Get();

	// The list of the actions for which we want to give the opportunity to change the binding.
	UPROPERTY(EditAnywhere, Config, Category = "Input Binding: Actions", meta = (TitleProperty = "DisplayText"))
	TArray<FActionRebindRow> ActionRows;

	// The list of the axes for which we want to give the opportunity to change the binding.
	UPROPERTY(EditAnywhere, Config, Category = "Input Binding: Axises", meta = (TitleProperty = "DisplayText"))
	TArray<FAxisRebindRow> AxisRows;
};
