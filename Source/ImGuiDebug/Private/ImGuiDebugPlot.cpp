// Copyright 2023 CoC All rights reserved


#include "ImGuiDebugPlot.h"
#include "ImGuiDebug.h"
#include "ImGuiHelper.h"

FImGuiDebugPlotEvent FImGuiDebugPlot::DefaultEvent;
TArray<FImGuiDebugPlotEntry> FImGuiDebugPlot::Plots;
bool FImGuiDebugPlot::IsVisible = false;
bool FImGuiDebugPlot::Pause = false;
FName FImGuiDebugPlot::LastAddedEventPlotName = NAME_None;
int32 FImGuiDebugPlot::LastAddedEventIndex = INDEX_NONE;

//--------------------------------------------------------------------------------------------------------------------------
// FImGuiPlotEvent
//--------------------------------------------------------------------------------------------------------------------------
float FImGuiDebugPlotEvent::GetActualEndTime(const FImGuiDebugPlotEntry& Plot) const
{
    const float ActualEndTime = EndTime == 0.0f ? Plot.Time : EndTime;
    return ActualEndTime;
}

//--------------------------------------------------------------------------------------------------------------------------
uint64 FImGuiDebugPlotEvent::GetActualEndFrame(const FImGuiDebugPlotEntry& Plot) const
{
    const float ActualEndFame = EndFrame == 0.0f ? Plot.Frame : EndFrame;
    return ActualEndFame;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlotEvent::AddParam(const FName Name, bool Value)
{
    if (FImGuiDebugPlot::IsVisible)
    {
        AddParam(Name, FString::Printf(TEXT("%s"), Value ? TEXT("True") : TEXT("False")));
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlotEvent::AddParam(const FName Name, int Value)
{
    if (FImGuiDebugPlot::IsVisible)
    {
        AddParam(Name, FString::Printf(TEXT("%d"), Value));
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlotEvent::AddParam(const FName Name, float Value)
{
    if (FImGuiDebugPlot::IsVisible)
    {
        AddParam(Name, FString::Printf(TEXT("%0.2f"), Value));
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlotEvent::AddParam(const FName Name, FName Value)
{
    if (FImGuiDebugPlot::IsVisible)
    {
        AddParam(Name, Value.ToString());
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlotEvent::AddParam(const FName Name, const FString& Value)
{
    if (FImGuiDebugPlot::IsVisible)
    {

        if (Name == "Name")
        {
            DisplayName = Value;
        }
        else
        {
            FImGuiDebugPlotEventParams& Param = Params.AddDefaulted_GetRef();
            Param.Name = Name;
            Param.Value = Value;
        }
    }

    return *this;
}


//--------------------------------------------------------------------------------------------------------------------------
// FImGuiPlotEntry
//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugPlotEntry::AddPoint(float X, float Y)
{
    if (Values.Capacity == 0)
    {
        Values.reserve(2000);
    }

    if (Values.size() < Values.Capacity)
    {
        Values.push_back(ImVec2(X, Y));
    }
    else
    {
        Values[ValueOffset] = ImVec2(X, Y);
        ValueOffset = (ValueOffset + 1) % Values.size();
    }
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlotEntry::AddEvent(
    const FImGuiDebugPlotEntry& OwnwePlot,
    FString OwnerName,
    bool IsInstant,
    const FName EventId,
    const int32 Row,
    const FColor& Color)
{
    if (Events.Max() < 200)
    {
        Events.Reserve(200);
    }

    //-----------------------------------------------------------------------
    // We currently having two events with the same name at the same time.
    // So we stop the current one if any exist.
    //-----------------------------------------------------------------------
    StopEvent(EventId);

    FImGuiDebugPlotEvent* Event = nullptr;

    int32 AddedIndex = 0;
    if (Events.Num() < Events.Max())
    {
        Event = &Events.AddDefaulted_GetRef();
        AddedIndex = Events.Num() - 1;
    }
    else
    {
        Event = &Events[EventOffset];
        AddedIndex = EventOffset;
        EventOffset = (EventOffset + 1) % Events.Num();
    }

    Event->Id = EventId;
    Event->OwnerName = OwnerName;
    Event->DisplayName = EventId.ToString();
    Event->StartTime = OwnwePlot.Time;
    Event->EndTime = IsInstant ? OwnwePlot.Time : 0.0f;
    Event->StartFrame = OwnwePlot.Frame;
    Event->EndFrame = IsInstant ? OwnwePlot.Frame : 0.0f;
    Event->Row = (Row == FImGuiDebugPlot::AutoRow) ? OwnwePlot.FindFreeRow() : Row;

    MaxRow = FMath::Max(Event->Row, MaxRow);

    const FColor BorderColor = FImGuiDebugHelper::GetAutoColor(EventId, Color).WithAlpha(200);
    const FColor FillColor = BorderColor.WithAlpha(100);
    Event->BorderColor = FImGuiHelper::ToImColor(BorderColor);
    Event->FillColor = FImGuiHelper::ToImColor(FillColor);

    FImGuiDebugPlot::LastAddedEventPlotName = OwnwePlot.Name;
    FImGuiDebugPlot::LastAddedEventIndex = AddedIndex;

    return *Event;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlotEntry::StopEvent(const FName EventId)
{
    FImGuiDebugPlotEvent* Event = FindLastEventByName(EventId);
    if (Event == nullptr)
    {
        return FImGuiDebugPlot::DefaultEvent;
    }

    if (Event->EndTime == 0.0f)
    {
        Event->EndTime = Time;
        Event->EndFrame = Frame;
    }

    return *Event;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugPlotEntry::UpdateTime(const UWorld* World)
{
    Time = World ? World->GetTimeSeconds() : 0.0;
    Frame = GFrameCounter;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent* FImGuiDebugPlotEntry::GetLastEvent()
{
    if (Events.Num() == 0)
    {
        return nullptr;
    }

    int32 Index = Events.Num() - 1;
    if (EventOffset != 0)
    {
        Index = (Index + EventOffset) % Events.Num();
    }

    return &Events[Index];
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent* FImGuiDebugPlotEntry::FindLastEventByName(FName EventId)
{
    for (int32 i = Events.Num() - 1; i >= 0; --i)
    {
        //--------------------------------------------------
        // The array cycle so we must offset the index
        //--------------------------------------------------
        int32 Index = i;
        if (EventOffset != 0)
        {
            Index = (i + EventOffset) % Events.Num();
        }

        if (Events[Index].Id == EventId)
        {
            return &Events[Index];
        }
    }

    return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------
int32 FImGuiDebugPlotEntry::FindFreeRow() const
{
    static float InstantTimeThreshold = 1.0f;
    static float TotalTimeThreshold = 10.0f;
    TSet<int32> OccupiedRows;

    for (int32 i = Events.Num() - 1; i >= 0; --i)
    {
        int32 Index = i;
        if (EventOffset != 0)
        {
            Index = (i + EventOffset) % Events.Num();
        }
        const FImGuiDebugPlotEvent& Event = Events[Index];

        if (Event.EndTime != 0.0f && Time > Event.EndTime + TotalTimeThreshold)
        {
            break;
        }

        if (Event.StartTime == Event.EndTime && Time > Event.EndTime + InstantTimeThreshold)
        {
            continue;
        }

        if (Event.EndTime != 0.0f)
        {
            continue;
        }

        OccupiedRows.Add(Event.Row);
    }

    int32 FreeRow = 0;
    while (true)
    {
        if (OccupiedRows.Contains(FreeRow) == false)
        {
            break;
        }

        FreeRow++;
    }

    return FreeRow;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugPlotEntry::AssignAxis(int32 Row, ImAxis YAxis)
{
    CurrentRow = Row;
    CurrentYAxis = YAxis;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugPlotEntry::ResetAxis()
{
    CurrentRow = INDEX_NONE;
    CurrentYAxis = ImAxis_COUNT;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugPlotEntry::Clear()
{
    FImGuiDebugPlot::ResetLastAddedEvent();

    MaxRow = 0;

    if (Values.size() > 0)
    {
        Values.shrink(0);
        ValueOffset = 0;
    }

    if (Events.Num() > 0)
    {
        Events.Empty();
        Events.Shrink();
        EventOffset = 0;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
bool FImGuiDebugPlotEntry::FindValue(float x, float& y) const
{
    y = 0.0f;

    bool FoundAfter = false;
    bool FoundBefore = false;

    for (int32 i = Values.size() - 1; i >= 0; --i)
    {
        //--------------------------------------------------
        // The array cycle so we must offset the index
        //--------------------------------------------------
        int32 Index = i;
        if (ValueOffset != 0)
        {
            Index = (i + ValueOffset) % Values.size();
        }

        ImVec2 Point = Values[Index];
        if (Point.x > x)
        {
            FoundAfter = true;
        }

        if (Point.x < x)
        {
            FoundBefore = true;
        }

        if (FoundAfter && FoundBefore)
        {
            y = Point.y;
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------------
// FImGuiPlot
//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugPlot::Reset()
{
    Plots.Empty();
    Pause = false;
    ResetLastAddedEvent();
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugPlot::Clear()
{
    for (FImGuiDebugPlotEntry& Entry : FImGuiDebugPlot::Plots)
    {
        Entry.Clear();
    }

    ResetLastAddedEvent();
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugPlot::ResetLastAddedEvent()
{
    LastAddedEventPlotName = NAME_None;
    LastAddedEventIndex = INDEX_NONE;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent* FImGuiDebugPlot::GetLastAddedEvent()
{
    FImGuiDebugPlotEntry* Plot = FindPlot(LastAddedEventPlotName);
    if (Plot == nullptr)
    {
        return nullptr;
    }

    return Plot->GetLastEvent();
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEntry* FImGuiDebugPlot::FindPlot(const FName Name)
{
    FImGuiDebugPlotEntry* Plot = Plots.FindByPredicate([Name](const FImGuiDebugPlotEntry& P) { return P.Name == Name; });
    return Plot;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEntry* FImGuiDebugPlot::RegisterPlot(const UObject* WorldContextObject, const FName PlotName, bool IsEventPlot)
{
    //----------------------------------------------------------
    // When not visible, we don't go further for performances.
    //----------------------------------------------------------
    if (IsVisible == false)
    {
        return nullptr;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    if (World == nullptr)
    {
        return nullptr;
    }

    if (FImGuiDebug::IsDebugActiveForObject(WorldContextObject) == false)
    {
        return nullptr;
    }

    FImGuiDebugPlotEntry* EntryPtr = FindPlot(PlotName);
    if (EntryPtr == nullptr)
    {
        EntryPtr = &Plots.AddDefaulted_GetRef();
        EntryPtr->Name = PlotName;
        EntryPtr->IsEventPlot = IsEventPlot;
        Plots.Sort([](const FImGuiDebugPlotEntry& A, const FImGuiDebugPlotEntry& B) { return A.Name.ToString().Compare(B.Name.ToString()) < 0; });
    }

    if (EntryPtr->CurrentYAxis == ImAxis_COUNT)
    {
        return nullptr;
    }

    const float Time = World->GetTimeSeconds();
    if (Time < EntryPtr->Time)
    {
        EntryPtr->Clear();
    }

    EntryPtr->Time = World->GetTimeSeconds();
    EntryPtr->Frame = GFrameCounter;

    return EntryPtr;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugPlot::PlotValue(const UObject* WorldContextObject, const FName PlotName, const float Value)
{
    FImGuiDebugPlotEntry* Plot = RegisterPlot(WorldContextObject, PlotName, false);
    if (Plot == nullptr)
    {
        return;
    }

    Plot->AddPoint(Plot->Time, Value);
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlot::PlotEvent(const UObject* WorldContextObject, const FName PlotName, const FName EventId, bool IsInstant, const int32 Row, const FColor& Color)
{
    FImGuiDebugPlotEntry* Plot = RegisterPlot(WorldContextObject, PlotName, true);
    if (Plot == nullptr)
    {
        ResetLastAddedEvent();
        return DefaultEvent;
    }

    FImGuiDebugPlotEvent& Event = Plot->AddEvent(*Plot, GetNameSafe(WorldContextObject), IsInstant, EventId, Row, Color);
    return Event;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlot::PlotEventInstant(const UObject* WorldContextObject, const FName PlotName, const FName EventId, const int32 Row, const FColor& Color)
{
    FImGuiDebugPlotEvent& Event = PlotEvent(WorldContextObject, PlotName, EventId, true, Row, Color);
    return Event;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlot::PlotEventStart(const UObject* WorldContextObject, const FName PlotName, const FName EventId, const int32 Row, const FColor& Color)
{
    FImGuiDebugPlotEvent& Event = PlotEvent(WorldContextObject, PlotName, EventId, false, Row, Color);
    return Event;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlot::PlotEventStop(const UObject* WorldContextObject, const FName PlotName, const FName EventId)
{
    FImGuiDebugPlotEntry* Plot = RegisterPlot(WorldContextObject, PlotName, true);
    if (Plot == nullptr)
    {
        return DefaultEvent;
    }

    FImGuiDebugPlotEvent& Event = Plot->StopEvent(EventId);
    return Event;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugPlotEvent& FImGuiDebugPlot::PlotEventToggle(const UObject* WorldContextObject, const FName PlotName, const FName EventId, const bool ToggleValue, const int32 Row, const FColor& Color)
{
    if (ToggleValue)
    {
        return PlotEventStart(WorldContextObject, PlotName, EventId, Row, Color);
    }
    else
    {
        return PlotEventStop(WorldContextObject, PlotName, EventId);
    }
}
