// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "Subsystems/InputRebindSubsystem.h"

#include "CustomDeveloperSettings/InputRebindSettings.h"
#include "GameFramework/InputSettings.h"

void UInputRebindSubsystem::RebindKeyInActions(const TArray<FName>& InActions, const FKey& OldKey, const FKey& NewKey)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	
	for (const FName& InputName : InActions)
	{
		// If we have old key, remove it
		if (OldKey != FKey())
		{
			TArray<FInputActionKeyMapping> KeyMappings;
			InputSettings->GetActionMappingByName(InputName, KeyMappings);
			for (const FInputActionKeyMapping& KeyMapping : KeyMappings)
			{
				if (KeyMapping.Key == OldKey)
				{
					InputSettings->RemoveActionMapping(KeyMapping);
					break;
				}
			}
		}

		// Bind new key, if we have to
		if (NewKey != FKey())
		{
			FInputActionKeyMapping NewKeyMapping(InputName, NewKey);
			InputSettings->AddActionMapping(NewKeyMapping);
		}
	}

	// Save changes
	InputSettings->SaveKeyMappings();
	InputSettings->ForceRebuildKeymaps();
}

void UInputRebindSubsystem::RebindKeyInAxes(const TArray<FInputAxis>& InAxes, const FKey& OldKey, const FKey& NewKey)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();

	for (const FInputAxis& InputAxis : InAxes)
	{
		// If we have old key, remove it
		if (OldKey != FKey())
		{
			TArray<FInputAxisKeyMapping> KeyMappings;
			InputSettings->GetAxisMappingByName(InputAxis.AxisName, KeyMappings);
			for (const FInputAxisKeyMapping& KeyMapping : KeyMappings)
			{
				if (KeyMapping.Key == OldKey && KeyMapping.Scale == InputAxis.Scale)
				{
					InputSettings->RemoveAxisMapping(KeyMapping);
					break;
				}
			}
		}

		// Bind new key, if we have to
		if (NewKey != FKey())
		{
			FInputAxisKeyMapping NewKeyMapping(InputAxis.AxisName, NewKey, InputAxis.Scale);
			InputSettings->AddAxisMapping(NewKeyMapping);
		}
	}

	// Save changes
	InputSettings->SaveKeyMappings();
	InputSettings->ForceRebuildKeymaps();
}

bool UInputRebindSubsystem::GetInputDisplayNamesByKey(const FKey& Key, TArray<FText>& InputDisplayNames) const
{
	const UInputSettings* InputSettings = UInputSettings::GetInputSettings();

	// Search key in actions
	for (const FInputActionKeyMapping& KeyMapping : InputSettings->GetActionMappings())
	{
		if (KeyMapping.Key == Key)
		{
			FText InputDisplayName;
			if (GetInputDisplayNameByActionName(KeyMapping.ActionName, InputDisplayName))
			{
				InputDisplayNames.Add(InputDisplayName);
			}
		}	
	}

	// Search key in axes
	for (const FInputAxisKeyMapping& KeyMapping : InputSettings->GetAxisMappings())
	{
		if (KeyMapping.Key == Key)
		{
			FText InputDisplayName;
			if (GetInputDisplayNameByInputAxis(FInputAxis(KeyMapping.AxisName, KeyMapping.Scale), InputDisplayName))
			{
				InputDisplayNames.Add(InputDisplayName);
			}
		}	
	}

	return InputDisplayNames.Num() > 0;
}

bool UInputRebindSubsystem::GetInputDisplayNameByActionName(const FName& ActionName, FText& InputDisplayName) const
{
	const FActionRebindRow* Found = UInputRebindSettings::Get()->ActionRows.FindByPredicate([ActionName](const FActionRebindRow& BindingRow)
	{
		return BindingRow.ActionNames.Contains(ActionName);
	});

	if (!Found)
	{
		return false;
	}

	InputDisplayName = Found->DisplayText;
	return true;
}

bool UInputRebindSubsystem::GetInputDisplayNameByInputAxis(const FInputAxis& Axis, FText& InputDisplayName) const
{
	const FAxisRebindRow* Found = UInputRebindSettings::Get()->AxisRows.FindByPredicate([Axis](const FAxisRebindRow& BindingRow)
	{
		return BindingRow.Axes.Contains(Axis);
	});

	if (!Found)
	{
		return false;
	}

	InputDisplayName = Found->DisplayText;
	return true;
}

bool UInputRebindSubsystem::IsKeyHasAnyBindings(const FKey& Key) const
{
	const UInputSettings* InputSettings = UInputSettings::GetInputSettings();

	// Search key in actions
	for (const FInputActionKeyMapping& KeyMapping : InputSettings->GetActionMappings())
	{
		if (KeyMapping.Key == Key)
		{
			FText InputDisplayName;
			if (GetInputDisplayNameByActionName(KeyMapping.ActionName, InputDisplayName))
			{
				return true;
			}
		}	
	}

	// Search key in axes
	for (const FInputAxisKeyMapping& KeyMapping : InputSettings->GetAxisMappings())
	{
		if (KeyMapping.Key == Key)
		{
			FText InputDisplayName;
			if (GetInputDisplayNameByInputAxis(FInputAxis(KeyMapping.AxisName, KeyMapping.Scale), InputDisplayName))
			{
				return true;
			}
		}	
	}

	return false;
}

void UInputRebindSubsystem::GetActionKeys(const FName& InAction, FKey& FirstSlotKey, FKey& SecondSlotKey) const
{
	const UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	
	TArray<FInputActionKeyMapping> KeyMappings;
	InputSettings->GetActionMappingByName(InAction, KeyMappings);
	
	for (const FInputActionKeyMapping& KeyMapping : KeyMappings)
	{
		if (FirstSlotKey == FKey())
		{
			FirstSlotKey = KeyMapping.Key;
		}
		else if (SecondSlotKey == FKey())
		{
			SecondSlotKey = KeyMapping.Key;
			break;
		}
	}
}

void UInputRebindSubsystem::GetAxisKeys(const FName& InAxis, const float Scale, FKey& FirstSlotKey, FKey& SecondSlotKey) const
{
	const UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	
	TArray<FInputAxisKeyMapping> KeyMappings;
	InputSettings->GetAxisMappingByName(InAxis, KeyMappings);
	
	for (const FInputAxisKeyMapping& KeyMapping : KeyMappings)
	{
		if (KeyMapping.Scale == Scale)
		{
			if (FirstSlotKey == FKey())
			{
				FirstSlotKey = KeyMapping.Key;
			}
			else if (SecondSlotKey == FKey())
			{
				SecondSlotKey = KeyMapping.Key;
				break;
			}
		}
	}
}
