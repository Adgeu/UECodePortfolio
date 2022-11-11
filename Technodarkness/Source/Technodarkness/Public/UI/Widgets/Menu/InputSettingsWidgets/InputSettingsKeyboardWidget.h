// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputSettingsKeyboardWidget.generated.h"

class UKeyboardKeyWidget;
class UScrollBox;
class UInputSettingsAsset;
class UInputRebindSubsystem;

/**
 * Virtual keyboard widget.
 */
UCLASS()
class TECHNODARKNESS_API UInputSettingsKeyboardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/** Updating all keyboard key buttons state. */
	void UpdateKeyboardKeyButtons();

protected:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* InputNamesScrollBox;
	
private:
	/** One of the keyboard key button was clicked. */
	void OnKeyButtonClicked(UKeyboardKeyWidget* Sender);

	/** Returns array of all keyboard key buttons. */
	TArray<UKeyboardKeyWidget*> GetKeyboardKeyWidgets() const;

	/** Checks conflicts with the key. */
	bool CheckConflicts(const UInputRebindSubsystem* InInputRebindSubsystem, const FKey& Key) const;

	/** Returns input rebinds subsystem. */
	UInputRebindSubsystem* GetInputRebindSubsystem() const;
	
	// Array of all keyboard key buttons.
	TArray<UKeyboardKeyWidget*> KeyButtons;

	// Current selected keyboard key button.
	UPROPERTY()
	UKeyboardKeyWidget* SelectedKeyboardKey = nullptr;

#pragma region Keyboard Keys

protected:
	
	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Escape;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F1;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F2;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F3;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F4;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F5;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F6;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F7;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F8;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F9;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F10;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F11;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F12;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_PrintScreen;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Insert;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Delete;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_ScrollLock;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Home;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_End;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_PauseBreak;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_PageUp;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_PageDown;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Tilde;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_1;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_2;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_3;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_4;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_5;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_6;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_7;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_8;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_9;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_0;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Hyphen;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Equal;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Backspace;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Tab;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_CapsLock;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_LeftShift;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_RightShift;
	
	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_LeftCtrl;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_RightCtrl;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_LeftCmd;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_RightCmd;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_LeftAlt;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_RightAlt;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Space;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Enter;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_NumLock;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Divide;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Multiply;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Subtract;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Add;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_NumEnter;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Decimal;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Num0;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Num1;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Num2;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Num3;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Num4;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Num5;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Num6;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Num7;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Num8;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Num9;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Q;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_W;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_E;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_R;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_T;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Y;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_U;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_I;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_O;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_P;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_LeftBracket;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_RightBracket;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_A;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_S;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_D;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_F;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_G;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_H;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_J;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_K;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_L;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Semicolon;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Apostrophe;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Backslash;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Z;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_X;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_C;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_V;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_B;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_N;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_M;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Comma;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Period;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Slash;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Up;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Down;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Left;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_Right;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_LMB;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_RMB;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_MMB;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_SMB_1;

	UPROPERTY(meta = (BindWidget))
	UKeyboardKeyWidget* ButtonKey_SMB_2;

#pragma endregion ~Keyboard Keys
};
