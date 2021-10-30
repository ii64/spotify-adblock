#ifndef _MODIFY
#define _MODIFY

#include <Windows.h>
#include <Psapi.h>
#include <locale.h>

#include "lib/log.c/src/log.h"
#include "lib/minhook/include/MinHook.h"

#include "config.h"
#include "modify.h"
#include "log_util.h"
#include "hook.h"

//
char *g_Cmdline = NULL;
HANDLE g_Handle = NULL;
LPVOID g_DllHandle = NULL;
HWND g_HWND = NULL;
DWORD g_Pid = 0;
//

static void __alloc_console()
{
    AllocConsole();
    FILE *f;
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONOUT$", "w", stdout);

    setlocale(LC_ALL, "C.UTF-8");
}

FILE *flog;
static void __setup_logger()
{
    flog = fopen("log.txt", "a");
    log_add_fp(flog, LOG_DEBUG);
}

// ----------------------------------------------------------------

BOOL CALLBACK cb_window_enumeration(HWND hwnd, LPARAM lParam)
{
    DWORD lpdwPid;
    GetWindowThreadProcessId(hwnd, &lpdwPid);
    if (lpdwPid == lParam)
    {
        g_HWND = hwnd;
        return FALSE;
    }
    return TRUE;
}
void get_window_thread_pid(DWORD pid)
{
    EnumWindows(cb_window_enumeration, pid);
}

//------------------------------

void hook_reg()
{
    hook_reg_cef();
    hook_reg_ws();
}
void hook_enable()
{
    log_debug("** all hook enabled");
    (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
        ? log_debug("hook enable failed")
        : log_debug("hook enable ok");
}

//------------------------------

DWORD
WINAPI
modifier_thread(LPVOID dllHandle)
{
    __alloc_console();
    __setup_logger();

    log_info("Spotify adblock");
    log_debug("modifer started");

    log_debug("loading config");
    if (config_load_file("adblock.yaml") != CONF_OK)
    {
        log_debug("config not ok");
        return 1;
    }
    log_debug("config ok");

    log_debug("* total block host count:    %d", configs->block_host_count);
    log_debug("* total cef substring count: %d", configs->cef_acl_count);

    g_Cmdline = GetCommandLine();
    g_DllHandle = dllHandle;
    g_Pid = GetCurrentProcessId();
    g_Handle = GetCurrentProcess();
    log_debug("pid:  %d", g_Pid);
    log_debug("hdl:  %p", g_Handle);
    log_debug("dll:  %p", g_DllHandle);
    get_window_thread_pid(g_Pid);
    log_debug("hwnd: %d", g_HWND);

    if (MH_Initialize() != MH_OK)
    {
        log_debug("minhook init not ok");
        return 1;
    }
    log_debug("minhook init ok");

    HMODULE hModule = GetModuleHandle(NULL);
    MODULEINFO mInfo = {0};
    if (GetModuleInformation(g_Handle, hModule, &mInfo, sizeof(MODULEINFO)))
    {
        log_debug("GetModuleInformation ok");
        log_debug(" > hModule: %p", hModule);
        log_debug(" > szOfImg: %p", mInfo.SizeOfImage);
    }

    //
    hook_reg();
    hook_enable();

    log_debug("init finised");
    return 0;
}

void modifier_detach()
{
    (MH_Uninitialize() != MH_OK)
        ? log_debug("minhook uninit not ok")
        : log_debug("minhook uninit ok");

    (flog != NULL)
        ? fclose(flog)
        : log_error("flog ptr null");

    ((config_close() & CONF_ERR) == CONF_ERR)
        ? log_debug("config free not ok")
        : log_debug("config free ok");

    log_debug("detached");
    // MessageBox(NULL, "Exited", "Info", MB_ICONINFORMATION);
}

#endif