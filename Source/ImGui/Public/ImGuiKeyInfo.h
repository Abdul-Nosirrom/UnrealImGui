#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "GameFramework/PlayerInput.h"
#include "Styling/SlateTypes.h"

#include "ImGuiKeyInfo.generated.h"

USTRUCT()
struct IMGUI_API FImGuiKeyInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	FKey Key = EKeys::Invalid;

	UPROPERTY(EditAnywhere, Category = "Input")
    ECheckBoxState Shift = ECheckBoxState::Undetermined;

	UPROPERTY(EditAnywhere, Category = "Input")
    ECheckBoxState Ctrl = ECheckBoxState::Undetermined;

	UPROPERTY(EditAnywhere, Category = "Input")
    ECheckBoxState Alt = ECheckBoxState::Undetermined;

	UPROPERTY(EditAnywhere, Category = "Input")
    ECheckBoxState Cmd = ECheckBoxState::Undetermined;

    FImGuiKeyInfo()
    {
    }

    FImGuiKeyInfo(const FKey InKey,
                     const ECheckBoxState InShift = ECheckBoxState::Undetermined,
                     const ECheckBoxState InCtrl = ECheckBoxState::Undetermined,
                     const ECheckBoxState InAlt = ECheckBoxState::Undetermined,
                     const ECheckBoxState InCmd = ECheckBoxState::Undetermined)
        : Key(InKey)
        , Shift(InShift)
        , Ctrl(InCtrl)
        , Alt(InAlt)
        , Cmd(InCmd)
    {
    }

	friend bool operator==(const FImGuiKeyInfo& Lhs, const FImGuiKeyInfo& Rhs)
	{
		return Lhs.Key == Rhs.Key
			&& Lhs.Shift == Rhs.Shift
			&& Lhs.Ctrl == Rhs.Ctrl
			&& Lhs.Alt == Rhs.Alt
			&& Lhs.Cmd == Rhs.Cmd;
	}

	friend bool operator!=(const FImGuiKeyInfo& Lhs, const FImGuiKeyInfo& Rhs)
	{
		return !(Lhs == Rhs);
	}

	FInputChord InputChord() const
    {
    	const bool bShift = Shift == ECheckBoxState::Checked;
    	const bool bCtrl = Ctrl == ECheckBoxState::Checked;
    	const bool bAlt = Alt == ECheckBoxState::Checked;
    	const bool bCmd = Cmd == ECheckBoxState::Checked;
	    return FInputChord(Key, bShift, bCtrl, bAlt, bCmd);
    }

	FKeyBind KeyBind() const
    {
	    FKeyBind KeyBind;
    	KeyBind.Key = Key;
    	KeyBind.Shift = Shift == ECheckBoxState::Checked;
    	KeyBind.Control = Ctrl == ECheckBoxState::Checked;
    	KeyBind.Alt = Alt == ECheckBoxState::Checked;
    	KeyBind.Cmd = Cmd == ECheckBoxState::Checked;
    	KeyBind.bIgnoreShift = Shift == ECheckBoxState::Unchecked;
    	KeyBind.bIgnoreCtrl = Ctrl == ECheckBoxState::Unchecked;
    	KeyBind.bIgnoreAlt = Alt == ECheckBoxState::Unchecked;
    	KeyBind.bIgnoreCmd = Cmd == ECheckBoxState::Unchecked;

    	return KeyBind;
    }
};
