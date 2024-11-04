// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"
#include <imgui.h>

class UImGuiWindowConfig;

class IMGUIWINDOW_API FImGuiWindow
{
	friend class UImGuiWindowManager;

public:
	virtual ~FImGuiWindow() {}

	/// @brief	Called when the window starts its render loop
	virtual void Initialize() {}
	/// @brief	Called when the window stops its render loop
	virtual void Shutdown() {}

	/// @brief	Main render loop for this window when the ImGuiWindow is visible (within Begin/End)
	virtual void Render(float DeltaTime);
	/// @brief	Called every frame with a valid ImGui context even if the window is hidden (outside of Begin/End)
	virtual void Tick(float DeltaTime);
	/// @brief	Add a widget to the main menu bar associated with this window (e.g actor selection, widget displays name of actor selected)
	virtual void RenderMainMenuWidget(int32 SubWidgetIndex, float Width) {}
	/// @brief	Pass custom width for main menu bar widget
	virtual float GetMainMenuWidgetWidth(int32 SubWidgetIndex, float MaxWidth) { return -1.0f; }

	// Window Visibility
	bool IsVisible() const { return bIsVisible; }
	void SetIsVisible(bool bVisibility) { bIsVisible = bVisibility; }
	// ~

	// Associated widgets visibility/query
	bool HasWidget() const { return bHasWidget; }
	bool GetIsWidgetVisible() const { return bIsWidgetVisible; }
	void SetIsWidgetVisible(bool Value) { bIsWidgetVisible = Value; }
	int32 GetWidgetOrderIndex() const { return WidgetOrderIndex; }
	void SetWidgetOrderIndex(int32 Value) { WidgetOrderIndex = Value; }
	// ~

	/// @brief	The full name of the window, that contains the path in the main menu. For example "Gameplay.Character.Effect"
	const FString& GetFullName() const { return FullName; }
	void SetFullName(const FString& InFullName);
	/// @brief	The short name of the window. "Effect" if the window full name is "Gameplay.Character.Effect"
	const FString& GetName() const { return Name; }

	void SetOwner(UImGuiWindowManager* InOwner) { Owner = InOwner; }
	UImGuiWindowManager* GetOwner() const { return Owner; }

	AActor* GetSelection() const { return CurrentSelection.Get(); }
	void SetSelection(AActor* Actor);

	virtual void PreSaveConfig() {}
	virtual void ResetConfig() {}

	template<class T>
	T* GetConfig() { return Cast<T>(GetConfig(T::StaticClass())); }
	UImGuiWindowConfig* GetConfig(const TSubclassOf<UImGuiWindowConfig> ConfigClass) const;
	template<class T>
	const T* GetAsset() { return Cast<T>(GetAsset(T::StaticClass())); }
	const UObject* GetAsset(const TSubclassOf<UObject> AssetClass) const;

	ImGuiID GetID() const { return ID; }

protected:

	virtual const FString& GetTitle() const { return Title; }

	virtual UWorld* GetWorld() const;

	/// @brief	Tooltip render function
	virtual void RenderHelp() { ImGui::Text("No help"); }
	/// @brief	Allows initialization for rendering the ImGui shit + setting up custom window flags for it (also pushing style sets)
	virtual void PreRender(ImGuiWindowFlags& WindowFlags) {}
	/// @brief	Rendering of the main ImGuiWindow for this class
	virtual void RenderContent() {}
	/// @brief	Called after rendering the contents of this window (e.g popping style sets)
	virtual void PostRender() {}

	/// @brief	Callback function when the given actor selection changes
	virtual void OnSelectionChanged(AActor* OldSelection, AActor* NewSelection) {}

	APawn* GetLocalPlayerPawn() const;
	APlayerController* GetLocalPlayerController() const;
	ULocalPlayer* GetLocalPlayer() const;

protected:

	bool bHideMenu = false;

	bool bNoPadding = false;

	bool bHasMenu = false;

	/// @brief	Should this window be visible
	bool bIsVisible = false;

	/// @brief	Does this window have some non-gui widget drawn to the screen? Like a bounding box
	bool bHasWidget = false;

	/// @brief	Should this windows widget be visible
	bool bIsWidgetVisible = false;

	int32 WidgetOrderIndex = -1;

	ImGuiID ID;

	FString FullName;

	FString Name;

	FString Title;

	UImGuiWindowManager* Owner = nullptr; // this has singleton like access so maybe we dont really need this member

	TWeakObjectPtr<AActor> CurrentSelection;

	TWeakObjectPtr<AActor> OverridenSelection;
};
