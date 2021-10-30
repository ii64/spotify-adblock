#ifndef _MODIFY_H
#define _MODIFY_H

#include <Windows.h>

DWORD WINAPI modifier_thread(LPVOID);
void modifier_detach();

#endif