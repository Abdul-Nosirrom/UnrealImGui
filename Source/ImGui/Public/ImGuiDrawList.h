#pragma once

#include "CoreMinimal.h"
#include "ImGuiHelper.h"
#include "imgui.h"
#include "Rendering/RenderingCommon.h"

//--------------------------------------------------------------------------------------------------------------------------
struct FImGuiDrawList
{
	FImGuiDrawList() {};

	FImGuiDrawList(ImDrawList* Source);

	ImVector<ImDrawVert> VtxBuffer;

	ImVector<ImDrawIdx> IdxBuffer;

	ImVector<ImDrawCmd> CmdBuffer;

	ImDrawListFlags Flags = ImDrawListFlags_None;
};

//--------------------------------------------------------------------------------------------------------------------------
struct FImGuiDrawData
{
	FImGuiDrawData() {};

	FImGuiDrawData(const ImDrawData* Source);

	bool bValid = false;

	int32 TotalIdxCount = 0;

	int32 TotalVtxCount = 0;

	TArray<FImGuiDrawList> DrawLists;

	FVector2f DisplayPos = FVector2f::ZeroVector;

	FVector2f DisplaySize = FVector2f::ZeroVector;

	FVector2f FrameBufferScale = FVector2f::ZeroVector;
};
