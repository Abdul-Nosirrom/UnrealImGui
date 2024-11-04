// Copyright 2023 CoC All rights reserved

#include "ImGuiWindowManager.h"

#include "imgui.h"
#include "ImGuiDebugDraw.h"
#include "ImGuiHelper.h"
#include "ImGuiInputHelper.h"
#include "ImGuiSettings.h"
#include "ImGuiWidgets.h"
#include "ImGuiWindow.h"
#include "ImGuiWindowHelper.h"
#include "imgui_internal.h"
#include "GameFramework/PlayerInput.h"
#include "Interfaces/IMainFrameModule.h"
#include "Kismet/GameplayStatics.h"
#include "ImGuiWindowHelper.h"
#include "Windows/ImGuiWindow_Settings.h"

#define FONT_WIDTH UImGuiHelperWidgets::GetFontWidth()

FString UImGuiWindowManager::ToggleInputCommand = "ImGui.ToggleInput";
FString UImGuiWindowManager::OpenWindowCommand = "ImGui.Window";

/*--------------------------------------------------------------------------------------------------------------
* Initialization & Shutdown
*--------------------------------------------------------------------------------------------------------------*/

void UImGuiWindowManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
			*ToggleInputCommand,
			TEXT("Toggle the input focus between the Game and ImGui"),
			FConsoleCommandWithArgsDelegate::CreateLambda([this](const TArray<FString>& Args) { ToggleInputMode(); }),
			ECVF_Cheat));

	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
			*OpenWindowCommand,
			TEXT("Open a specific window through the console ImGui.Window [Name]"),
			FConsoleCommandWithArgsDelegate::CreateLambda([this](const TArray<FString>& Args) { if (Args.Num()==1) OpenWindow(Args[0]); }),
			ECVF_Cheat));


	// Bind initialization (when game viewport is created)
	UGameViewportClient::OnViewportCreated().AddUObject(this, &UImGuiWindowManager::InitializeGameImGui);

	// Add some default settings
	SettingsWindow = AddWindow<FImGuiWindow_Settings>("Window.Settings", false);
}

void UImGuiWindowManager::Deinitialize()
{
	Super::Deinitialize();

	for (FImGuiWindow* Window : Windows)
	{
		delete Window;
	}
	Windows.Empty();

	for (IConsoleObject* ConsoleCommand : ConsoleCommands)
	{
		IConsoleManager::Get().UnregisterConsoleObject(ConsoleCommand);
	}
}

void UImGuiWindowManager::InitializeGameImGui()//UWorld* InWorld, const UWorld::InitializationValues IVS)
{
	Context.Initialize();
	InitializedWorld = GEngine->GameViewport->GetWorld();
	bInputInitialized = false;
	bIsInitialized = true;

	const UImGuiSettings* Settings = UImGuiSettings::Get();
	Context.SetShareKeyboard(Settings->bShareKeyboardInput);
	Context.SetShareMouse(Settings->bShareMouseInput);

	bInputInitialized = RegisterDefaultCommandBindings();

	// Bind to viewport render loop (we choose pre/post actor tick so that game systems can write raw ImGui code without a window and it still fits to the render loop)
	FWorldDelegates::OnWorldPreActorTick.AddUObject(this, &UImGuiWindowManager::BeginFrame);
	FWorldDelegates::OnWorldPostActorTick.AddUObject(this, &UImGuiWindowManager::EndFrame);

	// End when viewport is closed
	GEngine->GameViewport->OnCloseRequested().AddUObject(this, &UImGuiWindowManager::ShutdownGameImGui);

	// Initialization callback for all windows
	for (auto* window : Windows) window->Initialize();
}

void UImGuiWindowManager::ShutdownGameImGui(FViewport* InViewport)
{
	// Shutdown callback for all windows
	for (auto* window : Windows) window->Shutdown();

	FWorldDelegates::OnWorldPreActorTick.RemoveAll(this);
	FWorldDelegates::OnWorldPostActorTick.RemoveAll(this);
	InitializedWorld = nullptr;
	Context.Shutdown();
}

/*--------------------------------------------------------------------------------------------------------------
* Primary Render Functions
*--------------------------------------------------------------------------------------------------------------*/

void UImGuiWindowManager::BeginFrame(UWorld* InWorld, ELevelTick InLevelTick, float InDeltaSeconds)
{
	if (!bInputInitialized) bInputInitialized = RegisterDefaultCommandBindings();

	Context.BeginFrame();

	for (FImGuiWindow* Window : Windows)
	{
		Window->Tick(InDeltaSeconds); // We need to make sure we tick after begin frame otherwise shape draws wont be properly submitted
	}

	if (!bHideAllWindows)
		Render(InDeltaSeconds);
	//OnImGuiDraw.Broadcast();

}

void UImGuiWindowManager::Render(float DeltaTime)
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::DockSpaceOverViewport(0, 0, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingOverCentralNode | ImGuiDockNodeFlags_AutoHideTabBar);
	ImGui::PopStyleColor(1);

	const bool bCompactSaved = SettingsWindow->GetSettingsConfig()->bCompactMode;
	if (bCompactSaved)
	{
		UImGuiHelperWidgets::PushStyleCompact();
	}

	if (Context.GetEnableInput())
	{
		RenderMainMenu();
	}

	for (FImGuiWindow* Window : Windows)
	{
		if (Window->IsVisible())
		{
			if (SettingsWindow->GetSettingsConfig()->bTransparentMode)
			{
				ImGui::SetNextWindowBgAlpha(0.35f);
			}

			Window->Render(DeltaTime);
		}
	}

	if (bCompactSaved)
	{
		UImGuiHelperWidgets::PopStyleCompact();
	}
}

void UImGuiWindowManager::EndFrame(UWorld* InWorld, ELevelTick InLevelTick, float InDeltaSeconds)
{
	// Draw Widgets (submit this at end frame so external systems can register ImGui drawers)
	FImGuiDebugDraw::Draw();

	Context.EndFrame();
}

void UImGuiWindowManager::RenderMainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        for (FMenu& Menu : MainMenu.SubMenus)
        {
            RenderOptionMenu(Menu);
        }

        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Close All Windows"))
            {
                CloseAllWindows();
            }

            ImGui::Separator();

            //RenderMenuItem(*LayoutsWindow, "Layouts");

            RenderMenuItem(*SettingsWindow, "Settings");

            /*if (ImGui::BeginMenu("Spacing"))
            {
                for (FImGuiWindow* SpaceWindow : SpaceWindows)
                {
                    bool bSpaceVisible = SpaceWindow->GetIsVisible();
                    if (ImGui::MenuItem(TCHAR_TO_ANSI(*SpaceWindow->GetName()), nullptr, &bSpaceVisible))
                    {
                        SpaceWindow->SetIsVisible(bSpaceVisible);
                    }
                }
                ImGui::EndMenu();
            }*/

        	if (ImGui::BeginMenu("Widgets"))
        	{
        		for (int32 i = 0; i < Widgets.Num(); ++i)
        		{
        			FImGuiWindow* Window = Widgets[i];

        			ImGui::PushID(i);

        			bool Visible = Window->GetIsWidgetVisible();
        			if (ImGui::Checkbox(TCHAR_TO_ANSI(*Window->GetName()), &Visible))
        			{
        				Window->SetIsWidgetVisible(Visible);
        			}

        			if (ImGui::IsItemActive() && ImGui::IsItemHovered() == false)
        			{
        				const int iNext = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
        				if (iNext >= 0 && iNext < Widgets.Num())
        				{
        					Widgets[i] = Widgets[iNext];
        					Widgets[iNext] = Window;
        					ImGui::ResetMouseDragDelta();
        				}
        			}

        			if (i == 0)
        			{
        				ImGui::SameLine();
        				UImGuiHelperWidgets::HelpMarker("Drag and drop the widget names to reorder them.");
        			}

        			ImGui::PopID();
        		}

        		ImGui::EndMenu();
        	}

            ImGui::EndMenu();
        }

    	        const float MinCursorX = ImGui::GetCursorPosX();
        float CursorX = ImGui::GetWindowWidth();

        //------------------------------------------------------------
        // Render in reverse order because it makes more sense
        // when looking at the widget ordered list in the UI.
        //------------------------------------------------------------
        for (int32 WindowIndex = Widgets.Num() - 1;  WindowIndex >= 0; WindowIndex--)
        {
            FImGuiWindow* Window = Widgets[WindowIndex];

            if (Window->GetIsWidgetVisible() == false)
            {
                continue;
            }

            TArray<float> SubWidgetsWidths;
            float SimCursorX = CursorX;
            for (int32 SubWidgetIndex = 0; ; ++SubWidgetIndex)
            {
                const float MaxWidth = SimCursorX - MinCursorX;
                float SubWidgetWidth = Window->GetMainMenuWidgetWidth(SubWidgetIndex, MaxWidth);
                if (SubWidgetWidth == -1)
                {
                    break;
                }

                SimCursorX -= SubWidgetWidth;
                SubWidgetsWidths.Add(SubWidgetWidth);
            }

            bool Stop = false;
            for (int32 SubWidgetIndex = SubWidgetsWidths.Num() - 1; SubWidgetIndex >= 0; SubWidgetIndex--)
            {
                const float SubWidgetWidth = SubWidgetsWidths[SubWidgetIndex];
                const float MaxWidth = CursorX - MinCursorX;

                //-------------------------------------------
                // Bypass this subwidget if its width is 0
                //-------------------------------------------
                if (SubWidgetWidth == 0)
                {
                    continue;
                }

                //-------------------------------------------
                // Stop drawing if there is not enough room
                //-------------------------------------------
                if (SubWidgetWidth > MaxWidth)
                {
                    Stop = true;
                    break;
                }

                CursorX -= SubWidgetWidth;
                ImGui::SetCursorPosX(CursorX);

                Window->RenderMainMenuWidget(SubWidgetIndex, SubWidgetWidth);
            }

            if (Stop)
            {
                break;
            }

            CursorX -= ImGui::GetStyle().ItemSpacing.x;
        }

        ImGui::EndMainMenuBar();
    }
}

UImGuiWindowManager::FMenu* UImGuiWindowManager::AddMenu(const FString& Name)
{
	TArray<FString> Path;
	Name.ParseIntoArray(Path, TEXT("."));

	FMenu* CurrentMenu = &MainMenu;
	for (int i = 0; i < Path.Num(); ++i)
	{
		FString MenuName = Path[i];

		int SubMenuIndex = CurrentMenu->SubMenus.IndexOfByPredicate([&](const FMenu& Menu) { return Menu.Name == MenuName; });
		if (SubMenuIndex != -1)
		{
			CurrentMenu = &CurrentMenu->SubMenus[SubMenuIndex];
		}
		else
		{
			CurrentMenu = &CurrentMenu->SubMenus.AddDefaulted_GetRef();
			CurrentMenu->Name = MenuName;
		}
	}

	return CurrentMenu;
}

void UImGuiWindowManager::RenderOptionMenu(FMenu& Menu)
{
	if (Menu.Window != nullptr)
	{
		RenderMenuItem(*Menu.Window, TCHAR_TO_ANSI(*Menu.Name));
	}
	else
	{
		if (ImGui::BeginMenu(TCHAR_TO_ANSI(*Menu.Name)))
		{
			for (FMenu& SubMenu : Menu.SubMenus)
			{
				RenderOptionMenu(SubMenu);
			}
			ImGui::EndMenu();
		}
	}
}

void UImGuiWindowManager::RenderMenuItem(FImGuiWindow& Window, const char* MenuItemName)
{
	if (SettingsWindow->GetSettingsConfig()->bShowWindowsInMainMenu)
	{
		ImGui::SetNextWindowSizeConstraints(
			ImVec2(FONT_WIDTH * 40, ImGui::GetTextLineHeightWithSpacing() * 5),
			ImVec2(FONT_WIDTH * 50, ImGui::GetTextLineHeightWithSpacing() * 60));

		if (ImGui::BeginMenu(MenuItemName))
		{
			Window.RenderContent();
			ImGui::EndMenu();
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			Window.SetIsVisible(!Window.IsVisible());
		}

		RenderMenuItemHelp(Window);
	}
	else
	{
		bool bIsVisible = Window.IsVisible();
		if (ImGui::MenuItem(MenuItemName, nullptr, &bIsVisible))
		{
			Window.SetIsVisible(bIsVisible);
		}

		RenderMenuItemHelp(Window);
	}
}

void UImGuiWindowManager::RenderMenuItemHelp(FImGuiWindow& Window)
{
	//if (SettingsWindow->GetSettingsConfig()->bShowHelp)
	{
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - FONT_WIDTH * 3.0f);
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(29, 42, 62, 240));
			const float HelpWidth = FONT_WIDTH * 80;
			ImGui::SetNextWindowSizeConstraints(ImVec2(HelpWidth / 2.0f, 0.0f), ImVec2(HelpWidth, FLT_MAX));
			if (ImGui::BeginTooltip())
			{
				ImGui::PushTextWrapPos(HelpWidth - 1 * FONT_WIDTH);
				Window.RenderHelp();
				ImGui::Separator();
				ImGui::TextDisabled("Help can be hidden in Window/Settings.");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
			ImGui::PopStyleColor();
		}
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(FONT_WIDTH * 1, 0));
	}

}

/*--------------------------------------------------------------------------------------------------------------
* General Utility
*--------------------------------------------------------------------------------------------------------------*/

void UImGuiWindowManager::CloseAllWindows()
{
	for (FImGuiWindow* Window : Windows) Window->SetIsVisible(false);
}

void UImGuiWindowManager::AddWindow(FImGuiWindow* Window, const FString& Name, bool AddToMainMenu)
{
	Window->SetFullName(Name);
	Window->SetOwner(this);
	//Window->Initialize(); (we do this when the ImGui context initializes & shuts down)
	Windows.Add(Window);

	if (Window->HasWidget())
	{
		Widgets.Add(Window);
	}

	if (AddToMainMenu)
	{
		if (FMenu* Menu = AddMenu(Window->GetFullName()))
		{
			Menu->Window = Window;
		}
	}
}

void UImGuiWindowManager::RemoveWindow(const FString& Name)
{
	for (int w = Windows.Num() - 1; w >= 0; --w)
	{
		auto window = Windows[w];
		if (window->GetFullName().Equals(Name))
		{
			Windows.RemoveAtSwap(w);
			delete window;
		}
	}
}

UImGuiWindowConfig* UImGuiWindowManager::GetConfig(const TSubclassOf<UImGuiWindowConfig> ConfigClass)
{
	const UClass* Class = ConfigClass.Get();

	for (UImGuiWindowConfig* Config : Configs)
	{
		if (Config && Config->IsA(Class))
		{
			return Cast<UImGuiWindowConfig>(Config);
		}
	}

	UImGuiWindowConfig* Config = NewObject<UImGuiWindowConfig>(this, Class);
	Configs.Add(Config);
	return Config;
}

const UObject* UImGuiWindowManager::GetAsset(const TSubclassOf<UObject> AssetClass) const
{
	const UClass* Class = AssetClass.Get();

	for (const UObject* Asset : Assets)
	{
		if (Asset && Asset->IsA(Class))
		{
			return Asset;
		}
	}

	const UObject* Asset = FImGuiWindowHelper::GetFirstAssetByClass(AssetClass);
	if (Asset == nullptr)
	{
		return nullptr;
	}

	Assets.Add(Asset);

	return Asset;
}

/*--------------------------------------------------------------------------------------------------------------
* Input Bindings For Console Commands
*--------------------------------------------------------------------------------------------------------------*/

bool UImGuiWindowManager::RegisterDefaultCommandBindings()
{
	if (!GetWorld()) return false;
	auto PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return false;

	UPlayerInput* PlayerInput = PC->PlayerInput;

	const UImGuiSettings* Settings = UImGuiSettings::Get();

	AddCommand(PlayerInput, ToggleInputCommand, Settings->ToggleInput);
	for (auto windowBind : Settings->WindowToggles)
	{
		const FString Command = FString::Printf(TEXT("%s %s"), *OpenWindowCommand, *windowBind.Key);
		AddCommand(PlayerInput, Command, windowBind.Value);
	}

	SortCommands(PlayerInput);
	PlayerInput->SaveConfig();
	return true;
}

void UImGuiWindowManager::AddCommand(UPlayerInput* PlayerInput, const FString& Command, const FImGuiKeyInfo& Key)
{
	//---------------------------------------------------
	// Reassign conflicting commands
	//---------------------------------------------------
	/*for (FKeyBind& KeyBind : PlayerInput->DebugExecBindings)
	{
		if (KeyBind.Key == Key && KeyBind.Command != Command)
		{
			KeyBind.Control = true;
			KeyBind.bIgnoreCtrl = false;
		}
	}*/

	//---------------------------------------------------
	// Find or add desired command
	//---------------------------------------------------
	FKeyBind* ExistingKeyBind = PlayerInput->DebugExecBindings.FindByPredicate([Command](const FKeyBind& KeyBind) { return KeyBind.Command == Command; });
	if (ExistingKeyBind == nullptr)
	{
		ExistingKeyBind = &PlayerInput->DebugExecBindings.AddDefaulted_GetRef();
	}

	//---------------------------------------------------
	// Assign the key to the command
	//---------------------------------------------------
	FKeyBind KeyBind = Key.KeyBind();
	KeyBind.Command = Command;

	*ExistingKeyBind = KeyBind;
}

void UImGuiWindowManager::SortCommands(UPlayerInput* PlayerInput)
{
	PlayerInput->DebugExecBindings.Sort([](const FKeyBind& Key1, const FKeyBind& Key2)
	{
		return Key1.Command.Compare(Key2.Command) < 0;
	});
}

void UImGuiWindowManager::ToggleInputMode()
{
	Context.SetEnableInput(!Context.GetEnableInput());
}

void UImGuiWindowManager::OpenWindow(const FString& Args)
{
	for (FImGuiWindow* Window : Windows)
	{
		if (Window->GetName().ToLower().Equals(Args.ToLower()))
		{
			Window->SetIsVisible(!Window->IsVisible());
			return;
		}
	}
}
