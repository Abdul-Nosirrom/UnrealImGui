#include "ImGuiInputCatcherWidget.h"

#include "ImGuiContext.h"
#include "ImGuiInputHelper.h"
#include "Engine/GameViewportClient.h"
#include "imgui.h"
#include "SlateOptMacros.h"
#include "Widgets/SWindow.h"

//--------------------------------------------------------------------------------------------------------------------------
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SImGuiInputCatcherWidget::Construct(const FArguments& InArgs)
{
    Context = InArgs._Context;

    RefreshVisibility();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

//--------------------------------------------------------------------------------------------------------------------------
SImGuiInputCatcherWidget::~SImGuiInputCatcherWidget()
{
}

//--------------------------------------------------------------------------------------------------------------------------
void SImGuiInputCatcherWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    Super::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
    RefreshVisibility();
}

//--------------------------------------------------------------------------------------------------------------------------
int32 SImGuiInputCatcherWidget::OnPaint(
    const FPaintArgs& Args,
    const FGeometry& AllottedGeometry,
    const FSlateRect& MyClippingRect,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId,
    const FWidgetStyle& WidgetStyle,
    bool bParentEnabled) const
{
    return LayerId;
}

//--------------------------------------------------------------------------------------------------------------------------
FVector2D SImGuiInputCatcherWidget::ComputeDesiredSize(float Scale) const
{
    return FVector2D::ZeroVector;
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiInputCatcherWidget::OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& CharacterEvent)
{
    return FReply::Unhandled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiInputCatcherWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
    return FReply::Unhandled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiInputCatcherWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
    return FReply::Unhandled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiInputCatcherWidget::OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& AnalogInputEvent)
{
    return FReply::Unhandled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiInputCatcherWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return HandleMouseButtonEvent(MouseEvent, true);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiInputCatcherWidget::OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return HandleMouseButtonEvent(MouseEvent, true);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiInputCatcherWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return HandleMouseButtonEvent(MouseEvent, false);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiInputCatcherWidget::HandleMouseButtonEvent(const FPointerEvent& MouseEvent, bool Down)
{
    if (Context->GetEnableInput() == false)
    {
        UE_LOG(LogImGui, VeryVerbose, TEXT("SImGuiInputCatcherWidget::HandleMouseButtonEvent | Window:%s | Unhandled | EnableInput == false | Down:%d"), Window.IsValid() ? *Window->GetTitle().ToString() : *FString("None"), Down);
        return FReply::Unhandled();
    }

    const uint32 MouseButton = FImGuiInputHelper::ToImGuiMouseButton(MouseEvent.GetEffectingButton());
    ImGui::GetIO().AddMouseSourceEvent(ImGuiMouseSource_Mouse);
    ImGui::GetIO().AddMouseButtonEvent(MouseButton, Down);

    UE_LOG(LogImGui, VeryVerbose, TEXT("SImGuiInputCatcherWidget::HandleMouseButtonEvent | Window:%s | Handled | Down:%d"), Window.IsValid() ? *Window->GetTitle().ToString() : *FString("None"), Down);
    return FReply::Handled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SImGuiInputCatcherWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
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
FReply SImGuiInputCatcherWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return FReply::Unhandled();
}

//--------------------------------------------------------------------------------------------------------------------------
void SImGuiInputCatcherWidget::RefreshVisibility()
{
    EVisibility DesiredVisiblity = EVisibility::SelfHitTestInvisible;

    if (Context->GetEnableInput() && Context->GetShareMouseWithGameplay() == false)
    {
        DesiredVisiblity = EVisibility::Visible;
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
