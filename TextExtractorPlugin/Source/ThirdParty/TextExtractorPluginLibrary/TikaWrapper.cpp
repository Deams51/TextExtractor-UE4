// Copyright (c) 2016 Mickaël Fourgeaud

#include "TikaWrapper.h"


TikaWrapper::TikaWrapper(string PathToJar, string PathToJvmDll)
	:Jni(PathToJvmDll, PathToJar)
{
	//CreateJvm("tika-app-1.13.jar");

	CreateTikaObject();

}TikaWrapper::TikaWrapper()
	:Jni()
{

}

TikaWrapper::~TikaWrapper()
{
}

void TikaWrapper::CreateTikaObject()
{
	const std::string TikaClassPath = "org/apache/tika/Tika";
	const std::string TikaConstructorName = "Tika";

	JNIEnv* Env = Jni.GetEnv();

	if (Env == nullptr)
		return; 

	// Get class 
	jclass TikaClass = Env->FindClass(TikaClassPath.c_str());
	if (TikaClass == NULL) {
		//ShowError(env);
		return;
	}

	// Get Class constructor
	jmethodID TikaClassCstrMID = Env->GetMethodID(TikaClass, "<init>", "()V");
	if (TikaClassCstrMID == 0) {
		//fprintf(stderr, "Can't find constructor for Tika class");
		return;
	}

	// Make & store Tika object
	TikaObject = Env->NewObject(TikaClass, TikaClassCstrMID);
	if (TikaObject == 0) {
		//fprintf(stderr, "Failed to create Tika object.");
		return;
	}

	// Get & store parseToString method from Tika class
	ParseToStringMID = Env->GetMethodID(TikaClass, "parseToString", "(Ljava/nio/file/Path;)Ljava/lang/String;");
	if (ParseToStringMID == 0) {
		//fprintf(stderr, "Failed to get parseToString from Tika class.");
		return;
	}
}


string TikaWrapper::ParseFile(const string& PathToFile)
{
	string result = string();

	JNIEnv* Env = Jni.GetEnv();

	if (Env == nullptr)
		return "Env == nullptr";

	// Create input strings for "get"
	jstring jstrPathToFile = Env->NewStringUTF(PathToFile.c_str());
	// Create an empty to feed the second arg, workaround from being force to have 2 args here...
	jstring jstrEmpty = Env->NewStringUTF("");

	// Get path object using "get"
	jobject pathObject = Env->CallStaticObjectMethod(GetPathsClass(), GetPathsGetMID(), jstrPathToFile, jstrEmpty);
	if (pathObject == 0) {
		//fprintf(stderr, "Failed to get path object from path=%s", PathToFile.c_str());
		return result;
	}

	// Parse at path
	jstring rv = (jstring)Env->CallObjectMethod(TikaObject, ParseToStringMID, pathObject);
	if (rv == 0) {
		//ShowError(env);
		//fprintf(stderr, "Failed to parse file.");
		return result;
	}

	const char* strReturn;
	jboolean isCopy;
	strReturn = Env->GetStringUTFChars(rv, &isCopy);
	result = string(strReturn);
	Env->ReleaseStringUTFChars(rv, strReturn);

	//printf("strReturn: %s", result);

	return result;
}

jclass TikaWrapper::GetPathsClass()
{
	// Try to get PathsClass
	if (PathsClass == 0 && Jni.GetEnv() != nullptr)
	{
		// Get paths class 
		PathsClass = Jni.GetEnv()->FindClass("java/nio/file/Paths");
		if (PathsClass == 0) {
			//fprintf(stderr, "Failed to get Paths class.");
		}
	}
	return PathsClass;
}

jmethodID TikaWrapper::GetPathsGetMID()
{
	// Try to get "get" static method from Paths class
	if (PathsGetMID == 0 && Jni.GetEnv() != nullptr)
	{
		PathsGetMID = Jni.GetEnv()->GetStaticMethodID(GetPathsClass(), "get", "(Ljava/lang/String;[Ljava/lang/String;)Ljava/nio/file/Path;");
		if (PathsGetMID == 0) {
			//fprintf(stderr, "Failed to get \"get\" static method from Paths class.");
		}
	}
	return PathsGetMID;
}
