// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ImGuiKeyInfo.h"
#include "UObject/Object.h"
#include "ImGuiSettings.generated.h"

/**
 *
 */
UCLASS(Config=Game, defaultconfig)
class IMGUIWINDOW_API UImGuiSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	friend class FImGuiWindowCommands;
	friend class UImGuiWindowManager;
	friend class FImGuiWindowModule;

public:
	UImGuiSettings()
	{
		CategoryName = TEXT("Plugins");
		SectionName = TEXT("ImGui");
	}
	static const UImGuiSettings* Get() { return GetDefault<UImGuiSettings>(); }

	inline static FSimpleMulticastDelegate OnImGuiSettingsChanged;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override { Super::PostEditChangeProperty(PropertyChangedEvent); OnImGuiSettingsChanged.Broadcast(); }

protected:

	// Whether ImGui should share keyboard input with game.
	// This defines initial behaviour which can be later changed using 'ImGui.ToggleKeyboardInputSharing' command or
	// module properties interface.
	UPROPERTY(EditAnywhere, config, Category = "Input")
	bool bShareKeyboardInput = false;

	// Whether ImGui should share gamepad input with game.
	// This defines initial behaviour which can be later changed using 'ImGui.ToggleGamepadInputSharing' command or
	// module properties interface.
	UPROPERTY(EditAnywhere, config, Category = "Input")
	bool bShareGamepadInput = false;

	// Whether ImGui should share mouse input with game.
	// This defines initial behaviour which can be later changed using 'ImGui.ToggleMouseInputSharing' command or
	// module properties interface.
	UPROPERTY(EditAnywhere, config, Category = "Input")
	bool bShareMouseInput = false;

	// Define a shortcut key to 'ImGui.ToggleInput' command. Binding is only set if the key field is valid.
	// Note that modifier key properties can be set to one of the three values: undetermined means that state of the given
	// modifier is not important, checked means that it needs to be pressed and unchecked means that it cannot be pressed.
	//
	// This binding is using Player Input's DebugExecBindings which only works in non-shipping builds.
	UPROPERTY(EditAnywhere, config, Category = "Keyboard Shortcuts")
	FImGuiKeyInfo ToggleInput;

	// Shortcut for opening various windows in a way thats configurable. Mapping window name -> key
	UPROPERTY(EditAnywhere, config, Category = "Keyboard Shortcuts")
	TMap<FString, FImGuiKeyInfo> WindowToggles;
};


class FImGuiWindowCommands : public TCommands<FImGuiWindowCommands>
{
public:
	FImGuiWindowCommands() : TCommands<FImGuiWindowCommands>(
		"ImGuiWindowCommands",
		NSLOCTEXT("Contexts", "ImGuiWindowCommandsName", "ImGui Window Commands"),
		NAME_None,
		FAppStyle::GetAppStyleSetName())
	{}

	// Commands
	TSharedPtr<FUICommandInfo> ImGuiToggleInput;
	TArray<TSharedPtr<FUICommandInfo>> ImGuiOpenWindow;

	virtual void RegisterCommands() override;

};
