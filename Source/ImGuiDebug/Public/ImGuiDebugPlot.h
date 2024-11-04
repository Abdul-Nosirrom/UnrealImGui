// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ImGuiConfig.h"
#include "imgui.h"
#include "implot.h"
#include "ImGuiDebugPlot.generated.h"

#ifdef ENABLE_IMGUI

struct FImGuiDebugPlotEntry;

//--------------------------------------------------------------------------------------------------------------------------
struct IMGUIDEBUG_API FImGuiDebugPlotEventParams
{
    FName Name;
    FString Value;
};

//--------------------------------------------------------------------------------------------------------------------------
struct IMGUIDEBUG_API FImGuiDebugPlotEvent
{
    float GetActualEndTime(const FImGuiDebugPlotEntry& Plot) const;

    uint64 GetActualEndFrame(const FImGuiDebugPlotEntry& Plot) const;

    FImGuiDebugPlotEvent& AddParam(const FName Name, bool Value);

    FImGuiDebugPlotEvent& AddParam(const FName Name, int Value);

    FImGuiDebugPlotEvent& AddParam(const FName Name, float Value);

    FImGuiDebugPlotEvent& AddParam(const FName Name, FName Value);

    FImGuiDebugPlotEvent& AddParam(const FName Name, const FString& Value);

    FName Id;
    float StartTime = 0.0f;
    float EndTime = 0.0f;
    uint64 StartFrame = 0;
    uint64 EndFrame = 0;
    ImU32 BorderColor;
    ImU32 FillColor;
    int32 Row;
    FString OwnerName;
    FString DisplayName;
    TArray<FImGuiDebugPlotEventParams> Params;
};

//--------------------------------------------------------------------------------------------------------------------------
struct IMGUIDEBUG_API FImGuiDebugPlotEntry
{
    void AssignAxis(int32 AssignedRow, ImAxis CurrentYAxis);
    void AddPoint(float X, float Y);
    bool FindValue(float Time, float& Value) const;
    void ResetAxis();
    void Clear();
    FImGuiDebugPlotEvent& AddEvent(const FImGuiDebugPlotEntry& OwnwePlot, FString OwnerName, bool IsInstant, const FName EventId, const int32 Row, const FColor& Color);
    FImGuiDebugPlotEvent& StopEvent(const FName EventId);
    void UpdateTime(const UWorld* World);
    int32 FindFreeRow() const;
    FImGuiDebugPlotEvent* GetLastEvent();
    FImGuiDebugPlotEvent* FindLastEventByName(FName EventId);

    FName Name;
    bool IsEventPlot = false;
    int32 CurrentRow = INDEX_NONE;
    ImAxis CurrentYAxis = ImAxis_COUNT;
    float Time = 0;
    uint64 Frame = 0;

    //--------------------------
    // Values
    //--------------------------
    int32 ValueOffset = 0;
    ImVector<ImVec2> Values;
    bool ShowValuesMarkers = false;

    //--------------------------
    // Events
    //--------------------------
    int32 EventOffset = 0;
    TArray<FImGuiDebugPlotEvent> Events;
    int32 MaxRow = 1;
};

//--------------------------------------------------------------------------------------------------------------------------

class IMGUIDEBUG_API FImGuiDebugPlot
{
public:
    static const int32 AutoRow = -1;

    static void PlotValue(const UObject* WorldContextObject, const FName PlotName, const float Value);
    static FImGuiDebugPlotEvent& PlotEvent(const UObject* WorldContextObject, const FName PlotName, const FName EventId, bool IsInstant, const int32 Row = AutoRow, const FColor& Color = FColor::Transparent);
    static FImGuiDebugPlotEvent& PlotEventInstant(const UObject* WorldContextObject, const FName PlotName, const FName EventId, const int32 Row = AutoRow, const FColor& Color = FColor::Transparent);
    static FImGuiDebugPlotEvent& PlotEventStart(const UObject* WorldContextObject, const FName PlotName, const FName EventId, const int32 Row = AutoRow, const FColor& Color = FColor::Transparent);
    static FImGuiDebugPlotEvent& PlotEventStop(const UObject* WorldContextObject, const FName PlotName, const FName EventId);
    static FImGuiDebugPlotEvent& PlotEventToggle(const UObject* WorldContextObject, const FName PlotName, const FName EventId, const bool ToggleValue, const int32 Row = AutoRow, const FColor& Color = FColor::Transparent);

    static void Reset();
    static void Clear();
    static FImGuiDebugPlotEntry* FindPlot(const FName Name);

    static TArray<FImGuiDebugPlotEntry> Plots;
    static bool IsVisible;
    static bool Pause;

private:
    friend struct FImGuiDebugPlotEntry;

    static void ResetLastAddedEvent();
    static FImGuiDebugPlotEntry* RegisterPlot(const UObject* Owner, const FName PlotName, bool IsEventPlot);
    FImGuiDebugPlotEventParams* PlotEventAddParam(const FName Name);
    static FImGuiDebugPlotEvent* GetLastAddedEvent();

    static FName LastAddedEventPlotName;
    static int32 LastAddedEventIndex;

    static FImGuiDebugPlotEvent DefaultEvent;
};

#endif //ENABLE_IMGUI


UCLASS(meta = (ScriptName = "ImGuiDebugPlot", DisplayName = "ImGuiDebugPlot"))
class IMGUIDEBUG_API UImGuiDebugPlotLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DevelopmentOnly))
	static void Plot(const UObject* Owner, const FName Name, const float Value)
	{
#if ENABLE_IMGUI
		FImGuiDebugPlot::PlotValue(Owner, Name, Value);
#endif
	}
};
