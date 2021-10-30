#ifndef _INJECT
#define _INJECT

#include <Windows.h>

#include "lib/log.c/src/log.h"

#include "inject.h"

int _inject_dll(const int pid, const char *dll_path, size_t dll_path_sz)
{
    int ret;
    log_debug("opening process pid=%d", pid);
    HANDLE hProc;
    LPVOID mAlloc;
    int wRet;

    hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProc == NULL)
    {
        ret = INJECT_ERR_OPEN;
        goto eproc;
    }
    log_debug(" > ret %p", hProc);

    log_debug("alloc mem dll path sz=%d", dll_path_sz);
    mAlloc = VirtualAllocEx(hProc, NULL, dll_path_sz, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (mAlloc == NULL)
    {
        ret = INJECT_ERR_ALLOC;
        goto ealloc;
    }
    log_debug(" > ret %p", mAlloc);

    log_debug("write mem dll path ct=%s", dll_path);
    wRet = WriteProcessMemory(hProc, mAlloc, dll_path, dll_path_sz, 0);
    if (wRet == 0 || wRet == ERROR_INVALID_HANDLE)
    {
        ret = INJECT_ERR_MEMWRITE;
        goto ealloc;
    }
    log_debug(" > ret %p", wRet);

    log_debug("create remote thread");
    LPTHREAD_START_ROUTINE aLoadLibrary;
    DWORD dw;
    HANDLE hThread;
    HMODULE hmKernel32;

    hmKernel32 = GetModuleHandle("Kernel32");
    aLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(
        hmKernel32, "LoadLibraryA");
    hThread = CreateRemoteThread(hProc, NULL, 0, aLoadLibrary, mAlloc, 0, &dw);
    if (hThread == NULL)
    {
        ret = INJECT_ERR_EXEC;
        goto ethread;
    }
    log_debug(" > ret %p", hThread);

    log_debug("waiting");
    WaitForSingleObject(hThread, INFINITE);
    ret = INJECT_OK;

ethread:
    if (hThread != NULL)
        CloseHandle(hThread);
ealloc:
    if (mAlloc != NULL)
        VirtualFreeEx(hProc, mAlloc, 0, MEM_RELEASE);
eproc:
    if (hProc != NULL)
        CloseHandle(hProc);

    return ret;
}

int _free_dll(const int pid, const char *dll_path, size_t dll_path_sz)
{
    int ret;
    return ret;
}

#endif