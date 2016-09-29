// Copyright (c) 2016 Mickaël Fourgeaud

#if defined _WIN32 || defined _WIN64
    #include <Windows.h>

    #define DLLEXPORT __declspec(dllexport)
#else
    #include <stdio.h>
#endif

#ifndef DLLEXPORT
    #define DLLEXPORT
#endif

#include "TikaWrapper.h"
#include <tchar.h>
#include <string>

static TikaWrapper* Tika;
static bool Initialized = nullptr; 

//DLLEXPORT void ExampleLibraryTikaInit()
//{
//	Initialized = true;
//	Tika = new TikaWrapper("C:\\Users\\Deams\\Documents\\Visual Studio 2015\\Projects\\TestJniApp - Copy\\tika-app-1.13.jar", "C:\\Program Files\\Java\\jre1.8.0_101\\bin\\server\\jvm.dll");
//	return;
//}
LONG GetStringRegKey(HKEY hKey, const std::wstring &strValueName, std::wstring &strValue, const std::wstring &strDefaultValue)
{
	strValue = strDefaultValue;
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError;
	nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
	if (ERROR_SUCCESS == nError)
	{
		strValue = szBuffer;
	}
	return nError;
}

bool FindJVMDll(string& PathToDll)
{
	HKEY hKey;
	std::wstring JavaKey(_T("SOFTWARE\\JavaSoft\\Java Runtime Environment"));
	PathToDll = "";

	if(RegOpenKeyExW(HKEY_LOCAL_MACHINE, JavaKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		// Fail
		return false;
	}

	std::wstring strValueOfVersion;
	if (GetStringRegKey(hKey, L"CurrentVersion", strValueOfVersion, L"bad") != ERROR_SUCCESS)
	{
		// Fail
		return false;
	}

	RegCloseKey(hKey);

	JavaKey.append(_T("\\"));
	JavaKey.append(strValueOfVersion); 
	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, JavaKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		// Fail
		return false;
	}

	std::wstring strValueOfRuntimeLib;
	if (GetStringRegKey(hKey, L"RuntimeLib", strValueOfRuntimeLib, L"bad") != ERROR_SUCCESS)
	{
		// Fail
		return false;
	}

	PathToDll = string(strValueOfRuntimeLib.begin(), strValueOfRuntimeLib.end()); 
	return true; 











	//char version[255];
	//char runtimeLib[255];
	//DWORD BufferSize = 8192;
	//DWORD dwType = REG_SZ;
	//PathToDll = "";
	////std::wstring JavaKey(_T("SOFTWARE\\JavaSoft\\Java Runtime Environment"));
	////std::wstring Value(_T("CurrentVersion"));

	//if (RegGetValue(HKEY_LOCAL_MACHINE, JavaKey.c_str(), Value.c_str(), RRF_RT_ANY, &dwType, (LPBYTE)&version, &BufferSize) != ERROR_SUCCESS)
	//	return false;

	//JavaKey.append(_T("\\"));

	////int size_needed = MultiByteToWideChar(CP_UTF8, 0, version, (int)BufferSize/sizeof(WCHAR), NULL, 0);
	////std::wstring wstrTo(size_needed, 0);
	////MultiByteToWideChar(CP_UTF8, 0, version, (int)BufferSize / sizeof(WCHAR), &wstrTo[0], size_needed);
	////
	////wstring versionWstr = wstring(version, BufferSize / sizeof(WCHAR));
	////string versionstr = string(version, BufferSize / sizeof(WCHAR));
	////JavaKey.append(wstrTo);

	//std::wstring Value2(_T("RuntimeLib"));
	//BufferSize = 8192;

	//if (RegGetValue(HKEY_LOCAL_MACHINE, JavaKey.c_str(), Value2.c_str(), RRF_RT_ANY, NULL, (PVOID)&runtimeLib, &BufferSize) != ERROR_SUCCESS)
	//	return false;

	//PathToDll = string(runtimeLib);
	//return true;
}

DLLEXPORT bool TikaWrapperInit(const string& JarPath)
{
	if (!Initialized)
	{
		// Find JVM dll 
		string JVMDllPath;
		if (FindJVMDll(JVMDllPath))
		{
			Tika = new TikaWrapper(JarPath, JVMDllPath);
			if (Tika)
			{
				Initialized = true;
			}
		}
		else
		{

		}
	}
	return Initialized;
}

DLLEXPORT void TikaWrapperParse(const string& FilePath, char** output)
{
	if (!Initialized)
		return; 

	string result = string(Tika->ParseFile(FilePath));

	*output = (char *)malloc(strlen(result.c_str()) + 1);
	strcpy(*output, result.c_str());
}

DLLEXPORT void TikaWrapperParseFree(char** output)
{
	delete[] *output;
}