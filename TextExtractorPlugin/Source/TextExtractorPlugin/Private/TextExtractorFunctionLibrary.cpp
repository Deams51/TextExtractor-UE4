// Copyright (c) 2016 Mickaël Fourgeaud

#include "TextExtractorPluginPrivatePCH.h"
#include "TextExtractorFunctionLibrary.h"
#include "TikaWrapperLibrary.h"
#include <string>

UTextExtractorFunctionLibrary::UTextExtractorFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FString UTextExtractorFunctionLibrary::GetTextFromFile(const FString& filePath)
{
	char* ResLocal; 
	char* result = nullptr;
	std::string FilePathStr = std::string(TCHAR_TO_UTF8(*filePath));
	
	// Get result from dll
	TikaWrapperParse(FilePathStr, &result);

	// Copy result from dll
	ResLocal = (char *)malloc(strlen(result) + 1);
	strcpy(ResLocal, result);

	// Free result from dll
	TikaWrapperParseFree(&result);

	return UTF8_TO_TCHAR(ResLocal);
}




