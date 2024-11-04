// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"


enum class EImGuiDebugShape : uint8
{
    Invalid,

    Arrow,
    Axes,
    Bone,
    Box,
    Capsule,
    Circle,
    CircleArc,
    Cone,
    Cylinder,
    FlatCapsule,
    Point,
    Polygon,
    Segment,
    SolidBox
};

struct IMGUIDEBUG_API FImGuiDebugShape
{
    EImGuiDebugShape Type = EImGuiDebugShape::Invalid;
    TArray<FVector> ShapeData;
    FColor Color;
    bool bPersistent = false;
    float Thickness = 0.0f;
    uint8 DepthPriority = 0;

    FImGuiDebugShape() {}

    bool operator==(const FImGuiDebugShape& Other) const
    {
        return (Type == Other.Type)
            && (Color == Other.Color)
            && (ShapeData == Other.ShapeData)
            && (bPersistent == Other.bPersistent)
            && (Thickness == Other.Thickness)
            && (DepthPriority == Other.DepthPriority);
    }

    static FImGuiDebugShape MakePoint(const FVector& Location, const float Size, const FColor& Color, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeSegment(const FVector& StartLocation, const FVector& EndLocation, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeBone(const FVector& BoneLocation, const FVector& ParentLocation, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeArrow(const FVector& StartLocation, const FVector& EndLocation, const float HeadSize, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeAxes(const FVector& Location, const FRotator& Rotation, const float HeadSize, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeBox(const FVector& Center, const FRotator& Rotation, const FVector& Extent, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeSolidBox(const FVector& Center, const FRotator& Rotation, const FVector& Extent, const FColor& Color, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeCone(const FVector& Location, const FVector& Direction, const float Length, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeCylinder(const FVector& Center, const float Radius, const float HalfHeight, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeCircle(const FVector& Center, const FRotator& Rotation, const float Radius, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeCircleArc(const FVector& Center, const FRotator& Rotation, const float InnerRadius, const float OuterRadius, const float Angle, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeCapsule(const FVector& Center, const FQuat& Rotation, const float Radius, const float HalfHeight, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakeFlatCapsule(const FVector2D& Start, const FVector2D& End, const float Radius, const float Z, const FColor& Color, const float Thickness, const bool bPersistent, const uint8 DepthPriority);
    static FImGuiDebugShape MakePolygon(const TArray<FVector>& Verts, const FColor& Color, const bool bPersistent, const uint8 DepthPriority);

    void DrawPoint(const UWorld* World) const;
    void DrawSegment(const UWorld* World) const;
    void DrawBone(const UWorld* World) const;
    void DrawArrow(const UWorld* World) const;
    void DrawAxes(const UWorld* World) const;
    void DrawBox(const UWorld* World) const;
    void DrawSolidBox(const UWorld* World) const;
    void DrawCone(const UWorld* World) const;
    void DrawCylinder(const UWorld* World) const;
    void DrawCircle(const UWorld* World) const;
    void DrawCircleArc(const UWorld* World) const;
    void DrawCapsule(const UWorld* World) const;
    void DrawFlatCapsule(const UWorld* World) const;
    void DrawPolygon(const UWorld* World) const;

    void Draw(const UWorld* World) const;
};

FArchive& operator<<(FArchive& Ar, FImGuiDebugShape& Shape);
