#ifndef _HOOK_CEF
#define _HOOK_CEF

#include <Windows.h>

#include "lib/log.c/src/log.h"
#include "lib/minhook/include/MinHook.h"
#include "lib/cef/include/capi/cef_urlrequest_capi.h"

#include "mhook.h"
#include "log_util.h"
#include "acl.h"

HMODULE g_h_libcef = NULL;

//
typedef int (*cef_string_utf16_to_wide__t)(const char16 *src,
                                           size_t src_len,
                                           cef_string_wide_t *output);
cef_string_utf16_to_wide__t g_fp_cef_string_utf16_to_wide = NULL;
int detour_cef_string_utf16_to_wide(const char16 *src,
                                    size_t src_len,
                                    cef_string_wide_t *output)
{
    return g_fp_cef_string_utf16_to_wide(src, src_len, output);
}

typedef void (*cef_string_userfree_utf16_free__t)(cef_string_userfree_utf16_t str);
cef_string_userfree_utf16_free__t g_fp_cef_string_userfree_utf16_free = NULL;
void detour_cef_string_userfree_utf16_free(cef_string_userfree_utf16_t str)
{
    return g_fp_cef_string_userfree_utf16_free(str);
}

typedef cef_urlrequest_t *(*cef_urlrequest_create__t)(
    struct _cef_request_t *request,
    struct _cef_urlrequest_client_t *client,
    struct _cef_request_context_t *request_context);
cef_urlrequest_create__t g_fp_cef_urlrequest_create = NULL;
cef_urlrequest_t *detour_cef_urlrequest_create(
    struct _cef_request_t *request,
    struct _cef_urlrequest_client_t *client,
    struct _cef_request_context_t *request_context)
{
    cef_urlrequest_t *ret;
    cef_string_userfree_t r_method = request->get_method(request);
    cef_string_userfree_t r_url = request->get_url(request);

    cef_string_wide_t *w_method = (cef_string_wide_t *)calloc(1, sizeof(cef_string_wide_t));

    cef_string_wide_t *w_url = (cef_string_wide_t *)calloc(1, sizeof(cef_string_wide_t));
    g_fp_cef_string_utf16_to_wide(r_method->str, r_method->length, w_method);
    g_fp_cef_string_utf16_to_wide(r_url->str, r_url->length, w_url);

    log_debug("detour_cef_urlrequest_create(method:%ls, url:%ls, ... );",
              r_method->str,
              r_url->str);

    if ((acl_is_allowed_request(w_method->str, w_method->length,
                                w_url->str, w_url->length) &
         ACL_DENY) == ACL_DENY)
    {
        log_debug(" >> cef deined");
        ret = NULL;
        goto efree;
    }

    log_debug(" >> cef passed");
    ret = g_fp_cef_urlrequest_create(request, client, request_context);

efree:
    g_fp_cef_string_userfree_utf16_free(r_method);
    g_fp_cef_string_userfree_utf16_free(r_url);
    free(w_method);
    free(w_url);
    return ret;
}

void hook_reg_cef()
{
    g_h_libcef = GetModuleHandle("libcef.dll");
    log_debug("h_cef: %p", g_h_libcef);

    mhook_ex(L"libcef", "cef_string_utf16_to_wide",
             &detour_cef_string_utf16_to_wide, &g_fp_cef_string_utf16_to_wide, NULL);

    mhook_ex(L"libcef", "cef_string_userfree_utf16_free",
             &detour_cef_string_userfree_utf16_free, &g_fp_cef_string_userfree_utf16_free, NULL);

    mhook_ex(L"libcef", "cef_urlrequest_create",
             &detour_cef_urlrequest_create, &g_fp_cef_urlrequest_create, NULL);
}

#endif