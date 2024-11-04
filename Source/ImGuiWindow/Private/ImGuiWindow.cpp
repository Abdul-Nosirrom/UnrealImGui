// Copyright 2023 CoC All rights reserved
#include "ImGuiWindow.h"

#include "ImGuiDebug.h"
#include "ImGuiWindowManager.h"
#include "imgui_internal.h"


void FImGuiWindow::Render(float DeltaTime)
{
	ImGuiWindowFlags WindowFlags = 0;
	PreRender(WindowFlags);

	const FString WindowTitle = GetTitle() + "##" + Name;

	if (bHasMenu && bHideMenu == false)
	{
		WindowFlags |= ImGuiWindowFlags_MenuBar;
	}

	if (bNoPadding)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	}

	if (ImGui::Begin(TCHAR_TO_ANSI(*WindowTitle), &bIsVisible, WindowFlags))
	{
		if (bNoPadding)
		{
			ImGui::PopStyleVar(1);
		}

		if (ImGui::BeginPopupContextWindow())
		{
			if (bHasMenu)
			{
				ImGui::Checkbox("Hide Menu", &bHideMenu);
			}

			ImGui::EndPopup();
		}

		RenderContent();
	}
	else
	{
		if (bNoPadding)
		{
			ImGui::PopStyleVar(1);
		}
	}

	ImGui::End();

	PostRender();
}

void FImGuiWindow::Tick(float DeltaTime)
{
	SetSelection(FImGuiDebug::GetSelection());
}

void FImGuiWindow::SetFullName(const FString& InFullName)
{
	FullName = InFullName;

	TArray<FString> Path;
	int CharIndex = 0;
	if (FullName.FindLastChar(TEXT('.'), CharIndex))
	{
		Name = FullName.RightChop(CharIndex + 1);
	}
	else
	{
		Name = FullName;
	}

	Title = Name;

	ID = ImHashStr(TCHAR_TO_ANSI(*FullName));
}

void FImGuiWindow::SetSelection(AActor* Actor)
{
	if (CurrentSelection == Actor)
	{
		return;
	}

	AActor* OldActor = CurrentSelection.Get();

	CurrentSelection = Actor;
	OnSelectionChanged(OldActor, Actor);
}

UImGuiWindowConfig* FImGuiWindow::GetConfig(const TSubclassOf<UImGuiWindowConfig> ConfigClass) const
{
	return GetOwner()->GetConfig(ConfigClass);
}

const UObject* FImGuiWindow::GetAsset(const TSubclassOf<UObject> AssetClass) const
{
	return GetOwner()->GetAsset(AssetClass);
}

UWorld* FImGuiWindow::GetWorld() const
{
	return Owner->GetWorld();
}

APawn* FImGuiWindow::GetLocalPlayerPawn() const
{
	const APlayerController* LocalPlayerController = GetLocalPlayerController();

	if (LocalPlayerController == nullptr)
	{
		return nullptr;
	}

	APawn* Pawn = LocalPlayerController->GetPawn();

	return Pawn;
}

APlayerController* FImGuiWindow::GetLocalPlayerController() const
{
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer == nullptr)
	{
		return nullptr;
	}

	return LocalPlayer->PlayerController;
}

ULocalPlayer* FImGuiWindow::GetLocalPlayer() const
{
	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return nullptr;
	}

	return World->GetFirstLocalPlayerFromController();
}
