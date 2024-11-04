#include "ImGuiWindowModule.h"

#include "ImGuiSettings.h"
#include "ImGuiWindowManager.h"
#include "LevelEditor.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE "FImGuiWindowModule"

void FImGuiWindowModule::StartupModule()
{
	// Registering command bindings
	{
		auto Settings = UImGuiSettings::Get();
		FImGuiWindowCommands::Register();

		ImGuiCommands = MakeShared<FUICommandList>();

		ImGuiCommands->MapAction(FImGuiWindowCommands::Get().ImGuiToggleInput, FExecuteAction::CreateRaw(this, &FImGuiWindowModule::ToggleImGuiInput));


		int i = 0;
		for (auto windowOpen : Settings->WindowToggles)
		{
			const auto windowName = windowOpen.Key;

			ImGuiCommands->MapAction(FImGuiWindowCommands::Get().ImGuiOpenWindow[i], FExecuteAction::CreateLambda([this, windowName]
			{
				OpenImGuiWindow(windowName);
			}));

			i++;
		}

		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorModule.GetGlobalLevelEditorActions()->Append(ImGuiCommands.ToSharedRef());

	}
}

void FImGuiWindowModule::ShutdownModule()
{
	FImGuiWindowCommands::Unregister();
}

void FImGuiWindowModule::ToggleImGuiInput()
{
	UImGuiWindowManager::Get()->ToggleInputMode();
}

void FImGuiWindowModule::OpenImGuiWindow(const FString& Name)
{
	UImGuiWindowManager::Get()->OpenWindow(Name);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FImGuiWindowModule, ImGuiWindow)
