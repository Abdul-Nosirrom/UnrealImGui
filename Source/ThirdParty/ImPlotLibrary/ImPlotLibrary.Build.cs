using System.IO;
using UnrealBuildTool;

public class ImPlotLibrary : ModuleRules
{
	public ImPlotLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PublicSystemIncludePaths.Add(ModuleDirectory);

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

	}
}
