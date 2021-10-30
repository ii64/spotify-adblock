#ifndef _ACL
#define _ACL

#include <Windows.h>
#include <string.h>
#include <stdlib.h>

#include "lib/log.c/src/log.h"

#include "config.h"
#include "acl.h"

//

wchar_t *char2wchar(const char *src, size_t sz)
{
    wchar_t *dst = (wchar_t *)calloc(sz, sizeof(wchar_t));
    if (!dst)
        return NULL;
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, -1,
                        dst, sz * sizeof(wchar_t));
    return dst;
}

// acl allow by default
int acl_is_allowed_request(wchar_t *method, size_t method_sz, wchar_t *url, size_t url_sz)
{
    for (int i = 0; i < configs->cef_acl_count; i++)
    {
        cef_acl_t t = configs->cef_acl[i];

        wchar_t *wsub = char2wchar(t.substr, strlen(t.substr));
        if (wsub == NULL)
        {
            goto econt;
        }
        if (!wcsstr(url, wsub))
        {
            // log_debug("   > suburl not match [%ls] [%ls]", wsub, url);
            free(wsub);
            goto econt;
        }

        wchar_t *wmet = char2wchar(t.method, strlen(t.method));
        if (wmet == NULL)
        {
            free(wsub);
            goto econt;
        }
        if (!wcsstr(method, wmet))
        {
            // log_debug("   > method not match [%ls] [%ls]", wmet, method);
            free(wsub);
            free(wmet);
            goto econt;
        }

        free(wsub);
        free(wmet);
        return ACL_DENY;
    econt:
        continue;
    }
    return ACL_ALLOW;
}

// acl allow by default
int acl_is_allowed_host(PCSTR host)
{
    for (int i = 0; i < configs->block_host_count; i++)
    {
        const char *t = configs->block_host[i];
        if (!strstr(host, t))
            return ACL_DENY;
    }
    return ACL_ALLOW;
}

#endif