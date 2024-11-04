using System.IO;
using UnrealBuildTool;

public class ImGuiLibrary : ModuleRules
{
	public ImGuiLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PublicSystemIncludePaths.Add(ModuleDirectory);

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));

	}
}
