#ifndef _FINDPROCID_H
#define _FINDPROCID

#include <string.h>

#define FIND_PROCESS_FAIL -1
#define FIND_PROCESS_NOT_FOUND -2

#define find_process_id(name) _find_process_id(name, strlen(name))
int _find_process_id(const char *title, size_t sz);

#endif