#ifndef _FINDPROCID
#define _FINDPROCID

#include <stdlib.h>

#include <Windows.h>
#include <TlHelp32.h>

#include "lib/log.c/src/log.h"

#include "find_process_id.h"

int _find_process_id(const char *title, size_t sz)
{
    log_debug("finding process [%s]", title);

    int ret = FIND_PROCESS_FAIL;
    PROCESSENTRY32 *process_snapshot;
    HANDLE snapshot;

    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    process_snapshot = (PROCESSENTRY32 *)calloc(1, sizeof(PROCESSENTRY32));
    memset(process_snapshot, 0, sizeof(PROCESSENTRY32));
    process_snapshot->dwSize = sizeof(PROCESSENTRY32);

    if (snapshot == INVALID_HANDLE_VALUE ||
        Process32First(snapshot, process_snapshot) == FALSE)
    {
        ret = FIND_PROCESS_FAIL;
        goto fret;
    }

    ret = FIND_PROCESS_NOT_FOUND;
    while (Process32Next(snapshot, process_snapshot))
    {
        if (strncmp(title, process_snapshot->szExeFile, sz) == 0)
        {
            ret = process_snapshot->th32ProcessID;
            log_debug("found process name [ %s == %s ]", process_snapshot->szExeFile, title);
            break;
        }
    }

fret:
    free(process_snapshot);
    CloseHandle(snapshot);
    return ret;
}

#endif