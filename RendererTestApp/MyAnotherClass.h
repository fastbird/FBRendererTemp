#pragma once

#include "MyClass.h"

class MyAnotherClass
{
	MyClass* pMyClass; // 8
	MyClass& refMyClass; // 8
	
	MyClass MyCl; // 12
};
