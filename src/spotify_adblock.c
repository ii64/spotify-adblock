#ifndef _SPOT_MAIN
#define _SPOT_MAIN

// lib
#include <Windows.h>
#include <Shlwapi.h>
#include <fcntl.h>
#include <unistd.h>

#include "lib/log.c/src/log.h"

#include "proc_privilege.h"
#include "find_process_id.h"
#include "inject.h"

// app
#define TARGET "Spotify.exe"

int main(int argc, char *argv[])
{
    log_info("program start");
    if (argc < 2)
    {
        log_error("too few arguments, provide dll name");
        return 1;
    }
    char *dll_path = strdup(argv[1]);
    char dllFullPath[MAX_PATH];
    if (dll_path)
    {
        if (_fullpath(dllFullPath, dll_path, MAX_PATH) == NULL)
        {
            log_error("invalid path: %s", dll_path);
            return 1;
        }
        dll_path = (char *)calloc(strlen(dllFullPath), sizeof(char));
        strncpy(dll_path, dllFullPath, strlen(dllFullPath));
    }
    log_debug("provided dll name: %s", dll_path);
    if (access(dll_path, F_OK) != 0)
    {
        log_error("dll is not exist");
        return 1;
    }

    log_info("enable debug privilege");
    if (!enable_debug_privilege(TRUE))
    {
        log_error("failed to enable debug privilege");
        return 1;
    }

    int pid = find_process_id(TARGET);
    if (pid == FIND_PROCESS_FAIL)
    {
        log_error("failed to enumerate process");
        return 1;
    }
    else if (pid == FIND_PROCESS_NOT_FOUND)
    {
        log_error("process not found [%s]", TARGET);
        return 1;
    }

    log_info("process found pid=%d", pid);

    log_info("injecting dll");
    int ret = inject_dll(pid, dll_path);
    if ((ret | INJECT_ERR) == INJECT_ERR)
    {
        log_error("inject error %d", ret);
        return 1;
    }

    log_info("program end");
    return 0;
}

#endif