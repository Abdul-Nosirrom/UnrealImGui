#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FImGuiWindowModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

protected:
	TSharedPtr<FUICommandList> ImGuiCommands;

	void ToggleImGuiInput();
	void OpenImGuiWindow(const FString& Name);
};
