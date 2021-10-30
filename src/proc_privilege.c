#ifndef _PROC_PRIV
#define _PROC_PRIV

#include <Windows.h>

#include "lib/log.c/src/log.h"

#include "proc_privilege.h"

int enable_debug_privilege(int f_enable)
{
    int ret = FALSE;
    HANDLE hToken;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        log_debug("OpenProcessToken ok");
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount = 1;
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
        tp.Privileges[0].Attributes = f_enable ? SE_PRIVILEGE_ENABLED : 0;
        AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
        ret = GetLastError();
        log_debug("AdjustTokenPrivleges = %p", ret);
        ret = ret == ERROR_SUCCESS;
        CloseHandle(hToken);
    }
    return ret;
}

#endif