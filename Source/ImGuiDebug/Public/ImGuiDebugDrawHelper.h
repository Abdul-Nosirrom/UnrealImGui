﻿// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"

class AActor;
class UPrimitiveComponent;
class UWorld;
namespace EDrawDebugTrace { enum Type : int; }
struct FCollisionShape;
struct FHitResult;
struct FOverlapResult;

struct FImGuiDebugDrawOverlapParams
{
    FColor HitColor = FColor::Green;
    FColor NoHitColor = FColor::Red;
    bool DrawHitPrimitives = true;
    bool DrawHitPrimitiveActorsName = false;
    bool HitPrimitiveActorsNameShadow = true;
    float HitPrimitiveActorsNameSize = 1.0f;
    bool Persistent = false;
    float LifeTime = 0.0f;
    uint8 DepthPriority = 0;
    float Thickness = 0.0f;
    FColor ChannelColors[ECC_MAX];
};

struct FImGuiDebugDrawLineTraceParams : public FImGuiDebugDrawOverlapParams
{
    bool DrawHitLocation = true;
    bool DrawHitImpactPoints = true;
    bool DrawHitNormals = true;
    bool DrawHitImpactNormals = true;
    float HitPointSize = 5.0f;
    FColor NormalColor = FColor::Yellow;
    FColor ImpactNormalColor = FColor::Cyan;
};

struct FImGuiDebugDrawSweepParams : public FImGuiDebugDrawLineTraceParams
{
    bool DrawHitShapes = true;
};

class IMGUIDEBUG_API FImGuiDebugDrawHelper
{
public:

    static void DrawArc(const UWorld* World, const FMatrix& Matrix, const float InnerRadius, const float OuterRadius, const float AngleStart, const float AngleEnd, const int32 Segments, const FColor& Color, const bool bPersistentLines = false, const float LifeTime = -1.f, const uint8 DepthPriority = 0U, const float Thickness = 0.0f);

    static void DrawArc(const UWorld* World, const FMatrix& Matrix, const float InnerRadius, const float OuterRadius, const float ArcAngle, const int32 Segments, const FColor& Color, const bool bPersistentLines = false, const float LifeTime = -1.f, const uint8 DepthPriority = 0U, const float Thickness = 0.0f);

    static void DrawSphere(const UWorld* World, const FVector& Center, const float Radius, const int32 Segments, const FColor& Color, const bool bPersistentLines = false, const float LifeTime = -1.f, const uint8 DepthPriority = 0U, const float Thickness = 0.0f);

    static void DrawFrustum(const UWorld* World, const FMatrix& Matrix, const float Angle, const float AspectRatio, const float NearPlane, const float FarPlane, const FColor& Color, const bool bPersistentLines = false, const float LifeTime = -1.f, const uint8 DepthPriority = 0U, const float Thickness = 0.0f);

    static void DrawFlatCapsule(const UWorld* World, const FVector2D& Start, const FVector2D& End, const float Radius, const float Z, const float Segments, const FColor& Color, const bool bPersistentLines = false, const float LifeTime = -1.0f, const uint8 DepthPriority = 0, const float Thickness = 0.0f);

    static void DrawQuad(const UWorld* World, const FVector& Position, const FQuat& Rotation, const FVector2D& Extents, const FColor& Color, bool bPersistent = false, float LifeTime = -1, uint8 DepthPriority = 0, const float Thickness = 0.0f);

    static void DrawSolidQuad(const UWorld* World, const FVector& Position, const FQuat& Rotation, const FVector2D& Extents, const FColor& Color, bool bPersistent = false, float LifeTime = -1, uint8 DepthPriority = 0);

    static void DrawShape(const UWorld* World, const FCollisionShape& InShape, const FVector& Location, const FQuat& Rotation, const FVector& Scale, const FColor& Color, const bool Persistent, const float LifeTime, const uint8 DepthPriority, const float Thickness);

    static void DrawPrimitiveComponent(const UPrimitiveComponent& PrimitiveComponent, const FColor& Color, const bool Persistent, const float LifeTime, const uint8 DepthPriority, const float Thickness);

    static void DrawOverlap(const UWorld* World, const FCollisionShape& Shape, const FVector& Location, const FQuat& Rotation, TArray<FOverlapResult>& OverlapResults, const FImGuiDebugDrawOverlapParams& Settings);

    static void DrawHitResult(const UWorld* World, const FHitResult& HitResult, const FImGuiDebugDrawLineTraceParams& Settings);

    static void DrawHitResults(const UWorld* World, const TArray<FHitResult>& HitResults, const FImGuiDebugDrawLineTraceParams& Settings);

    static void DrawLineTrace(const UWorld* World, const FVector& Start, const FVector& End, const bool HasHits, TArray<FHitResult>& HitResults, const FImGuiDebugDrawLineTraceParams& Settings);

    static void DrawSweep(const UWorld* World, const FCollisionShape& Shape, const FVector& Start, const FVector& End, const FQuat& Rotation, const bool HasHits, TArray<FHitResult>& HitResults, const FImGuiDebugDrawSweepParams& Settings);
};