#include "SImGuiWidget.h"

#include "ImGuiContext.h"
#include "ImGuiInputHelper.h"
#include "Engine/GameViewportClient.h"
#include "imgui.h"
#include "SlateOptMacros.h"
#include "Widgets/SWindow.h"

//--------------------------------------------------------------------------------------------------------------------------
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SImGuiWidget::Construct(const FArguments& InArgs)
{
    Context = InArgs._Context;

    RefreshVisibility();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

//--------------------------------------------------------------------------------------------------------------------------
SImGuiWidget::~SImGuiWidget()
{
}

//--------------------------------------------------------------------------------------------------------------------------
void SImGuiWidget::SetDrawData(const ImDrawData* InDrawData)
{
    DrawData = FImGuiDrawData(InDrawData);
}

//--------------------------------------------------------------------------------------------------------------------------
void SImGuiWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    Super::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
    RefreshVisibility();
}

//--------------------------------------------------------------------------------------------------------------------------
int32 SImGuiWidget::OnPaint(
    const FPaintArgs& Args,
    const FGeometry& AllottedGeometry,
    const FSlateRect& MyClippingRect,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId,
    const FWidgetStyle& WidgetStyle,
    bool bParentEnabled) const
{

    const FSlateRenderTransform Transform(FImGuiHelper::RoundTranslation(AllottedGeometry.GetAccumulatedRenderTransform().GetTranslation() - FVector2d(DrawData.DisplayPos)));

    FSlateBrush TextureBrush;
    for (const FImGuiDrawList& DrawList : DrawData.DrawLists)
    {
        TArray<FSlateVertex> Vertices;
        Vertices.SetNumUninitialized(DrawList.VtxBuffer.Size);
        for (int32 BufferIdx = 0; BufferIdx < Vertices.Num(); ++BufferIdx)
        {
            const ImDrawVert& Vtx = DrawList.VtxBuffer.Data[BufferIdx];
            Vertices[BufferIdx] = FSlateVertex::Make<ESlateVertexRounding::Disabled>(Transform, FImGuiHelper::ToFVector2f(Vtx.pos), FImGuiHelper::ToFVector2f(Vtx.uv), FVector2f::UnitVector, FImGuiHelper::ToFColor(Vtx.col));
        }

        TArray<SlateIndex> Indices;
        Indices.SetNumUninitialized(DrawList.IdxBuffer.Size);
        for (int32 BufferIdx = 0; BufferIdx < Indices.Num(); ++BufferIdx)
        {
            Indices[BufferIdx] = DrawList.IdxBuffer.Data[BufferIdx];
        }

        for (const ImDrawCmd& DrawCmd : DrawList.CmdBuffer)
        {
            TArray VerticesSlice(Vertices.GetData() + DrawCmd.VtxOffset, Vertices.Num() - DrawCmd.VtxOffset);
            TArray IndicesSlice(Indices.GetData() + DrawCmd.IdxOffset, DrawCmd.ElemCount);

            UTexture2D* Texture = DrawCmd.GetTexID();
            if (TextureBrush.GetResourceObject() != Texture)
            {
                TextureBrush.SetResourceObject(Texture);
                if (IsValid(Texture))
                {
                    TextureBrush.ImageSize.X = Texture->GetSizeX();
                    TextureBrush.ImageSize.Y = Texture->GetSizeY();
                    TextureBrush.ImageType = ESlateBrushImageType::FullColor;
                    TextureBrush.DrawAs = ESlateBrushDrawType::Image;
                }
                else
                {
                    TextureBrush.ImageSize.X = 0;
                    TextureBrush.ImageSize.Y = 0;
                    TextureBrush.ImageType = ESlateBrushImageType::NoImage;
                    TextureBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
                }
            }

            FSlateRect ClipRect(DrawCmd.ClipRect.x, DrawCmd.ClipRect.y, DrawCmd.ClipRect.z, DrawCmd.ClipRect.w);
            ClipRect = TransformRect(Transform, ClipRect);

            OutDrawElements.PushClip(FSlateClippingZone(ClipRect));
            FSlateDrawElement::MakeCustomVerts(OutDrawElements, LayerId, TextureBrush.GetRenderingResource(), VerticesSlice, IndicesSlice, nullptr, 0, 0);
            OutDrawElements.PopClip();
        }
    }

    return LayerId;
}

//--------------------------------------------------------------------------------------------------------------------------
FVector2D SImGuiWidget::ComputeDesiredSize(float Scale) const
{
    return FVector2D::ZeroVector;
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& CharacterEvent)
{
    ImGuiIO& IO = ImGui::GetIO();
    IO.AddInputCharacter(FImGuiInputHelper::CastInputChar(CharacterEvent.GetCharacter()));

    const FReply Result = IO.WantCaptureKeyboard ? FReply::Handled() : FReply::Unhandled();
    return Result;
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
    return HandleKeyEvent(KeyEvent, true);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
    return HandleKeyEvent(KeyEvent, false);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::HandleKeyEvent(const FKeyEvent& KeyEvent, bool Down)
{
    if (Context->GetEnableInput() == false)
    {
        return FReply::Unhandled();
    }

    if (KeyEvent.GetKey().IsGamepadKey())
    {
        return FReply::Unhandled();
    }

    if (FImGuiInputHelper::IsKeyEventHandled(Context->GetGameViewport()->GetWorld(), KeyEvent) == false)
    {
        return FReply::Unhandled();
    }

    ImGuiIO& IO = ImGui::GetIO();
    IO.AddKeyEvent(FImGuiInputHelper::ToImKey(KeyEvent.GetKey()), Down);
    IO.AddKeyEvent(ImGuiMod_Ctrl, KeyEvent.IsControlDown());
    IO.AddKeyEvent(ImGuiMod_Shift, KeyEvent.IsShiftDown());
    IO.AddKeyEvent(ImGuiMod_Alt, KeyEvent.IsAltDown());
    IO.AddKeyEvent(ImGuiMod_Super, KeyEvent.IsCommandDown());

    if (IO.WantCaptureKeyboard == false && Context->GetShareKeyboard())
    {
        return FReply::Unhandled();
    }

    return FReply::Handled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& AnalogInputEvent)
{
    if (AnalogInputEvent.GetKey().IsGamepadKey())
    {
        return FReply::Unhandled();
    }

    return FReply::Handled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return HandleMouseButtonEvent(MouseEvent, true);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return HandleMouseButtonEvent(MouseEvent, true);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return HandleMouseButtonEvent(MouseEvent, false);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::HandleMouseButtonEvent(const FPointerEvent& MouseEvent, bool Down)
{
    if (Context->GetEnableInput() == false)
    {
        UE_LOG(LogImGui, VeryVerbose, TEXT("SImGuiWidget::HandleMouseButtonEvent | %s | Unhandled | EnableInput == false | Down:%d"), Window.IsValid() ? *Window->GetTitle().ToString() : *FString("None"), Down);
        return FReply::Unhandled();
    }
    const uint32 MouseButton = FImGuiInputHelper::ToImGuiMouseButton(MouseEvent.GetEffectingButton());
    ImGui::GetIO().AddMouseSourceEvent(ImGuiMouseSource_Mouse);
    ImGui::GetIO().AddMouseButtonEvent(MouseButton, Down);

    UE_LOG(LogImGui, VeryVerbose, TEXT("SImGuiWidget::HandleMouseButtonEvent | Window:%s | Handled | Down:%d"), Window.IsValid() ? *Window->GetTitle().ToString() : *FString("None"), Down);
    return FReply::Handled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (Context->GetEnableInput() == false)
    {
        return FReply::Unhandled();
    }
    ImGui::GetIO().AddMouseSourceEvent(ImGuiMouseSource_Mouse);
    ImGui::GetIO().AddMouseWheelEvent(0, MouseEvent.GetWheelDelta());
    return FReply::Handled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (Context->GetEnableInput() == false)
    {
        //UE_LOG(LogImGui, VeryVerbose, TEXT("SImGuiWidget::OnMouseMove | Window:%s | Unhandled | EnableInput == false"), Window.IsValid() ? *Window->GetTitle().ToString() : *FString("None"));
        return FReply::Unhandled();
    }

    ImGuiIO& IO = ImGui::GetIO();
    if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        IO.AddMousePosEvent(MouseEvent.GetScreenSpacePosition().X, MouseEvent.GetScreenSpacePosition().Y);
    }
    else
    {
        const FVector2D TransformedMousePosition = MouseEvent.GetScreenSpacePosition() - Context->GetMainWidget()->GetTickSpaceGeometry().GetAbsolutePosition();
        IO.AddMousePosEvent(TransformedMousePosition.X, TransformedMousePosition.Y);
    }

    //UE_LOG(LogImGui, VeryVerbose, TEXT("SImGuiWidget::OnMouseMove | Window:%s | Handled"), Window.IsValid() ? *Window->GetTitle().ToString() : *FString("None"));
    return FReply::Handled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiWidget::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& FocusEvent)
{
    return Super::OnFocusReceived(MyGeometry, FocusEvent);
}

//--------------------------------------------------------------------------------------------------------------------------
void SImGuiWidget::RefreshVisibility()
{
    EVisibility DesiredVisiblity = EVisibility::SelfHitTestInvisible;

    if (Context->GetEnableInput())
    {
        if (Context->GetShareMouse() && Context->GetWantCaptureMouse() == false)
        {
            DesiredVisiblity = EVisibility::SelfHitTestInvisible;
        }
        else
        {
            DesiredVisiblity = EVisibility::Visible;
        }
    }
    else
    {
        DesiredVisiblity = EVisibility::SelfHitTestInvisible;
    }

    if (DesiredVisiblity != GetVisibility())
    {
        SetVisibility(DesiredVisiblity);
    }
}
