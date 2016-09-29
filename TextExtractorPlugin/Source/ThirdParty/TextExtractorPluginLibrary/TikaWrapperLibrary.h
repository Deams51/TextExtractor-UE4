// Copyright (c) 2016 Mickaël Fourgeaud

#include <string>

#if defined _WIN32 || defined _WIN64
#define DLLIMPORT __declspec(dllimport)
#else
#define DLLIMPORT
#endif

//DLLIMPORT void ExampleLibraryTikaInit();
DLLIMPORT bool TikaWrapperInit(const std::string& JarPath);
DLLIMPORT void TikaWrapperParse(const std::string& FilePath, char** output);
DLLIMPORT void TikaWrapperParseFree(char** output);
