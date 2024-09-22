#include "ImGuiDrawList.h"

#include "ImGuiHelper.h"

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDrawList::FImGuiDrawList(ImDrawList* Source)
{
    VtxBuffer.swap(Source->VtxBuffer);
    IdxBuffer.swap(Source->IdxBuffer);
    CmdBuffer.swap(Source->CmdBuffer);
    Flags = Source->Flags;
}

//--------------------------------------------------------------------------------------------------------------------------
FImGuiDrawData::FImGuiDrawData(const ImDrawData* Source)
{
    bValid = Source->Valid;

    TotalIdxCount = Source->TotalIdxCount;
    TotalVtxCount = Source->TotalVtxCount;

    DrawLists.SetNumUninitialized(Source->CmdListsCount);
    ConstructItems<FImGuiDrawList>(DrawLists.GetData(), Source->CmdLists.Data, Source->CmdListsCount);

    DisplayPos = FImGuiHelper::ToFVector2f(Source->DisplayPos);
    DisplaySize = FImGuiHelper::ToFVector2f(Source->DisplaySize);
    FrameBufferScale = FImGuiHelper::ToFVector2f(Source->FramebufferScale);
}
