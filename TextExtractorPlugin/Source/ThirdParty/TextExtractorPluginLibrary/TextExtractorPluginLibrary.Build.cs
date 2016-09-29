// Copyright (c) 2016 Mickaël Fourgeaud

using System.IO;
using UnrealBuildTool;

public class TextExtractorPluginLibrary : ModuleRules
{
	public TextExtractorPluginLibrary(TargetInfo Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "x64", "Release"));
			PublicAdditionalLibraries.Add("TikaWrapperLibrary.lib");

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("TikaWrapperLibrary.dll");
		}
	}
}
