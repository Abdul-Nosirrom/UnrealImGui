// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ImGuiWindow.h"
#include "ImGuiWindowManager.h"
#include "ImGuiWindow_Settings.generated.h"

class UImGuiWindowConfig_Settings;

//--------------------------------------------------------------------------------------------------------------------------
class FImGuiWindow_Settings : public FImGuiWindow
{

public:

    virtual void Initialize() override;

    virtual void Tick(float DeltaTime) override;

    const UImGuiWindowConfig_Settings* GetSettingsConfig() const { return Config; }

    void SetDPIScale(float Value) const;

protected:

    virtual void RenderContent() override;

    virtual void PreSaveConfig() override;

    virtual void ResetConfig() override;

    TObjectPtr<UImGuiWindowConfig_Settings> Config = nullptr;
};


//--------------------------------------------------------------------------------------------------------------------------
UCLASS(Config = Cog)
class UImGuiWindowConfig_Settings : public UImGuiWindowConfig
{
    GENERATED_BODY()

public:

    UPROPERTY(Config)
    float DPIScale = 1.0f;

    UPROPERTY(Config)
    bool bEnableViewports = false;

    UPROPERTY(Config)
    bool bCompactMode = false;

    UPROPERTY(Config)
    bool bTransparentMode = false;

    UPROPERTY(Config)
    bool bShowHelp = true;

    UPROPERTY(Config)
    bool bShowWindowsInMainMenu = true;

    UPROPERTY(Config)
    bool bEnableInput = false;

    UPROPERTY(Config)
    bool bShareMouse = false;

    UPROPERTY(Config)
    bool bShareMouseWithGameplay = false;

    UPROPERTY(Config)
    bool bShareKeyboard = false;

    UPROPERTY(Config)
    bool bNavEnableKeyboard = false;

    //UPROPERTY(Config)
    //bool bNavEnableGamepad = false;

    //UPROPERTY(Config)
    //bool bNavNoCaptureInput = true;

    virtual void Reset() override
    {
        Super::Reset();

        DPIScale = 1.0f;
        bEnableViewports = false;
        bCompactMode = false;
        bTransparentMode = false;
        bShowHelp = true;
        bShowWindowsInMainMenu = true;
        bEnableInput = false;
        bShareMouse = false;
        bShareKeyboard = false;
        bNavEnableKeyboard = false;
        //bNavEnableGamepad = false;
        //bNavNoCaptureInput = true;
    }
};
