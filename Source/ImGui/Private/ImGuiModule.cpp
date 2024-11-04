#include "ImGuiModule.h"

#include "imgui.h"
#include "Editor/ImGuiKeyInfoCustomization.h"
#include "HAL/LowLevelMemTracker.h"
#include "HAL/UnrealMemory.h"

#define LOCTEXT_NAMESPACE "FImGuiModule"

//--------------------------------------------------------------------------------------------------------------------------
static void* ImGui_MemAlloc(size_t Size, void* UserData)
{
    LLM_SCOPE_BYNAME(TEXT("ImGui"));
    return FMemory::Malloc(Size);
}

//--------------------------------------------------------------------------------------------------------------------------
static void ImGui_MemFree(void* Ptr, void* UserData)
{
    FMemory::Free(Ptr);
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiModule::StartupModule()
{
    ImGui::SetAllocatorFunctions(ImGui_MemAlloc, ImGui_MemFree);

	//FCoreDelegates::OnBeginFrame.AddLambda([]{ ImGui::Ren})

	// Register keyinfo property customization
#if WITH_EDITOR
	if (auto PropertyEditorModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		PropertyEditorModule->RegisterCustomPropertyTypeLayout("ImGuiKeyInfo", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FImGuiKeyInfoCustomization::MakeInstance));
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
void FImGuiModule::ShutdownModule()
{
#if WITH_EDITOR
	if (auto PropertyEditorModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		PropertyEditorModule->UnregisterCustomPropertyTypeLayout("ImGuiKeyInfo");
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FImGuiModule, ImGui)
