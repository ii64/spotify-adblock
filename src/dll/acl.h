#ifndef _ACL_H
#define _ACL_H

#define ACL_ALLOW 2
#define ACL_DENY 4

int acl_is_allowed_request(wchar_t *method, size_t method_sz, wchar_t *url, size_t url_sz);
int acl_is_allowed_host(PCSTR host);

#endif