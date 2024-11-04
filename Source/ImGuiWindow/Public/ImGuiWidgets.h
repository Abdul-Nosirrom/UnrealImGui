// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuiWidgets.generated.h"

struct FImGuiKeyInfo;
struct FKeyBind;

using FImGuiWindowActorContextMenuFunction = TFunction<void(AActor* Actor)>;

UCLASS(meta=(ScriptName="ImGuiWidgets", DisplayName="ImGuiWidgets"))
class IMGUIWINDOW_API UImGuiHelperWidgets : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void BeginTableTooltip();

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void EndTableTooltip();

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void ThinSeparatorText(const FString& Label);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void ProgressBarCentered(float Fraction, const FVector2D& Size, FString Overlay);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool ToggleMenuButton(bool& Value, const FString& Text, const FVector4& TrueColor);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool ToggleButton(bool& Value, const FString& Text, const FVector4& TrueColor, const FVector4& FalseColor, const FVector2D& Size = FVector2D(0, 0));

	//UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool ToggleButton(bool& Value, const FString& TextTrue, const FString& TextFalse, const FVector4& TrueColor, const FVector4& FalseColor, const FVector2D& Size = FVector2D(0, 0));

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool MultiChoiceButton(const FString& Label, bool IsSelected, const FVector2D& Size = FVector2D(0, 0));

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool MultiChoiceButtonsInt(TArray<int32>& Values, int32& Value, const FVector2D& Size = FVector2D(0, 0));

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool MultiChoiceButtonsFloat(TArray<float>& Values, float& Value, const FVector2D& Size = FVector2D(0, 0));

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void SliderWithReset(const FString& Name, float& Value, float Min, float Max, const float& ResetValue, const FString& Format);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void HelpMarker(const FString& Text);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void PushStyleCompact();

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void PopStyleCompact();

	//UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void AddTextWithShadow(ImDrawList* DrawList, const FVector2D& Position, ImU32 Color, const FString& TextBegin, const FString& TextEnd = "");

	//UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void SearchBar(ImGuiTextFilter& Filter, float Width = -1.0f);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void PushBackColor(const FVector4& Color);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void PopBackColor();

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static float GetShortWidth();

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void SetNextItemToShortWidth();

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static float GetFontWidth();

    template<typename EnumType>
    static bool ComboboxEnum(const FString& Label, const EnumType CurrentValue, EnumType& NewValue);

    template<typename EnumType>
    static bool ComboboxEnum(const FString& Label, EnumType& Value);

    static bool ComboboxEnum(const FString& Label, UEnum* Enum, int64 CurrentValue, int64& NewValue);

    static bool ComboboxEnum(const FString& Label, UObject* Object, const FString& FieldName, uint8* PointerToEnumValue);

    static bool ComboboxEnum(const FString& Label, const FEnumProperty* EnumProperty, uint8* PointerToEnumValue);

    static bool CheckBoxState(const FString& Label, ECheckBoxState& State);

    static bool InputKey(const FString& Label, FImGuiKeyInfo& KeyInfo);

    static bool InputKey(FImGuiKeyInfo& KeyInfo);

    static bool KeyBind(FKeyBind& KeyBind);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool ButtonWithTooltip(const FString& Text, const FString& Tooltip);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool DeleteArrayItemButton();

	//UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool ComboCollisionChannel(const FString& Label, ECollisionChannel& Channel);

	//UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool CollisionProfileChannel(const UCollisionProfile& CollisionProfile, int32 ChannelIndex, FColor& ChannelColor, int32& Channels);

	//UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static bool CollisionProfileChannels(int32& Channels);

    static bool MenuActorsCombo(const FString& StrID, const UWorld* World, TSubclassOf<AActor> ActorClass, const FImGuiWindowActorContextMenuFunction& ContextMenuFunction = nullptr);

	static bool MenuActorsCombo(const FString& StrID, const UWorld* World, const TArray<TSubclassOf<AActor>>& ActorClasses, int32& SelectedActorClassIndex, ImGuiTextFilter* Filter, const APawn* LocalPlayerPawn, const FImGuiWindowActorContextMenuFunction& ContextMenuFunction = nullptr);

    static bool ActorsListWithFilters(const UWorld* World, const TArray<TSubclassOf<AActor>>& ActorClasses, int32& SelectedActorClassIndex, ImGuiTextFilter* Filter, const APawn* LocalPlayerPawn, const FImGuiWindowActorContextMenuFunction& ContextMenuFunction = nullptr);

    static bool ActorsList(const UWorld* World, const TSubclassOf<AActor> ActorClass, const ImGuiTextFilter* Filter = nullptr, const APawn* LocalPlayerPawn = nullptr, const FImGuiWindowActorContextMenuFunction& ContextMenuFunction = nullptr);

    static void ActorContextMenu(AActor* Selection, const FImGuiWindowActorContextMenuFunction& ContextMenuFunction);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void ActorFrame(const AActor* Actor);

	UFUNCTION(ScriptCallable, meta = (DevelopmentOnly))
    static void SmallButton(const FString& Text, const FVector4& Color);
};

template<typename EnumType>
bool UImGuiHelperWidgets::ComboboxEnum(const FString& Label, const EnumType CurrentValue, EnumType& NewValue)
{
    int64 NewValueInt;
    if (ComboboxEnum(Label, StaticEnum<EnumType>(), (int64)CurrentValue, NewValueInt))
    {
        NewValue = (EnumType)NewValueInt;
        return true;
    }

    return false;
}

template<typename EnumType>
bool UImGuiHelperWidgets::ComboboxEnum(const FString& Label, EnumType& Value)
{
    return ComboboxEnum(Label, Value, Value);
}
