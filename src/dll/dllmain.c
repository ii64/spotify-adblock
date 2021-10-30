#ifndef _DLLMAIN
#define _DLLMAIN

#include <Windows.h>

#include "lib/log.c/src/log.h"

#include "modify.h"

BOOL
    WINAPI
    DllMain(
        HINSTANCE hIns,
        DWORD dwReason,
        LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, modifier_thread, hIns, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        modifier_detach();
        break;
    }
    return TRUE;
}

#endif