// Copyright (c) 2016 Mickaël Fourgeaud

#pragma once

#include "ModuleManager.h"

class FTextExtractorPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void LoadDll();

private:
	/** Dll Handle */
	void* LibraryHandle;
};