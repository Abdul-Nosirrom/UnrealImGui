// Copyright 2023 CoC All rights reserved

#include "ImGuiDebugShape.h"
#include "ImGuiConfig.h"
#include "ImGuiDebug.h"
#include "ImGuiDebugDrawHelper.h"

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakePoint(const FVector& Location, const float Size, const FColor& Color, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(Location);

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Point;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Size;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawPoint(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 1)
    {
        const FVector Location = ShapeData[0];
        const float ServerThickness = Thickness;

        DrawDebugPoint(
            World,
            Location,
            ServerThickness,
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority));
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeSegment(const FVector& StartLocation, const FVector& EndLocation, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(StartLocation);
    NewElement.ShapeData.Add(EndLocation);

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Segment;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawSegment(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 2)
    {
        const FVector Start = ShapeData[0];
        const FVector End = ShapeData[1];
        const float ServerThickness = Thickness;

        DrawDebugLine(
            World,
            Start,
            End,
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            ServerThickness);
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeArrow(const FVector& StartLocation, const FVector& EndLocation, const float HeadSize, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(StartLocation);
    NewElement.ShapeData.Add(EndLocation);
    NewElement.ShapeData.Add(FVector(HeadSize, 0, 0));

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Arrow;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawArrow(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 3)
    {
        const FVector Start = ShapeData[0];
        const FVector End = ShapeData[1];
        const float Size = ShapeData[2].X;

        DrawDebugDirectionalArrow(
            World,
            Start,
            End,
            Size,
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            Thickness);
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeAxes(const FVector& Location, const FRotator& Rotation, const float HeadSize, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(Location);
    NewElement.ShapeData.Add(FVector(Rotation.Pitch, Rotation.Yaw, Rotation.Roll));
    NewElement.ShapeData.Add(FVector(HeadSize, 0, 0));

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Axes;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawAxes(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 3)
    {
        const FVector Location = ShapeData[0];
        const FRotator Rotation(ShapeData[1].X, ShapeData[1].Y, ShapeData[1].Z);
        const float Scale = ShapeData[2].X;

        DrawDebugCoordinateSystem(
            World,
            Location,
            Rotation,
            Scale,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            Thickness);
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeBox(const FVector& Center, const FRotator& Rotation, const FVector& Extent, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(Center);
    NewElement.ShapeData.Add(Extent);
    NewElement.ShapeData.Add(FVector(Rotation.Pitch, Rotation.Yaw, Rotation.Roll));

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Box;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawBox(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 3)
    {
        const FVector Location = ShapeData[0];
        const FVector Extent = ShapeData[1];
        const FRotator Rotation(ShapeData[2].X, ShapeData[2].Y, ShapeData[2].Z);

        DrawDebugBox(
            World, Location, Extent, FQuat(Rotation),
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            Thickness);
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeSolidBox(const FVector& Center, const FRotator& Rotation, const FVector& Extent, const FColor& Color, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(Center);
    NewElement.ShapeData.Add(FVector(Rotation.Pitch, Rotation.Yaw, Rotation.Roll));
    NewElement.ShapeData.Add(Extent);

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::SolidBox;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    return NewElement;
}


void FImGuiDebugShape::DrawSolidBox(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 12)
    {
        const FVector Location = ShapeData[0];
        const FVector Extent = ShapeData[1];
        const FRotator Rotation(ShapeData[2].X, ShapeData[2].Y, ShapeData[2].Z);

        DrawDebugSolidBox(
            World,
            Location,
            Extent,
            FQuat(Rotation),
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority));
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeCone(const FVector& Location, const FVector& Direction, const float Length, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(Location);
    NewElement.ShapeData.Add(Direction);
    NewElement.ShapeData.Add(FVector(Length, 0, 0));

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Cone;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawCone(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 3 && ShapeData[2].X > 0)
    {
        const FVector Location = ShapeData[0];
        const FVector Direction = ShapeData[1];
        const float Length = ShapeData[2].X;
        const float DefaultConeAngle = 0.25f; // ~ 15 degrees

        DrawDebugCone(
            World,
            Location,
            Direction,
            Length,
            DefaultConeAngle,
            DefaultConeAngle,
            FImGuiDebug::GetCircleSegments(),
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            Thickness);
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeCylinder(const FVector& Center, const float Radius, const float HalfHeight, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(Center);
    NewElement.ShapeData.Add(FVector(Radius, 0, HalfHeight));

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Cylinder;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}
//--------------------------------------------------------------------------------------------------------------------------

void FImGuiDebugShape::DrawCylinder(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 2)
    {
        DrawDebugCylinder(
            World,
            ShapeData[0] - FVector(0, 0, ShapeData[1].Z),
            ShapeData[0] + FVector(0, 0, ShapeData[1].Z),
            ShapeData[1].X,
            FImGuiDebug::GetCircleSegments(),
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            Thickness);
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeCircle(const FVector& Center, const FRotator& Rotation, const float Radius, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(Center);
    NewElement.ShapeData.Add(FVector(Rotation.Pitch, Rotation.Yaw, Rotation.Roll));
    NewElement.ShapeData.Add(FVector(Radius, 0, 0));

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Circle;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawCircle(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 3)
    {
        const FVector Location = ShapeData[0];
        const FRotator Rotation (ShapeData[1].X, ShapeData[1].Y, ShapeData[1].Z);
        const FRotationTranslationMatrix Matrix(Rotation, Location);
        const float Radius = ShapeData[2].X;

        DrawDebugCircle(
            World,
            Matrix,
            Radius,
            FImGuiDebug::GetCircleSegments(),
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            Thickness,
            false);

    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeCircleArc(const FVector& Center, const FRotator& Rotation, const float InnerRadius, const float OuterRadius, const float Angle, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(Center);
    NewElement.ShapeData.Add(FVector(Rotation.Pitch, Rotation.Yaw, Rotation.Roll));
    NewElement.ShapeData.Add(FVector(InnerRadius, OuterRadius, Angle));

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::CircleArc;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawCircleArc(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 3)
    {
        FImGuiDebugDrawHelper::DrawArc(
            World,
            FRotationTranslationMatrix(FRotator(ShapeData[1].X, ShapeData[1].Y, ShapeData[1].Z), ShapeData[0]),
            ShapeData[2].X,
            ShapeData[2].Y,
            ShapeData[2].Z,
            FImGuiDebug::GetDebugSegments(),
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            Thickness);
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeCapsule(const FVector& Center, const FQuat& Rotation, const float Radius, const float HalfHeight, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(Center);
    NewElement.ShapeData.Add(FVector(Radius, HalfHeight, 0));
    NewElement.ShapeData.Add(Rotation.Euler());

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Capsule;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawCapsule(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 3)
    {
        DrawDebugCapsule(
            World,
            ShapeData[0],
            ShapeData[1].Y,
            ShapeData[1].X,
            FQuat::MakeFromEuler(ShapeData[2]),
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            Thickness);
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeFlatCapsule(const FVector2D& Start, const FVector2D& End, const float Radius, const float Z, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(FVector(Start.X, Start.Y, 0));
    NewElement.ShapeData.Add(FVector(End.X, End.Y, 0));
    NewElement.ShapeData.Add(FVector(Radius, Z, 0));

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::FlatCapsule;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------

void FImGuiDebugShape::DrawFlatCapsule(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() > 0)
    {
        FImGuiDebugDrawHelper::DrawFlatCapsule(
            World,
            FVector2D(ShapeData[0].X, ShapeData[0].Y),
            FVector2D(ShapeData[1].X, ShapeData[1].Y),
            ShapeData[2].X,
            ShapeData[2].Y,
            FImGuiDebug::GetCircleSegments(),
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            Thickness);
    }

#endif //ENABLE_IMGUI
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakeBone(const FVector& BoneLocation, const FVector& ParentLocation, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;

    NewElement.ShapeData.Add(BoneLocation);
    NewElement.ShapeData.Add(ParentLocation);

    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Bone;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = Thickness;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawBone(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() == 2)
    {
        DrawDebugLine(
            World,
            ShapeData[0],
            ShapeData[1],
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority),
            Thickness);

        DrawDebugPoint(
            World,
            ShapeData[0],
            Thickness + 4.0f,
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority));
    }

#endif //ENABLE_IMGUI
}


//--------------------------------------------------------------------------------------------------------------------------
FImGuiDebugShape FImGuiDebugShape::MakePolygon(const TArray<FVector>& Verts, const FColor& Color, const bool bPersistent, const uint8 DepthPriority)
{
    FImGuiDebugShape NewElement;
    NewElement.ShapeData = Verts;
    NewElement.Color = Color;
    NewElement.Type = EImGuiDebugShape::Polygon;
    NewElement.bPersistent = bPersistent;
    NewElement.DepthPriority = DepthPriority;
    NewElement.Thickness = 0.0f;
    return NewElement;
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::DrawPolygon(const UWorld* World) const
{
#if ENABLE_IMGUI

    if (ShapeData.Num() > 0)
    {
        FVector MidPoint = FVector::ZeroVector;
        TArray<int32> Indices;
        for (int32 Idx = 0; Idx < ShapeData.Num(); Idx++)
        {
            Indices.Add(Idx);
            MidPoint += ShapeData[Idx];
        }

        DrawDebugMesh(
            World,
            ShapeData,
            Indices,
            Color,
            FImGuiDebug::GetDebugPersistent(bPersistent),
            FImGuiDebug::GetDebugDuration(bPersistent),
            FImGuiDebug::GetDebugDepthPriority(DepthPriority));
    }

#endif //ENABLE_IMGUI
}


//--------------------------------------------------------------------------------------------------------------------------
void FImGuiDebugShape::Draw(const UWorld* World) const
{
    switch (Type)
    {
    case EImGuiDebugShape::Arrow:          DrawArrow(World); break;
    case EImGuiDebugShape::Axes:           DrawAxes(World); break;
    case EImGuiDebugShape::Bone:           DrawBone(World); break;
    case EImGuiDebugShape::Box:            DrawBox(World); break;
    case EImGuiDebugShape::Capsule:        DrawCapsule(World); break;
    case EImGuiDebugShape::Circle:         DrawCircle(World); break;
    case EImGuiDebugShape::CircleArc:      DrawCircleArc(World); break;
    case EImGuiDebugShape::Cone:           DrawCone(World); break;
    case EImGuiDebugShape::Cylinder:       DrawCylinder(World); break;
    case EImGuiDebugShape::FlatCapsule:    DrawFlatCapsule(World); break;
    case EImGuiDebugShape::Point:          DrawPoint(World); break;
    case EImGuiDebugShape::Polygon:        DrawPolygon(World); break;
    case EImGuiDebugShape::Segment:        DrawSegment(World); break;
    default: break;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
FArchive& operator<<(FArchive& Ar, FImGuiDebugShape& Shape)
{
    Ar << Shape.ShapeData;
    Ar << Shape.Color;
    Ar << Shape.bPersistent;
    Ar << Shape.DepthPriority;
    Ar << Shape.Thickness;

    uint8 TypeNum = static_cast<uint8>(Shape.Type);
    Ar << TypeNum;
    Shape.Type = static_cast<EImGuiDebugShape>(TypeNum);

    return Ar;
}
