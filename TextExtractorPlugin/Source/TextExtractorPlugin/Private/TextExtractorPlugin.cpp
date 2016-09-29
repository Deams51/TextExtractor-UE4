// Copyright (c) 2016 Mickaël Fourgeaud

#include "TextExtractorPluginPrivatePCH.h"
#include "Core.h"
#include "TikaWrapperLibrary.h"
#include "ModuleManager.h"
#include "IPluginManager.h"

#define LOCTEXT_NAMESPACE "FTextExtractorPluginModule"

void FTextExtractorPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	LoadDll();
}

void FTextExtractorPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(LibraryHandle);
	LibraryHandle = nullptr;
}

void FTextExtractorPluginModule::LoadDll()
{
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("TextExtractorPlugin")->GetBaseDir();

	// PLATFORM_WINDOWS
	FString LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/TextExtractorPluginLibrary/Win64/TikaWrapperLibrary.dll"));
	
	// load dll
	LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	// Init JVM
	FString JarLibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/TextExtractorPluginLibrary/JarLibrary/tika-app-1.13.jar"));
	std::string JarLibraryPathStr = std::string(TCHAR_TO_UTF8(*JarLibraryPath));
	if (!TikaWrapperInit(JarLibraryPathStr))
	{
		// Failed to load JVM
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("TextExtractorPluginError", "Failed to load Java, please verify that the latest Java Runtime is installed."));
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTextExtractorPluginModule, TextExtractorPlugin)