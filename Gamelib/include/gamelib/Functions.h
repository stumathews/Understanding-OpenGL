#pragma once
#include <Windows.h>

// The new project template for a DLL project adds <PROJECTNAME>_EXPORTS to the defined preprocessor macros.
#ifdef GAMELIB_EXPORTS 
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __declspec(dllimport)
#endif

extern "C" LIBRARY_API void SayHello(const LPCSTR someText);
