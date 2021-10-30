#ifndef _MHOOK
#define _MHOOK

#include "lib/minhook/include/MinHook.h"

#define mhook_ex(mname, fname, dst, orig, ptarg)                                      \
    do                                                                                \
    {                                                                                 \
        (MH_CreateHookApiEx(mname, fname,                                             \
                            dst,                                                      \
                            orig,                                                     \
                            ptarg) != MH_OK)                                          \
            ? log_debug("** failed to hook " fname)                                   \
            : log_debug("** hook registered " fname " real:%p hook:%p", *orig, *dst); \
    } while (0)

#define mhook_create(fmsg, dst, orig, porig)                                       \
    do                                                                             \
    {                                                                              \
        (MH_CreateHook(orig, dst, porig) != MH_OK)                                 \
            ? log_debug("** failed to hook")                                       \
            : log_debug("** hook registered " fmsg " real:%p hook:%p", orig, dst); \
    } while (0)

#endif