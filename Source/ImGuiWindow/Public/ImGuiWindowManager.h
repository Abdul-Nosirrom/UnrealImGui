// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ImGuiContext.h"
#include "ImGuiWindowManager.generated.h"

class FImGuiWindow;
class ILevelEditor;
struct FImGuiKeyInfo;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnImGuiDrawEvent);
//DECLARE_MULTICAST_DELEGATE(FImGuiDrawEvent)

UCLASS(Config = ImGui)
class IMGUIWINDOW_API UImGuiWindowConfig : public UObject
{
	GENERATED_BODY()

public:

	UImGuiWindowConfig()
	{
		Reset();
	}

	virtual void Reset()
	{
	}
};

// instead of binding to viewport events and such, maybe making a WorldSubsystem that ticks the WindowManager, windowmanager
// being an engine subsystem gives us the benefit of how we add windows (e.g doing it Startupmodule)

UCLASS(Config = ImGui, DefaultConfig)
class IMGUIWINDOW_API UImGuiWindowManager : public UEngineSubsystem
{
	GENERATED_BODY()

	friend class FImGuiWindowModule;
	friend class FImGuiWindow_Settings;

public:
	// Easy singleton like access
	static UImGuiWindowManager* Get()
	{
		if (GEngine)
			return GEngine->GetEngineSubsystem<UImGuiWindowManager>();
		return nullptr;
	}

protected:
	// BEGIN USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ END USubsystem

	// BEGIN ImGui Initialization & Rendering
	void InitializeGameImGui();//UWorld* InWorld, const UWorld::InitializationValues IVS);
	void ShutdownGameImGui(FViewport* InViewport);

	void BeginFrame(UWorld* InWorld, ELevelTick InLevelTick, float InDeltaSeconds);
	virtual void Render(float DeltaTime);
	void EndFrame(UWorld* InWorld, ELevelTick InLevelTick, float InDeltaSeconds);
	// ~

public:
	virtual void CloseAllWindows();
	virtual bool GetHideAllWindows() const { return bHideAllWindows; }
	virtual void SetHideAllWindows(bool Value) { bHideAllWindows = Value; }

	virtual void AddWindow(FImGuiWindow* Window, const FString& Name, bool AddToMainMenu = true);
	template<class T> T* AddWindow(const FString& Name, bool AddToMainMenu = true);

	virtual void RemoveWindow(const FString& Name);

	UImGuiWindowConfig* GetConfig(const TSubclassOf<UImGuiWindowConfig> ConfigClass);
	template<class T> T* GetConfig();
	virtual void ResetAllWindowsConfig() { for (auto config : Configs) if (config) config->Reset(); }

	const UObject* GetAsset(const TSubclassOf<UObject> AssetClass) const;
	template<typename T> T* GetAsset();

    const FImGuiContext& GetContext() const { return Context; }
    FImGuiContext& GetContext() { return Context; }

	virtual UWorld* GetWorld() const override { return InitializedWorld; }

protected:
	/// @brief	Menu bar item with reference to the window it opens
	struct FMenu
	{
		FString Name;
		FImGuiWindow* Window = nullptr;
		TArray<FMenu> SubMenus;
	};

	// Menu Bar Rendering
	virtual FMenu* AddMenu(const FString& Name);
	virtual void RenderMainMenu();
	virtual void RenderOptionMenu(FMenu& Menu);
	virtual void RenderMenuItem(FImGuiWindow& Window, const char* MenuItemName);
	void RenderMenuItemHelp(FImGuiWindow& Window);
	// ~

	// Input Bindings & Commands
	virtual bool RegisterDefaultCommandBindings();
	static void AddCommand(UPlayerInput* PlayerInput, const FString& Command, const FImGuiKeyInfo& Key);
	static void SortCommands(UPlayerInput* PlayerInput);

	void ToggleInputMode();
	void OpenWindow(const FString& Args);
	// ~

	UPROPERTY(Transient)
	UWorld* InitializedWorld;

	FImGuiContext Context;

	FMenu MainMenu;
	TArray<FImGuiWindow*> Windows;
	/// @brief	Widgets are 'window'/UI items placed on the main menu bar at top right
	TArray<FImGuiWindow*> Widgets;
	int32 WidgetsOrderIndex = 0;

	class FImGuiWindow_Settings* SettingsWindow;

	UPROPERTY()
	mutable TArray<UImGuiWindowConfig*> Configs;
	UPROPERTY()
	mutable TArray<const UObject*> Assets;

	bool bHideAllWindows = false;
	bool bIsInitialized = false;
	bool bInputInitialized = false;

	TArray<IConsoleCommand*> ConsoleCommands;

	static FString ToggleInputCommand;
	static FString OpenWindowCommand;
};


//--------------------------------------------------------------------------------------------------------------------------
template<class T>
T* UImGuiWindowManager::AddWindow(const FString& Name, bool AddToMainMenu)
{
	// Avoid double-adding windows so check if it exists (the type exists) (this is bad tho because subclassing exists lol)
	T* Window = nullptr;
	//if (DoesWindowExist(Window)) return Window;
	Window = new T();
	AddWindow(Window, Name, AddToMainMenu);
	return Window;
}

//--------------------------------------------------------------------------------------------------------------------------
template<class T>
T* UImGuiWindowManager::GetConfig()
{
	static_assert(TPointerIsConvertibleFromTo<T, const UImGuiWindowConfig>::Value);
	return Cast<T>(&GetConfig(T::StaticClass()));
}

//--------------------------------------------------------------------------------------------------------------------------
template<typename T>
T* UImGuiWindowManager::GetAsset()
{
	return Cast<T>(GetAsset(T::StaticClass()));
}
