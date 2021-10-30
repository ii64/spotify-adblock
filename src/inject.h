#ifndef _INJECT_H
#define _INJECT_H

#include <string.h>

#define INJECT_ERR 2
#define INJECT_OK 4
#define INJECT_ERR_OPEN INJECT_ERR | 16
#define INJECT_ERR_ALLOC INJECT_ERR | 32
#define INJECT_ERR_MEMWRITE INJECT_ERR | 64
#define INJECT_ERR_EXEC INJECT_ERR | 128

#define inject_dll(pid, path) _inject_dll(pid, path, strlen(path))
int _inject_dll(const int pid, const char *dll_path, size_t dll_path_sz);

#endif