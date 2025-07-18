/*

*/

#include "Utilities.h"
#include "Misc/AsmHelpers.h"
#include "detours.h"
#include <stdio.h>
#include <string>

// Function pointers/trampolines:
void(__stdcall *DbgPrintError)(void **pPrintFunctions, const char *psFormat, __int64 arg1, __int64 arg2, __int64 arg3) = (void(__stdcall*)(void**, const char*, __int64, __int64, __int64))(__int64)GetModuleAddress((void*)0x14002F6D0);
void(__stdcall *DbgVariadicPrintError)(const char* psFormat,...) = (void(__stdcall*)(const char*,...))(__int64)GetModuleAddress((void*)0x140650EB0);

// Forward declarations:
void __stdcall Hook_DbgPrintError(void **pPrintFunctions, const char *psFormat, __int64 arg1, __int64 arg2, __int64 arg3);
void __stdcall Hook_DbgVariadicPrintError(const char* psFormat,...);

bool Utilities::InstallHooks()
{
    // Hook debug print functions.
    DetourAttach((void**)&DbgPrintError, Hook_DbgPrintError);
    DetourAttach((void**)&DbgVariadicPrintError, Hook_DbgVariadicPrintError);

    return true;
}

void __stdcall Hook_DbgPrintError(void **pPrintFunctions, const char *psFormat, __int64 arg1, __int64 arg2, __int64 arg3)
{
    char sMessage[512] = { 0 };

    // Format the error message.
    snprintf(sMessage, sizeof(sMessage), psFormat, arg1, arg2, arg3);

    // TODO: Print to console

    // Print to the debugger.
    OutputDebugStringA(sMessage);
}

// this is more or less the same as the last function, but it doesn't seem to have a pointer to what I believe is the caller
void __stdcall Hook_DbgVariadicPrintError(const char* psFormat,...)
{
    va_list args;
    va_start(args, psFormat);

    char sMessage[512] = { 0 };

    vsnprintf(sMessage, sizeof(sMessage), psFormat, args);

    va_end(args);

    OutputDebugStringA(sMessage);
}