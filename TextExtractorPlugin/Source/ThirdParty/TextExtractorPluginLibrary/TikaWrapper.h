// Copyright (c) 2016 Mickaël Fourgeaud

#pragma once

#include "JniWrapper.h"
#include <string>

using namespace std;

class TikaWrapper
{
public:
	TikaWrapper();
	TikaWrapper(string PathToJar, string PathToJvmDll);
	~TikaWrapper();

	string ParseFile(const string& PathToFile);




private:
	JniWrapper Jni; 
	jobject TikaObject;
	jmethodID ParseToStringMID; 
	jclass PathsClass;
	jmethodID PathsGetMID;

	void CreateTikaObject();

	jclass GetPathsClass();
	jmethodID GetPathsGetMID();

};

