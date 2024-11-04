#pragma once

#include "CoreMinimal.h"
#include "ImGuiDrawList.h"
#include "Rendering/RenderingCommon.h"
#include "UObject/WeakObjectPtr.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SLeafWidget.h"

class FImGuiContext;
class SWindow;
class UGameViewportClient;

//--------------------------------------------------------------------------------------------------------------------------
class IMGUI_API SImGuiInputCatcherWidget : public SLeafWidget
{
    typedef SLeafWidget Super;

public:

    SLATE_BEGIN_ARGS(SImGuiInputCatcherWidget) {}
        SLATE_ARGUMENT(FImGuiContext*, Context)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    ~SImGuiInputCatcherWidget();

    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& WidgetStyle, bool bParentEnabled) const override;

    virtual bool SupportsKeyboardFocus() const override { return true; }

    virtual FReply OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& CharacterEvent) override;

    virtual FVector2D ComputeDesiredSize(float Scale) const override;

    virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent) override;

    virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent) override;

    virtual FReply OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& AnalogInputEvent) override;

    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

    virtual FReply OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

    virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

    virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

    virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

    TSharedPtr<const SWindow> GetWindow() const { return Window; }

    void SetWindow(TSharedPtr<SWindow> Value) { Window = Value; }

protected:

    FReply HandleKeyEvent(const FKeyEvent& KeyEvent, bool Down);

    FReply HandleMouseButtonEvent(const FPointerEvent& MouseEvent, bool Down);

    void RefreshVisibility();

    FImGuiContext* Context = nullptr;

    TSharedPtr<SWindow> Window = nullptr;
};