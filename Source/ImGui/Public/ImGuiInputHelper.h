#pragma once

#include "CoreMinimal.h"
#include "imgui.h"
#include "GenericPlatform/ICursor.h"

class APlayerController;
class UPlayerInput;
class UWorld;
enum class ECheckBoxState : uint8;
struct FImGuiKeyInfo;
struct FImGuiKeyInfo;
struct FKey;
struct FKeyBind;
struct FKeyEvent;

class IMGUI_API FImGuiInputHelper
{
public:

    static APlayerController* GetFirstLocalPlayerController(const UWorld& World);

    static UPlayerInput* GetPlayerInput(const UWorld& World);

    static bool IsKeyEventHandled(UWorld* World, const FKeyEvent& KeyEvent);

    static bool WasKeyInfoJustPressed(APlayerController& PlayerController, const FImGuiKeyInfo& KeyInfo);

    static bool IsCheckBoxStateMatchingValue(ECheckBoxState CheckBoxState, bool bValue);

    static bool IsKeyEventMatchingKeyInfo(const FKeyEvent& KeyEvent, const FImGuiKeyInfo& InputChord);

    static bool IsKeyEventMatchingKeyBind(const FKeyEvent& KeyEvent, const FKeyBind& KeyBind);

    static ECheckBoxState MakeCheckBoxState(uint8 RequireValue, uint8 IgnoreValue);

    static void KeyBindToKeyInfo(const FKeyBind& KeyBind, FImGuiKeyInfo& KeyInfo);

    static void KeyInfoToKeyBind(const FImGuiKeyInfo& KeyInfo, FKeyBind& KeyBind);

    static bool IsConsoleEvent(const FKeyEvent& KeyEvent);

    static bool IsKeyBoundToCommand(UWorld* World, const FKeyEvent& KeyEvent);

    static bool IsStopPlaySessionEvent(const FKeyEvent& KeyEvent);

    static uint32 ToImGuiMouseButton(const FKey& MouseButton);

    static ImGuiKey ToImKey(const FKey& Key);

    static EMouseCursor::Type ToSlateMouseCursor(ImGuiMouseCursor MouseCursor);

    static FString CommandToString(const UWorld& World, const FString& Command);

    static FString CommandToString(const UPlayerInput* PlayerInput, const FString& Command);

    static FString KeyBindToString(const FKeyBind& KeyBind);

    static bool IsMouseInsideMainViewport();

    static bool IsKeyBoundToCommand(const UPlayerInput* InPlayerInput, const FKeyEvent& KeyEvent);

    template<typename T, std::enable_if_t<(sizeof(T) <= sizeof(ImWchar)), T>* = nullptr>
    static ImWchar CastInputChar(T Char)
    {
        return static_cast<ImWchar>(Char);
    }
};
