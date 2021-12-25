#include "pch.h"
#include "Functions.h"

void SayHello(const LPCSTR someText)
{
	MessageBoxA(nullptr, someText, "DLL Text", MB_OK);
}