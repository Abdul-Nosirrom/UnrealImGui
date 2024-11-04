#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "imgui.h"
#include "Templates/SharedPointer.h"
#include "UObject/StrongObjectPtr.h"

class FImGuiContext;
class IInputProcessor;
class SImGuiWidget;
class SImGuiWidget;
class SImGuiInputCatcherWidget;
class SWidget;
class SWindow;
class UGameViewportClient;
class ULocalPlayer;
struct FDisplayMetrics;
struct ImPlotContext;

struct IMGUI_API FImGuiViewportData
{
	TWeakPtr<SWindow> Window = nullptr;
	FImGuiContext* Context = nullptr;
	TWeakPtr<SImGuiWidget> Widget = nullptr;
};

class IMGUI_API FImGuiContext : public TSharedFromThis<FImGuiContext>
{
	friend class UImGuiWindowManager;

public:

	void Initialize();

	void Shutdown();

	bool GetEnableInput() const { return bEnableInput; }

	void SetEnableInput(bool Value);

	bool GetWantCaptureMouse() const { return bWantCaptureMouse; }

	bool GetShareMouse() const { return bShareMouse; }

	void SetShareMouse(bool Value);

	bool GetShareMouseWithGameplay() const { return bShareMouseWithGameplay; }

	void SetShareMouseWithGameplay(bool Value);

	bool GetShareKeyboard() const { return bShareKeyboard; }

	void SetShareKeyboard(bool Value) { bShareKeyboard = Value; }

	void BeginFrame();

	void EndFrame();

	float GetDpiScale() const { return DpiScale; }

	void SetDPIScale(float Value);

	TObjectPtr<const UGameViewportClient> GetGameViewport() const { return GameViewport; }

	TSharedPtr<const SImGuiWidget> GetMainWidget() const { return MainWidget; }

private:

	bool BeginFrame(float InDeltaTime);

	void OnDisplayMetricsChanged(const FDisplayMetrics& DisplayMetrics) const;

	bool IsConsoleOpened() const;

	void DrawDebug();

	void BuildFont();

	void RefreshMouseCursor();

	ULocalPlayer* GetLocalPlayer() const;

	static void ImGui_CreateWindow(ImGuiViewport* Viewport);

	static void ImGui_DestroyWindow(ImGuiViewport* Viewport);

	static void ImGui_ShowWindow(ImGuiViewport* Viewport);

	static void ImGui_SetWindowPos(ImGuiViewport* Viewport, ImVec2 Pos);

	static ImVec2 ImGui_GetWindowPos(ImGuiViewport* Viewport);

	static void ImGui_SetWindowSize(ImGuiViewport* Viewport, ImVec2 Size);

	static ImVec2 ImGui_GetWindowSize(ImGuiViewport* Viewport);

	static void ImGui_SetWindowFocus(ImGuiViewport* Viewport);

	static bool ImGui_GetWindowFocus(ImGuiViewport* Viewport);

	static bool ImGui_GetWindowMinimized(ImGuiViewport* Viewport);

	static void ImGui_SetWindowTitle(ImGuiViewport* Viewport, const char* TitleAnsi);

	static void ImGui_SetWindowAlpha(ImGuiViewport* Viewport, float Alpha);

	static void ImGui_RenderWindow(ImGuiViewport* Viewport, void* Data);

	UPROPERTY()
	UTexture2D* FontAtlasTexture = nullptr;

	TMap<TWeakPtr<SWindow>, ImGuiID> WindowToViewportMap;

	TSharedPtr<IInputProcessor> InputProcessor = nullptr;

	TStrongObjectPtr<UTexture2D> FontAtlasTexturePtr = nullptr;

	TSharedPtr<SImGuiWidget> MainWidget = nullptr;

	TSharedPtr<SImGuiInputCatcherWidget> InputCatcherWidget = nullptr;

	TWeakPtr<SWidget> PreviousMouseCaptor = nullptr;

	TObjectPtr<UGameViewportClient> GameViewport = nullptr;

	ImGuiContext* ImGuiContext = nullptr;

	ImPlotContext* PlotContext = nullptr;

	char IniFilename[512] = {};

	bool bEnableInput = false;

	bool bShareMouse = false;

	bool bShareMouseWithGameplay = false;

	bool bHasSavedInitialCursorVisibility = false;

	bool bIsCursorInitiallyVisible = false;

	bool bShareKeyboard = false;

	bool bRefreshDPIScale = false;

	bool bIsThrottleDisabled = false;

	bool bIsFirstFrame = true;

	bool bCalledBeginFrame = false;

	bool bWantCaptureMouse = false;

	float DpiScale = 1.f;
};
