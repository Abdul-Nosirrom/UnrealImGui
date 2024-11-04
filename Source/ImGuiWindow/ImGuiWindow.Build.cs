using UnrealBuildTool;
using System.IO;

public class ImGuiWindow : ModuleRules
{
    public ImGuiWindow(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "ImGui", "ImGuiDebug", "InputCore"
            }
        );
        PublicIncludePaths.AddRange(
	        new string[] {
		        Path.Combine(ModuleDirectory, "../ThirdParty/ImGuiLibrary/Include"),
		        Path.Combine(ModuleDirectory, "../ThirdParty/ImPlotLibrary/Public"),
		        // ... add public include paths required here ...
	        }
        );
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DeveloperSettings",
                "LevelEditor",
            }
        );
    }
}
