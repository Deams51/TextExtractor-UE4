// Copyright (c) 2016 Mickaël Fourgeaud

#include "JniWrapper.h"
#include <stdio.h>
#include <windows.h> // for EXCEPTION_ACCESS_VIOLATION
#include <excpt.h>
#include <stdlib.h>
#include <signal.h>
#include <tchar.h>

#define CLEAR(x) memset(&x, 0, sizeof(x))

JniWrapper::JniWrapper(string PathToJvmDll, string JarPath)
	:_JarPath(JarPath)
{
	LoadJvmDll(PathToJvmDll);
}

JniWrapper::JniWrapper()
	: _JarPath()
{

}


JniWrapper::~JniWrapper()
{
}

void JniWrapper::LoadJvmDll(string Path)
{
	if (Path.empty())
	{
		printf("Failed to load library, path is empty.\n");
		return;
	}

	/* Load library at path */
	_hLib = LoadLibraryA(Path.c_str());

	if (_hLib == NULL) {
		printf("Unable to Load Library.");
	}
}

void SignalHandler(int signal)
{
	printf("Signal %d", signal);
	//throw "!Access Violation!";
}

void JniWrapper::ShowError(JNIEnv* env)
{
	auto exc = env->ExceptionOccurred();
	if (exc)
	{
		env->ExceptionClear();

		// Get the class
		jclass exccls = env->GetObjectClass(exc);

		// Get method ID for methods 
		jmethodID getCodeMeth = env->GetMethodID(exccls, "getCode", "()I");

		jmethodID getMsgMeth = env->GetMethodID(exccls, "toString", "()Ljava/lang/String;");

		jstring obj = (jstring)env->CallObjectMethod(exccls, getMsgMeth);
		const char *nativeString = env->GetStringUTFChars(obj, JNI_FALSE);

		fprintf(stderr, "JNI Exception class: %s\n", nativeString);

		env->ReleaseStringUTFChars(obj, nativeString);
	}
}

JNIEnv* JniWrapper::GetEnv()
{
	if (_hLib == nullptr)
		return nullptr;

	if (env == nullptr)
	{
		typedef void(*SignalHandlerPointer)(int);

		//SignalHandlerPointer previousHandler;
		//previousHandler = signal(SIGSEGV, SignalHandler);
		//const LPTOP_LEVEL_EXCEPTION_FILTER oldExceptionFilter = SetUnhandledExceptionFilter(NULL);


		JavaVMInitArgs vm_args;
		CLEAR(vm_args);
		JavaVMOption options;
		CLEAR(options);

		string optionTemp = "-Djava.class.path=" + _JarPath;
		options.optionString = (char*)optionTemp.c_str();
		options.extraInfo = 0;
		vm_args.version = JNI_VERSION_1_8;
		vm_args.nOptions = 1;
		vm_args.options = &options;
		vm_args.ignoreUnrecognized = 0;

		/* Store the function pointer for creating the VM */
		try
		{
			_pfnCreateJavaVM = (P_JNI_CreateJavaVM)GetProcAddress(_hLib, "JNI_CreateJavaVM");
		}
		catch (...)
		{
			fprintf(stderr, "Exception: Can't create Java VM\n");
		}



		/* Create the Java VM */
		jint res = -1;
		if (_pfnCreateJavaVM != NULL)
		{
			try
			{
				res = (*_pfnCreateJavaVM)(&jvm, (void **)&env, &vm_args);
			}
			catch (...)
			{
				fprintf(stderr, "Exception: Can't create Java VM\n");
			}
		}

		// Failed
		if (res < 0) {
			fprintf(stderr, "Can't create Java VM\n");
			env = nullptr; 
		}
	}
	return env;
}


