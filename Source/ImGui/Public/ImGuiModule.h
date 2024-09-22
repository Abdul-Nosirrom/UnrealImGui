#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IMGUI_API FImGuiModule : public IModuleInterface
{
public:

    static inline FImGuiModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FImGuiModule>("ImGui");
    }

    //----------------------------------------------------------------------------------------------------------------------
    // IModuleInterface implementation
    //----------------------------------------------------------------------------------------------------------------------
    virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

};
