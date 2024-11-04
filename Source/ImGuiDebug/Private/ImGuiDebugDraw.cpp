// Copyright 2023 CoC All rights reserved


#include "ImGuiDebugDraw.h"

#include "ImGuiDebug.h"
#include "ImGuiDebugDrawHelper.h"
#include "ImGuiDebugLog.h"
#include "ImGuiHelper.h"
#include "imgui_internal.h"

#if ENABLE_IMGUI

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::String2D(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FString& Text, const FVector2D& Location, const FColor& Color, const bool Persistent)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    FImGuiDebugDraw::AddText(
        FImGuiHelper::ToImVec2(Location),
        Text,
        FImGuiHelper::ToImU32(Color),
        true,
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::Settings.Fade2D);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Segment2D(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector2D& SegmentStart, const FVector2D& SegmentEnd, const FColor& Color, const bool Persistent)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    FImGuiDebugDraw::AddLine(
        FImGuiHelper::ToImVec2(SegmentStart),
        FImGuiHelper::ToImVec2(SegmentEnd),
        FImGuiHelper::ToImU32(Color),
        FImGuiDebug::GetDebugThickness(0),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::Settings.Fade2D);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Circle2D(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector2D& Location, const float Radius, const FColor& Color, const bool Persistent)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    FImGuiDebugDraw::AddCircle(
        FImGuiHelper::ToImVec2(Location),
        Radius,
        FImGuiHelper::ToImU32(Color),
        FImGuiDebug::GetDebugSegments(),
        FImGuiDebug::GetDebugThickness(0),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::Settings.Fade2D);
}

//--------------------------------------------------------------------------------------------------------------------------

void FImGuiDebugDraw::Rect2D(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector2D& Min, const FVector2D& Max, const FColor& Color, const bool Persistent)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    FImGuiDebugDraw::AddRect(
        FImGuiHelper::ToImVec2(Min),
        FImGuiHelper::ToImVec2(Max),
        FImGuiHelper::ToImU32(Color),
        0.0f,
        FImGuiDebug::GetDebugThickness(0),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::Settings.Fade2D);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::String(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FString& Text, const FVector& Location, const FColor& Color, const bool Persistent)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    UE_VLOG_LOCATION(WorldContextObject, LogCategory, Verbose, Location, 10.0f, NewColor, TEXT("%s"), *Text);

    ::DrawDebugString(
        World,
        Location,
        *Text,
        nullptr,
        NewColor,
        FImGuiDebug::GetDebugTextDuration(Persistent),
        FImGuiDebug::Settings.TextShadow,
        FImGuiDebug::Settings.TextSize);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Point(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& Location, const float Size, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    ::DrawDebugPoint(
        World,
        Location,
        Size,
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Segment(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& SegmentStart, const FVector& SegmentEnd, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    UE_VLOG_SEGMENT(WorldContextObject, LogCategory, Verbose, SegmentStart, SegmentEnd, NewColor, TEXT_EMPTY);

    ::DrawDebugLine(
        World,
        SegmentStart,
        SegmentEnd,
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Bone(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& BoneLocation, const FVector& ParentLocation, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    UE_VLOG_SEGMENT(WorldContextObject, LogCategory, Verbose, BoneLocation, ParentLocation, NewColor, TEXT_EMPTY);

    ::DrawDebugLine(
        World,
        BoneLocation,
        ParentLocation,
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0));

    ::DrawDebugPoint(
        World,
        BoneLocation,
        FImGuiDebug::GetDebugThickness(4.0f),
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Arrow(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& SegmentStart, const FVector& SegmentEnd, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    UE_VLOG_ARROW(WorldContextObject, LogCategory, Verbose, SegmentStart, SegmentEnd, NewColor, TEXT_EMPTY);

    ::DrawDebugDirectionalArrow(
        World,
        SegmentStart,
        SegmentEnd,
        FImGuiDebug::Settings.ArrowSize,
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Axis(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& AxisLoc, const FRotator& AxisRot, const float Scale, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FRotationMatrix R(AxisRot);
    UE_VLOG_ARROW(WorldContextObject, LogCategory, Verbose, AxisLoc, AxisLoc + R.GetScaledAxis(EAxis::X) * Scale, FColor::Red, TEXT_EMPTY);
    UE_VLOG_ARROW(WorldContextObject, LogCategory, Verbose, AxisLoc, AxisLoc + R.GetScaledAxis(EAxis::Y) * Scale, FColor::Green, TEXT_EMPTY);
    UE_VLOG_ARROW(WorldContextObject, LogCategory, Verbose, AxisLoc, AxisLoc + R.GetScaledAxis(EAxis::Z) * Scale, FColor::Blue, TEXT_EMPTY);

    ::DrawDebugCoordinateSystem(
        World,
        AxisLoc,
        AxisRot,
        Scale * FImGuiDebug::Settings.AxesScale,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Circle(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FMatrix& Matrix, float Radius, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    const FVector Center = Matrix.GetOrigin();
    const FVector UpVector = Matrix.GetUnitAxis(EAxis::X);
    UE_VLOG_CIRCLE(WorldContextObject, LogCategory, Verbose, Center, UpVector, Radius, NewColor, TEXT_EMPTY);

    ::DrawDebugCircle(
        World,
        Matrix,
        Radius,
        FImGuiDebug::GetCircleSegments(),
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0),
        false);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::CircleArc(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FMatrix& Matrix, const float InnerRadius, const float OuterRadius, const float Angle, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);

    //TODO : Add VLOG

    FImGuiDebugDrawHelper::DrawArc(
        World,
        Matrix,
        InnerRadius,
        OuterRadius,
        Angle,
        FImGuiDebug::GetCircleSegments(),
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::FlatCapsule(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector2D& Start, const FVector2D& End, const float Radius, const float Z, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    // TODO : Add VLOG

    FImGuiDebugDrawHelper::DrawFlatCapsule(
        World,
        Start,
        End,
        Radius,
        Z,
        FImGuiDebug::GetCircleSegments(),
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Sphere(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& Location, const float Radius, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    UE_VLOG_CAPSULE(WorldContextObject, LogCategory, Verbose, Location, 0.0f, Radius, FQuat::Identity, NewColor, TEXT_EMPTY);

    FImGuiDebugDrawHelper::DrawSphere(
        World,
        Location,
        Radius,
        FImGuiDebug::GetDebugSegments(),
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Box(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& Center, const FVector& Extent, const FQuat& Rotation, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    UE_VLOG_OBOX(WorldContextObject, LogCategory, Verbose, FBox(-Extent, Extent), FQuatRotationTranslationMatrix::Make(Rotation, Center), NewColor, TEXT_EMPTY);

    ::DrawDebugBox(
        World,
        Center,
        Extent,
        Rotation,
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::SolidBox(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& Center, const FVector& Extent, const FQuat& Rotation, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    UE_VLOG_OBOX(WorldContextObject, LogCategory, Verbose, FBox(-Extent, Extent), FQuatRotationTranslationMatrix::Make(Rotation, Center), NewColor, TEXT_EMPTY);

    // If we make the Box Thick enough, it will be displayed as a filled box.
    // We don't use "DrawDebugSolidBox" because it produced weird result, with color being darker than what is intended
    const float NeededThickness = FMath::Min3(Extent.X, Extent.Y, Extent.Z) * 10.f;

    ::DrawDebugBox(
        World,
        Center,
        Extent,
        Rotation,
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        NeededThickness);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Frustum(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FMatrix& Matrix, const float Angle, const float AspectRatio, const float NearPlane, const float FarPlane, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);

    FImGuiDebugDrawHelper::DrawFrustum(
        World,
        Matrix,
        Angle,
        AspectRatio,
        NearPlane,
        FarPlane,
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Capsule(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& Center, const float HalfHeight, const float Radius, const FQuat& Rotation, const FColor& Color, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    const FColor NewColor = FImGuiDebug::ModulateDebugColor(World, Color, Persistent);
    UE_VLOG_CAPSULE(WorldContextObject, LogCategory, Verbose, Center, HalfHeight, Radius, FQuat::Identity, NewColor, TEXT_EMPTY);

    DrawDebugCapsule(
        World,
        Center,
        HalfHeight,
        Radius,
        Rotation,
        NewColor,
        FImGuiDebug::GetDebugPersistent(Persistent),
        FImGuiDebug::GetDebugDuration(Persistent),
        FImGuiDebug::GetDebugDepthPriority(DepthPriority),
        FImGuiDebug::GetDebugThickness(0));
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Points(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const TArray<FVector>& Points, const float Radius, const FColor& StartColor, const FColor& EndColor, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory))
    {
        int32 Index = 0;
        for (const FVector& Point : Points)
        {
            const FLinearColor Color = FLinearColor::LerpUsingHSV(FLinearColor(StartColor), FLinearColor(EndColor), Points.Num() <= 1 ? 0.0f : Index / (float)(Points.Num() - 1));
            Sphere(LogCategory, WorldContextObject, Point, Radius, Color.ToFColor(true), Persistent, DepthPriority);
            Index++;
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Path(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const TArray<FVector>& Points, const float PointSize, const FColor& StartColor, const FColor& EndColor, const bool Persistent, const uint8 DepthPriority)
{
    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }

    if (Points.Num() == 0)
    {
        return;
    }

    FVector LastPoint = Points[0];

    int32 Index = 0;
    for (const FVector& Position : Points)
    {
        const FLinearColor LinearColor = FLinearColor::LerpUsingHSV(FLinearColor(StartColor), FLinearColor(EndColor), Points.Num() <= 1 ? 0.0f : Index / (float)(Points.Num() - 1));
        FColor Color = LinearColor.ToFColor(true);

        Point(LogCategory, WorldContextObject, Position, PointSize, Color, Persistent, DepthPriority);

        if (Index > 0)
        {
            Segment(LogCategory, WorldContextObject, LastPoint, Position, Color, Persistent, DepthPriority);
        }

        Index++;
        LastPoint = Position;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Skeleton(const FLogCategoryBase& LogCategory, const USkeletalMeshComponent* Skeleton, const FColor& Color, const bool DrawSecondaryBones, const uint8 DepthPriority)
{
    if (Skeleton == nullptr)
    {
        return;
    }

    if (FImGuiDebugLog::IsLogCategoryActive(LogCategory) == false)
    {
        return;
    }

    const FReferenceSkeleton& ReferenceSkeleton = Skeleton->GetSkeletalMeshAsset()->GetRefSkeleton();
    const FTransform WorldTransform = Skeleton->GetComponentTransform();
    const TArray<FTransform>& ComponentSpaceTransforms = Skeleton->GetComponentSpaceTransforms();

    for (int32 BoneIndex = 0; BoneIndex < ComponentSpaceTransforms.Num(); ++BoneIndex)
    {
        if (DrawSecondaryBones == false)
        {
            const FName BoneName = ReferenceSkeleton.GetBoneName(BoneIndex);
            if (FImGuiDebug::IsSecondarySkeletonBone(BoneName))
            {
                continue;
            }
        }

        const FTransform Transform = ComponentSpaceTransforms[BoneIndex] * WorldTransform;
        const FVector BoneLocation = Transform.GetLocation();
        const FRotator BoneRotation = FRotator(Transform.GetRotation());
        const int32 ParentIndex = ReferenceSkeleton.GetParentIndex(BoneIndex);

        FVector ParentLocation;
        if (ParentIndex >= 0)
        {
            ParentLocation = (ComponentSpaceTransforms[ParentIndex] * WorldTransform).GetLocation();
        }
        else
        {
            ParentLocation = WorldTransform.GetLocation();
        }

        Bone(LogCategory, Skeleton->GetOwner(), BoneLocation, ParentLocation, Color, false, DepthPriority);
    }
}

#endif //ENABLE_IMGUI


//--------------------------------------------------------------------------------------------------------------------------
TArray<FImGuiDebugDraw::FLine>       FImGuiDebugDraw::Lines;
TArray<FImGuiDebugDraw::FTriangle>   FImGuiDebugDraw::Triangles;
TArray<FImGuiDebugDraw::FTriangle>   FImGuiDebugDraw::TrianglesFilled;
TArray<FImGuiDebugDraw::FRectangle>  FImGuiDebugDraw::Rectangles;
TArray<FImGuiDebugDraw::FRectangle>  FImGuiDebugDraw::RectanglesFilled;
TArray<FImGuiDebugDraw::FQuad>       FImGuiDebugDraw::Quads;
TArray<FImGuiDebugDraw::FQuad>       FImGuiDebugDraw::QuadsFilled;
TArray<FImGuiDebugDraw::FCircle>     FImGuiDebugDraw::Circles;
TArray<FImGuiDebugDraw::FCircle>     FImGuiDebugDraw::CirclesFilled;
TArray<FImGuiDebugDraw::FText>       FImGuiDebugDraw::Texts;

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::AddLine(const ImVec2& P1, const ImVec2& P2, ImU32 Color, float Thickness /*= 1.0f*/, float Duration /*= 0.0f*/, bool FadeColor /*= false*/)
{
    FLine Line;
    Line.P1 = P1;
    Line.P2 = P2;
    Line.Color = Color;
    Line.Thickness = Thickness;
    Line.Time = ImGui::GetCurrentContext()->Time;
    Line.Duration = Duration;
    Line.FadeColor = FadeColor;
    Lines.Add_GetRef(Line);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::AddRect(const ImVec2& Min, const ImVec2& Max, ImU32 Color, float Rounding /*= 0.0f*/, float Thickness /*= 1.0f*/, float Duration /*= 0.0f*/, bool FadeColor /*= false*/)
{
    FRectangle Rectangle;
    Rectangle.Min = Min;
    Rectangle.Max = Max;
    Rectangle.Color = Color;
    Rectangle.Rounding = Rounding;
    Rectangle.Thickness = Thickness;
    Rectangle.Time = ImGui::GetCurrentContext()->Time;
    Rectangle.Duration = Duration;
    Rectangle.FadeColor = FadeColor;
    Rectangles.Add_GetRef(Rectangle);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::AddRectFilled(const ImVec2& Min, const ImVec2& Max, ImU32 Color, float Rounding /*= 0.0f*/, float Duration /*= 0.0f*/, bool FadeColor /*= false*/)
{
    FRectangle Rectangle;
    Rectangle.Min = Min;
    Rectangle.Max = Max;
    Rectangle.Color = Color;
    Rectangle.Rounding = Rounding;
    Rectangle.Thickness = 0.0f;
    Rectangle.Time = ImGui::GetCurrentContext()->Time;
    Rectangle.Duration = Duration;
    Rectangle.FadeColor = FadeColor;
    RectanglesFilled.Add_GetRef(Rectangle);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::AddQuad(const ImVec2& P1, const ImVec2& P2, const ImVec2& P3, const ImVec2& P4, ImU32 Color, float Thickness/* = 1.0f*/, float Duration /*= 0.0f*/, bool FadeColor /*= false*/)
{
    FQuad Quad;
    Quad.P1 = P1;
    Quad.P2 = P2;
    Quad.P3 = P3;
    Quad.P4 = P4;
    Quad.Color = Color;
    Quad.Thickness = Thickness;
    Quad.Time = ImGui::GetCurrentContext()->Time;
    Quad.Duration = Duration;
    Quad.FadeColor = FadeColor;
    Quads.Add_GetRef(Quad);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::AddQuadFilled(const ImVec2& P1, const ImVec2& P2, const ImVec2& P3, const ImVec2& P4, ImU32 Color, float Duration /*= 0.0f*/, bool FadeColor /*= false*/)
{
    FQuad Quad;
    Quad.P1 = P1;
    Quad.P2 = P2;
    Quad.P3 = P3;
    Quad.P4 = P4;
    Quad.Color = Color;
    Quad.Thickness = 0.0f;
    Quad.Time = ImGui::GetCurrentContext()->Time;
    Quad.Duration = Duration;
    Quad.FadeColor = FadeColor;
    QuadsFilled.Add_GetRef(Quad);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::AddTriangle(const ImVec2& P1, const ImVec2& P2, const ImVec2& P3, ImU32 Color, float Thickness/* = 1.0f*/, float Duration /*= 0.0f*/, bool FadeColor /*= false*/)
{
    FTriangle Triangle;
    Triangle.P1 = P1;
    Triangle.P2 = P2;
    Triangle.P3 = P3;
    Triangle.Color = Color;
    Triangle.Thickness = Thickness;
    Triangle.Time = ImGui::GetCurrentContext()->Time;
    Triangle.Duration = Duration;
    Triangle.FadeColor = FadeColor;
    Triangles.Add_GetRef(Triangle);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::AddTriangleFilled(const ImVec2& P1, const ImVec2& P2, const ImVec2& P3, ImU32 Color, float Duration /*= 0.0f*/, bool FadeColor /*= false*/)
{
    FTriangle Triangle;
    Triangle.P1 = P1;
    Triangle.P2 = P2;
    Triangle.P3 = P3;
    Triangle.Color = Color;
    Triangle.Thickness = 0.0f;
    Triangle.Time = ImGui::GetCurrentContext()->Time;
    Triangle.Duration = Duration;
    Triangle.FadeColor = FadeColor;
    TrianglesFilled.Add_GetRef(Triangle);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::AddCircle(const ImVec2& Center, float Radius, ImU32 Color, int Segments /*= 0*/, float Thickness /*= 1.0f*/, float Duration /*= 0.0f*/, bool FadeColor /*= false*/)
{
    FCircle Circle;
    Circle.Center = Center;
    Circle.Radius = Radius > 0.0f ? Radius : 1.0f;
    Circle.Color = Color;
    Circle.Segments = Segments;
    Circle.Thickness = Thickness;
    Circle.Time = ImGui::GetCurrentContext()->Time;
    Circle.Duration = Duration;
    Circle.FadeColor = FadeColor;
    Circles.Add_GetRef(Circle);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::AddCircleFilled(const ImVec2& Center, float Radius, ImU32 Color, int Segments /*= 0*/, float Duration /*= 0.0f*/, bool FadeColor /*= false*/)
{
    FCircle Circle;
    Circle.Center = Center;
    Circle.Radius = Radius > 0.0f ? Radius : 1.0f;
    Circle.Color = Color;
    Circle.Segments = Segments;
    Circle.Thickness = 0.0f;
    Circle.Time = ImGui::GetCurrentContext()->Time;
    Circle.Duration = Duration;
    Circle.FadeColor = FadeColor;
    CirclesFilled.Add_GetRef(Circle);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::AddText(const ImVec2& Pos, const FString& Text, ImU32 Color, bool AddShadow /*= false*/, float Duration/* = 0.0f*/, bool FadeColor /*= false*/)
{
    FText TextElement;
    TextElement.Pos = Pos;
    TextElement.Text = Text;
    TextElement.Color = Color;
    TextElement.Time = ImGui::GetCurrentContext()->Time;
    TextElement.Duration = Duration;
    TextElement.FadeColor = FadeColor;
    Texts.Add_GetRef(TextElement);

    if (AddShadow)
    {
        FText ShadowTextElement;
        ShadowTextElement.Pos = Pos + ImVec2(1, 1);
        ShadowTextElement.Text = Text;
        const float Alpha = ImGui::ColorConvertU32ToFloat4(Color).w; // Keep original Alpha and set to black
        ShadowTextElement.Color = ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, Alpha));
        ShadowTextElement.Time = ImGui::GetCurrentContext()->Time;
        ShadowTextElement.Duration = Duration;
        ShadowTextElement.FadeColor = FadeColor;
        Texts.Add_GetRef(ShadowTextElement);
    }
}


//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugDraw::Draw()
{
    ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
    double Time = ImGui::GetCurrentContext()->Time;

    DrawShapes(Lines, [DrawList](const FLine& Line, const ImColor Color) { DrawList->AddLine(Line.P1, Line.P2, Color, Line.Thickness); });
    DrawShapes(Rectangles, [DrawList](const FRectangle& Rectangle, const ImColor Color) { DrawList->AddRect(Rectangle.Min, Rectangle.Max, Color, Rectangle.Rounding, Rectangle.Thickness); });
    DrawShapes(RectanglesFilled, [DrawList](const FRectangle& Rectangle, const ImColor Color) { DrawList->AddRectFilled(Rectangle.Min, Rectangle.Max, Color, Rectangle.Rounding); });
    DrawShapes(Quads, [DrawList](const FQuad& Quad, const ImColor Color) { DrawList->AddQuad(Quad.P1, Quad.P2, Quad.P3, Quad.P4, Color, Quad.Thickness); });
    DrawShapes(QuadsFilled, [DrawList](const FQuad& Quad, const ImColor Color) { DrawList->AddQuadFilled(Quad.P1, Quad.P2, Quad.P3, Quad.P4, Color); });
    DrawShapes(Triangles, [DrawList](const FTriangle& Triangle, const ImColor Color) { DrawList->AddTriangle(Triangle.P1, Triangle.P2, Triangle.P3, Color, Triangle.Thickness); });
    DrawShapes(TrianglesFilled, [DrawList](const FTriangle& Triangle, const ImColor Color) { DrawList->AddTriangleFilled(Triangle.P1, Triangle.P2, Triangle.P3, Color); });
    DrawShapes(Circles, [DrawList](const FCircle& Circle, const ImColor Color) { DrawList->AddCircle(Circle.Center, Circle.Radius, Color, Circle.Segments, Circle.Thickness); });
    DrawShapes(CirclesFilled, [DrawList](const FCircle& Circle, const ImColor Color) { DrawList->AddCircleFilled(Circle.Center, Circle.Radius, Color, Circle.Segments); });
    DrawShapes(Texts, [DrawList](const FText& Text, const ImColor Color) { DrawList->AddText(Text.Pos, Color, TCHAR_TO_ANSI(*Text.Text)); });
}
