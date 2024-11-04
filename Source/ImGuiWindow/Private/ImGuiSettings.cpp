// Copyright 2023 CoC All rights reserved


#include "ImGuiSettings.h"

#define LOCTEXT_NAMESPACE "FImGuiWindowModule"

void FImGuiWindowCommands::RegisterCommands()
{
	const auto KeyBinds = UImGuiSettings::Get();
	UI_COMMAND(ImGuiToggleInput, "ImGui Toggle Input", "Enables input focus for ImGui windows", EUserInterfaceActionType::None, FInputChord());

	for (auto window : KeyBinds->WindowToggles)
	{
		UI_COMMAND(ImGuiOpenWindow.Add_GetRef(nullptr), "ImGui Open Window", "Opens specific ImGui window based on binds", EUserInterfaceActionType::None, KeyBinds->WindowToggles[window.Key].InputChord());
	}
}

#undef LOCTEXT_NAMESPACE
