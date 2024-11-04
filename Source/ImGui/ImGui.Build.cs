// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class ImGui : ModuleRules
{
	public ImGui(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"ImGuiLibrary",
			"ImPlotLibrary",
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"ApplicationCore",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Slate",
			"SlateCore"
		});

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new[]
			{
				"MainFrame",
				"EditorStyle",
				"Settings",
				"UnrealEd",
			});
		}

		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "../ThirdParty/ImGuiLibrary/Private"));
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "../ThirdParty/ImPlotLibrary/Private"));

		PublicDefinitions.Add("IMGUI_USER_CONFIG=\"ImGuiConfig.h\"");
		//PublicDefinitions.Add("IMGUI_API=IMGUI_API");
		PublicDefinitions.Add("IMPLOT_API=IMGUI_API");

	}
}
