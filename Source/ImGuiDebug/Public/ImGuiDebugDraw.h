// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ImGuiConfig.h"
#include "imgui.h"

class USkeletalMeshComponent;
struct FImGuiDebugShape;

#if ENABLE_IMGUI

struct IMGUIDEBUG_API FImGuiDebugDraw
{

public:
    static void String2D(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FString& Text, const FVector2D& Location, const FColor& Color, bool Persistent);

    static void Segment2D(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector2D& SegmentStart, const FVector2D& SegmentEnd, const FColor& Color, bool Persistent);

    static void Circle2D(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector2D& Location, float Radius, const FColor& Color, const bool Persistent);

    static void Rect2D(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector2D& Min, const FVector2D& Max, const FColor& Color, const bool Persistent);

    static void String(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FString& Text, const FVector& Location, const FColor& Color, const bool Persistent);

    static void Point(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& Location, float Size, const FColor& Color, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Segment(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& SegmentStart, const FVector& SegmentEnd, const FColor& Color, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Bone(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& BoneLocation, const FVector& ParentLocation, const FColor& Color, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Arrow(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& SegmentStart, const FVector& SegmentEnd, const FColor& Color, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Axis(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& AxisLoc, const FRotator& AxisRot, float Scale, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Circle(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FMatrix& Matrix, float Radius, const FColor& Color, const bool Persistent, const uint8 DepthPriority = 0U);

    static void CircleArc(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FMatrix& Matrix, float InnerRadius, float OuterRadius, float Angle, const FColor& Color, bool Persistent, const uint8 DepthPriority = 0U);

    static void FlatCapsule(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector2D& Start, const FVector2D& End, const float Radius, const float Z, const FColor& Color, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Sphere(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& Location, float Radius, const FColor& Color, bool Persistent, const uint8 DepthPriority = 0U);

    static void Box(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& Center, const FVector& Extent, const FQuat& Rotation, const FColor& Color, const bool Persistent, const uint8 DepthPriority = 0U);

    static void SolidBox(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& Center, const FVector& Extent, const FQuat& Rotation, const FColor& Color, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Capsule(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FVector& Center, const float HalfHeight, const float Radius, const FQuat& Rotation, const FColor& Color, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Points(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const TArray<FVector>& Points, float Radius, const FColor& StartColor, const FColor& EndColor, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Path(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const TArray<FVector>& Points, float PointSize, const FColor& StartColor, const FColor& EndColor, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Frustum(const FLogCategoryBase& LogCategory, const UObject* WorldContextObject, const FMatrix& Matrix, const float Angle, const float AspectRatio, const float NearPlane, const float FarPlane, const FColor& Color, const bool Persistent, const uint8 DepthPriority = 0U);

    static void Skeleton(const FLogCategoryBase& LogCategory, const USkeletalMeshComponent* Skeleton, const FColor& Color, bool DrawSecondaryBones = false, const uint8 DepthPriority = 1);

public:
    static void AddLine(const ImVec2& P1, const ImVec2& P2, ImU32 Color, float Thickness = 1.0f, float Duration = 0.0f, bool FadeColor = false);
    static void AddRect(const ImVec2& Min, const ImVec2& Max, ImU32 Color, float Rounding = 0.0f, float Thickness = 1.0f, float Duration = 0.0f, bool FadeColor = false);
    static void AddRectFilled(const ImVec2& Min, const ImVec2& Max, ImU32 Color, float Rounding = 0.0f, float Duration = 0.0f, bool FadeColor = false);
    static void AddQuad(const ImVec2& P1, const ImVec2& P2, const ImVec2& P3, const ImVec2& P4, ImU32 Color, float Thickness = 1.0f, float Duration = 0.0f, bool FadeColor = false);
    static void AddQuadFilled(const ImVec2& P1, const ImVec2& P2, const ImVec2& P3, const ImVec2& P4, ImU32 Color, float Duration = 0.0f, bool FadeColor = false);
    static void AddTriangle(const ImVec2& P1, const ImVec2& P2, const ImVec2& P3, ImU32 Color, float Thickness = 1.0f, float Duration = 0.0f, bool FadeColor = false);
    static void AddTriangleFilled(const ImVec2& P1, const ImVec2& P2, const ImVec2& P3, ImU32 Color, float Duration = 0.0f, bool FadeColor = false);
    static void AddCircle(const ImVec2& Center, float Radius, ImU32 Color, int Segments = 0, float Thickness = 1.0f, float Duration = 0.0f, bool FadeColor = false);
    static void AddCircleFilled(const ImVec2& Center, float Radius, ImU32 Color, int Segments = 0, float Duration = 0.0f, bool FadeColor = false);
    static void AddText(const ImVec2& Pos, const FString& Text, ImU32 Color, bool AddShadow = false, float Duration = 0.0f, bool FadeColor = false);

    static void Draw();

private:
    struct FShape
    {
        ImU32 Color = 0;
        float Duration = 0.0f;
        double Time = 0.0f;
        bool FadeColor = false;
    };

    struct FLine : FShape
    {
        ImVec2 P1 = ImVec2(0, 0);
        ImVec2 P2 = ImVec2(0, 0);
        float Thickness = 0.0f;
    };

    struct FRectangle : FShape
    {
        ImVec2 Min = ImVec2(0, 0);
        ImVec2 Max = ImVec2(0, 0);
        float Rounding = 0.0f;
        float Thickness = 0.0f;
    };

    struct FQuad : FShape
    {
        ImVec2 P1 = ImVec2(0, 0);
        ImVec2 P2 = ImVec2(0, 0);
        ImVec2 P3 = ImVec2(0, 0);
        ImVec2 P4 = ImVec2(0, 0);
        float Thickness = 0.0f;
    };

    struct FTriangle : FShape
    {
        ImVec2 P1 = ImVec2(0, 0);
        ImVec2 P2 = ImVec2(0, 0);
        ImVec2 P3 = ImVec2(0, 0);
        float Thickness = 0.0f;
    };

    struct FCircle : FShape
    {
        ImVec2 Center = ImVec2(0, 0);
        float Radius = 0.0f;
        int Segments = 12;
        float Thickness = 0.0f;
    };

    struct FText : FShape
    {
        ImVec2 Pos = ImVec2(0, 0);
        FString Text;
        ImU32 Color = 0;
    };

    //----------------------------------------------------------------------------------------------------------------------
    static TArray<FLine> Lines;
    static TArray<FTriangle> Triangles;
    static TArray<FTriangle> TrianglesFilled;
    static TArray<FRectangle> Rectangles;
    static TArray<FRectangle> RectanglesFilled;
    static TArray<FQuad> Quads;
    static TArray<FQuad> QuadsFilled;
    static TArray<FCircle> Circles;
    static TArray<FCircle> CirclesFilled;
    static TArray<FText> Texts;

    //----------------------------------------------------------------------------------------------------------------------
    template<typename TShape, typename TDrawFunction>
    static void DrawShapes(TArray<TShape>& Shapes, TDrawFunction DrawFunction)
    {
        ImDrawList* ImDrawList = ImGui::GetBackgroundDrawList();
        const double Time = ImGui::GetCurrentContext()->Time;

        for (int32 i = 0; i < Shapes.Num(); i++)
        {
            const TShape& Shape = Shapes[i];
            const double ElapsedTime = Time - Shape.Time;
            const float Fade = Shape.FadeColor && Shape.Duration > 0.0f ? 1.0f - (ElapsedTime / Shape.Duration) : 1.0f;
            ImColor Color(Shape.Color);
            Color.Value.w = Fade * Color.Value.w;
            DrawFunction(Shape, Color);

            if (ElapsedTime < 0 || ElapsedTime > Shape.Duration)
            {
                Shapes.RemoveAtSwap(i--);
            }
        }
    }
};

#endif //ENABLE_COG
