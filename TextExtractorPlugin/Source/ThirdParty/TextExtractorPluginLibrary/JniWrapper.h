// Copyright (c) 2016 Mickaël Fourgeaud

#pragma once

#include "jni.h"
#include <windows.h>
#include <string>

#ifdef _WIN32
#define PATH_SEPARATOR ';'
#else
#define PATH_SEPARATOR ':'
#endif

#define USER_CLASSPATH "." /* directory where Sample.class is present */

typedef jint(*P_JNI_GetDefaultJavaVMInitArgs)(void *args);
typedef jint(*P_JNI_CreateJavaVM)(JavaVM **pvm, void ** penv, void *args);

using namespace std;

class JniWrapper
{

public:
	JniWrapper(string PathToJvmDll, string JarPath);
	JniWrapper();
	~JniWrapper();
	
	/** Get/Set */
	JNIEnv* GetEnv();
	void SetEnv(JNIEnv * val) { env = val; }

private:
	string _JarPath;
	HMODULE _hLib = NULL;
	JNIEnv *env = NULL; /* Environment */
	JavaVM *jvm = NULL; /* Virtual Machine */
	P_JNI_GetDefaultJavaVMInitArgs pfnGetDefaultJavaVMInitArgs = NULL;
	P_JNI_CreateJavaVM _pfnCreateJavaVM = NULL;

	/** Load the JVM dll */
	void LoadJvmDll(string Path);

	void ShowError(JNIEnv* env);
};

