// Copyright 2023 CoC All rights reserved


#include "ImGuiDebugDrawLibrary.h"
#include "ImGuiConfig.h"
#include "ImGuiDebugDraw.h"

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawString(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FString& Text, const FVector Location, const FLinearColor Color, const bool Persistent)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::String(*LogCategoryPtr, WorldContextObject, Text, Location, Color.ToFColor(true), Persistent);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawPoint(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector Location, const float Size, const FLinearColor Color, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Point(*LogCategoryPtr, WorldContextObject, Location, Size, Color.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawSegment(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector SegmentStart, const FVector SegmentEnd, const FLinearColor Color, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Segment(*LogCategoryPtr, WorldContextObject, SegmentStart, SegmentEnd, Color.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawArrow(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector SegmentStart, const FVector SegmentEnd, const FLinearColor Color, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Arrow(*LogCategoryPtr, WorldContextObject, SegmentStart, SegmentEnd, Color.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawAxis(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector Location, const FRotator Rotation, const float Scale, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Axis(*LogCategoryPtr, WorldContextObject, Location, Rotation, Scale, Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawSphere(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector Location, const float Radius, const FLinearColor Color, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Sphere(*LogCategoryPtr, WorldContextObject, Location, Radius, Color.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawBox(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector Center, const FVector Extent, const FQuat Rotation, const FLinearColor Color, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Box(*LogCategoryPtr, WorldContextObject, Center, Extent, Rotation, Color.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawSolidBox(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector Center, const FVector Extent, const FQuat Rotation, const FLinearColor Color, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::SolidBox(*LogCategoryPtr, WorldContextObject, Center, Extent, Rotation, Color.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawCapsule(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector Center, const float HalfHeight, const float Radius, const FQuat Rotation, const FLinearColor Color, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Capsule(*LogCategoryPtr, WorldContextObject, Center, HalfHeight, Radius, Rotation, Color.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawCircle(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FMatrix& Matrix, const float Radius, const FLinearColor Color, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Circle(*LogCategoryPtr, WorldContextObject, Matrix, Radius, Color.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawCircleArc(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FMatrix& Matrix, const float InnerRadius, const float OuterRadius, const float Angle, const FLinearColor Color, const bool Persistent, uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::CircleArc(*LogCategoryPtr, WorldContextObject, Matrix, InnerRadius, OuterRadius, Angle, Color.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawPoints(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const TArray<FVector>& Points, const float Radius, const FLinearColor StartColor, const FLinearColor EndColor, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Points(*LogCategoryPtr, WorldContextObject, Points, Radius, StartColor.ToFColor(true), EndColor.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawPath(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const TArray<FVector>& Points, const float PointSize, const FLinearColor StartColor, const FLinearColor EndColor, const bool Persistent, const uint8 DepthPriority)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Path(*LogCategoryPtr, WorldContextObject, Points, PointSize, StartColor.ToFColor(true), EndColor.ToFColor(true), Persistent, DepthPriority);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawString2D(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FString& Text, const FVector2D Location, const FLinearColor Color, const bool Persistent)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::String2D(*LogCategoryPtr, WorldContextObject, Text, Location, Color.ToFColor(true), Persistent);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawSegment2D(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector2D SegmentStart, const FVector2D SegmentEnd, const FLinearColor Color, const bool Persistent)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Segment2D(*LogCategoryPtr, WorldContextObject, SegmentStart, SegmentEnd, Color.ToFColor(true), Persistent);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawCircle2D(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector2D Location, const float Radius, const FLinearColor Color, const bool Persistent)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Circle2D(*LogCategoryPtr, WorldContextObject, Location, Radius, Color.ToFColor(true), Persistent);
    }
#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
void UImGuiDebugDrawLibrary::DebugDrawRect2D(const UObject* WorldContextObject, const FImGuiLogCategory LogCategory, const FVector2D Min, const FVector2D Max, const FLinearColor Color, const bool Persistent)
{
#if ENABLE_IMGUI
    if (const FLogCategoryBase* LogCategoryPtr = FImGuiDebugLog::GetLogCategoryBase(LogCategory))
    {
        FImGuiDebugDraw::Rect2D(*LogCategoryPtr, WorldContextObject, Min, Max, Color.ToFColor(true), Persistent);
    }
#endif //ENABLE_IMGUI
}

