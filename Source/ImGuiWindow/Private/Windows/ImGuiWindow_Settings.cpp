// Copyright 2023 CoC All rights reserved


#include "ImGuiWindow_Settings.h"

#include "imgui.h"
#include "imgui.h"
#include "ImGuiHelper.h"
#include "ImGuiInputHelper.h"
#include "ImGuiWidgets.h"

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiWindow_Settings::Initialize()
{
    bHasMenu = false;

    Config = GetConfig<UImGuiWindowConfig_Settings>();

    ImGuiIO& IO = ImGui::GetIO();
    FImGuiHelper::SetFlags(IO.ConfigFlags, ImGuiConfigFlags_ViewportsEnable, Config->bEnableViewports);
    FImGuiHelper::SetFlags(IO.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard, Config->bNavEnableKeyboard);
    //FCogImguiHelper::SetFlags(IO.ConfigFlags, ImGuiConfigFlags_NavEnableGamepad, Config->bNavEnableGamepad);
    //FCogImguiHelper::SetFlags(IO.ConfigFlags, ImGuiConfigFlags_NavNoCaptureKeyboard, Config->bNavNoCaptureInput);

    GetOwner()->GetContext().SetDPIScale(Config->DPIScale);

    FImGuiContext& Context = GetOwner()->GetContext();
    Context.SetEnableInput(Config->bEnableInput);
    Context.SetShareKeyboard(Config->bShareKeyboard);
    Context.SetShareMouse(Config->bShareMouse);
    Context.SetShareMouseWithGameplay(Config->bShareMouseWithGameplay);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiWindow_Settings::PreSaveConfig()
{
    ImGuiIO& IO = ImGui::GetIO();
    Config->bNavEnableKeyboard = IO.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard;
    //Config->bNavEnableGamepad = IO.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad;
    //Config->bNavNoCaptureInput = IO.ConfigFlags & ImGuiConfigFlags_NavNoCaptureKeyboard;

    const FImGuiContext& Context = GetOwner()->GetContext();
    Config->bEnableInput = Context.GetEnableInput();
    Config->bShareKeyboard = Context.GetShareKeyboard();
    Config->bShareMouse = Context.GetShareMouse();
    Config->bShareMouseWithGameplay = Context.GetShareMouseWithGameplay();
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiWindow_Settings::ResetConfig()
{
    Config->Reset();
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiWindow_Settings::RenderContent()
{
    const UPlayerInput* PlayerInput = FImGuiInputHelper::GetPlayerInput(*GetWorld());
    if (PlayerInput == nullptr)
    {
        return;
    }

    ImGuiIO& IO = ImGui::GetIO();
    FImGuiContext& Context = GetOwner()->GetContext();

    //-------------------------------------------------------------------------------------------
    if (ImGui::CollapsingHeader("Input", ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool bEnableInput = Context.GetEnableInput();
        if (ImGui::Checkbox("Enable Input", &bEnableInput))
        {
            Context.SetEnableInput(bEnableInput);
        }
        ImGui::SetItemTooltip("Enable ImGui inputs. When enabled the ImGui menu is shown and inputs are forwarded to ImGui.");

        const auto ShortcutText = StringCast<ANSICHAR>(*FImGuiInputHelper::CommandToString(PlayerInput, UImGuiWindowManager::ToggleInputCommand));
        const float ShortcutWidth = (ShortcutText.Get() != nullptr && ShortcutText.Get()[0]) ? ImGui::CalcTextSize(ShortcutText.Get(), NULL).x : 0.0f;
        if (ShortcutWidth > 0.0f)
        {
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX() - ShortcutWidth);
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            ImGui::Text("%s", ShortcutText.Get());
            ImGui::PopStyleColor();
        }

        //-------------------------------------------------------------------------------------------
        bool bShareKeyboard = Context.GetShareKeyboard();
        if (ImGui::Checkbox("Share Keyboard", &bShareKeyboard))
        {
            Context.SetShareKeyboard(bShareKeyboard);
        }
        ImGui::SetItemTooltip("Forward the keyboard inputs to the game when ImGui does not need them.");

        //-------------------------------------------------------------------------------------------
        bool bShareMouse = Context.GetShareMouse();
        if (ImGui::Checkbox("Share Mouse", &bShareMouse))
        {
            Context.SetShareMouse(bShareMouse);
        }
        ImGui::SetItemTooltip("Forward mouse inputs to the game when ImGui does not need them.");

        //-------------------------------------------------------------------------------------------
        if (bShareMouse == false)
        {
            ImGui::BeginDisabled();
        }

        bool bShareMouseWithGameplay = Context.GetShareMouseWithGameplay();
        if (ImGui::Checkbox("Share Mouse With Gameplay", &bShareMouseWithGameplay))
        {
            Context.SetShareMouseWithGameplay(bShareMouseWithGameplay);
        }
        ImGui::SetItemTooltip("When disabled, mouse inputs are only forwarded to game menus. "
        "When enabled, mouse inputs are also forwarded to the gameplay. Note that this mode: \n"
        "  - Force the cursor to be visible.\n"
        "  - Prevent the interaction of Cog's transform gizmos.\n"
        );

        if (bShareMouse == false)
        {
            ImGui::EndDisabled();
        }

        //-------------------------------------------------------------------------------------------
        ImGui::CheckboxFlags("Keyboard Navigation", &IO.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
        ImGui::SetItemTooltip("Use the keyboard to navigate in ImGui windows with the following keys : Tab, Directional Arrows, Space, Enter.");
    }

    //-------------------------------------------------------------------------------------------
    //ImGui::CheckboxFlags("Gamepad Navigation", &IO.ConfigFlags, ImGuiConfigFlags_NavEnableGamepad);
    //ImGui::SetItemTooltip("Use the gamepad to navigate in ImGui windows.");

    //-------------------------------------------------------------------------------------------
    if (ImGui::CollapsingHeader("Window", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Checkbox("Enable Viewports", &Config->bEnableViewports))
        {
            FImGuiHelper::SetFlags(IO.ConfigFlags, ImGuiConfigFlags_ViewportsEnable, Config->bEnableViewports);
        }
        ImGui::SetItemTooltip("Enable moving ImGui windows outside of the main viewport.");

        //-------------------------------------------------------------------------------------------
        ImGui::Checkbox("Compact Mode", &Config->bCompactMode);
        ImGui::SetItemTooltip("Enable compact mode.");

        //-------------------------------------------------------------------------------------------
        ImGui::Checkbox("Show Windows In Main Menu", &Config->bShowWindowsInMainMenu);
        ImGui::SetItemTooltip("Show the content of the windows when hovering the window menu item.");

        //-------------------------------------------------------------------------------------------
        ImGui::Checkbox("Show Help", &Config->bShowHelp);
        ImGui::SetItemTooltip("Show windows help on the window menu items.");

        //-------------------------------------------------------------------------------------------
        UImGuiHelperWidgets::SetNextItemToShortWidth();
        ImGui::SliderFloat("DPI Scale", &Config->DPIScale, 0.5f, 2.0f, "%.1f");
        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            SetDPIScale(Config->DPIScale);
        }
        if (ImGui::BeginItemTooltip())
        {
            ImGui::TextUnformatted("Change DPi Scale [Mouse Wheel]");
            ImGui::TextUnformatted("Reset DPi Scale  [Middle Mouse]");
            ImGui::EndTooltip();
        }
    }

    //-------------------------------------------------------------------------------------------
    if (ImGui::CollapsingHeader("Config"))
    {
        if (ImGui::Button("Reset All Windows Config", ImVec2(-1.0f, 0.0f)))
        {
            GetOwner()->ResetAllWindowsConfig();
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiWindow_Settings::Tick(float DeltaTime)
{
    const float MouseWheel = ImGui::GetIO().MouseWheel;
    const bool IsControlDown = ImGui::GetIO().KeyCtrl;
    if (IsControlDown && MouseWheel != 0)
    {
        SetDPIScale(FMath::Clamp(Config->DPIScale + (MouseWheel > 0 ? 0.1f : -0.1f), 0.5f, 2.0f));
    }

    const bool IsMiddleMouseClicked = ImGui::GetIO().MouseClicked[2];
    if (IsControlDown && IsMiddleMouseClicked)
    {
        SetDPIScale(1.0f);
    }
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiWindow_Settings::SetDPIScale(float Value) const
{
    Config->DPIScale = Value;
    GetOwner()->GetContext().SetDPIScale(Config->DPIScale);
}
