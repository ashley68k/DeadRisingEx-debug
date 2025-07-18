/*

*/

#include "Utilities.h"
#include "Misc/AsmHelpers.h"
#include "detours.h"
#include <stdio.h>

// Function pointers/trampolines:
// already unstubbed, this is async printer
void(__stdcall *DbgPrintError)(void **pPrintFunctions, const char *psFormat, __int64 arg1, __int64 arg2, __int64 arg3) = (void(__stdcall*)(void**, const char*, __int64, __int64, __int64))(__int64)GetModuleAddress((void*)0x14002F6D0);

// this is basically just printf
void(__cdecl* PrintfStub)(const char* _Format, ...) = (void(__cdecl*)(const char*, ...))(__int64)GetModuleAddress((void*)0x140650EB0);

// Forward declarations:
void __stdcall Hook_DbgPrintError(void **pPrintFunctions, const char *psFormat, __int64 arg1, __int64 arg2, __int64 arg3);
void __cdecl Hook_PrintfStub(const char* _Format, ...);

bool Utilities::InstallHooks()
{
    // Hook debug print functions.
    DetourAttach((void**)&DbgPrintError, Hook_DbgPrintError);
    DetourAttach((void**)&PrintfStub, Hook_PrintfStub);

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

// same as above, but printf is variadic so we use va_list with vsnprintf.
// signature should match standard printf and the printf stub seen at 0x140650EB0
void __cdecl Hook_PrintfStub(const char* _Format, ...)
{
    va_list args;
    va_start(args, _Format);

    char sMessage[512] = { 0 };
    vsnprintf(sMessage, sizeof(sMessage), _Format, args);

    OutputDebugStringA(sMessage);

    va_end(args);
}