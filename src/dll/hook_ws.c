#ifndef _HOOK_WS
#define _HOOK_WS

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#include "lib/log.c/src/log.h"
#include "mhook.h"
#include "log_util.h"
#include "acl.h"

//

HMODULE g_h_libws2 = NULL;

//

typedef struct hostent *(WINAPI *gethostbyname__t)(const char *name);
gethostbyname__t g_fp_gethostbyname = NULL;
struct hostent *WINAPI detour_gethostbyname(const char *name)
{
    log_debug("detour_gethostbyname(%s);", name);
    return g_fp_gethostbyname(name);
}

typedef INT(WSAAPI *getaddrinfo__t)(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA *pHints, PADDRINFOA *ppResult);
getaddrinfo__t g_fp_getaddrinfo = NULL;
INT WSAAPI detour_getaddrinfo(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA *pHints, PADDRINFOA *ppResult)
{
    log_debug("detour_getaddrinfo(%s, %s, ... );", pNodeName, pServiceName);
    if ((acl_is_allowed_host(pNodeName) & ACL_DENY) == ACL_DENY)
    {
        log_debug(" >> getaddrinfo deined");
        return EAI_NONAME;
    }
    log_debug(" >> getaddrinfo passed");
    return g_fp_getaddrinfo(pNodeName, pServiceName, pHints, ppResult);
}

typedef int(WINAPI *WSAGetLastError__t)(void);
WSAGetLastError__t g_fp_WSAGetLastError = NULL;
int WINAPI detour_WSAGetLastError(void)
{
    log_debug("detour_WSAGetLastError();");
    return g_fp_WSAGetLastError();
}

typedef void(WINAPI *WSASetLastError__t)(int iError);
WSASetLastError__t g_fp_WSASetLastError = NULL;
void WINAPI detour_WSASetLastError(int iError)
{
    log_debug("detour_WSASetLastError(%d);", iError);
    return g_fp_WSASetLastError(iError);
}

typedef SOCKET(WINAPI *WSASocketW__t)(
    int af, int type, int protocol,
    LPWSAPROTOCOL_INFOW lpProtocolInfo, GROUP g, DWORD dwFlags);
WSASocketW__t g_fp_WSASocketW = NULL;
SOCKET
WINAPI detour_WSASocketW(int af, int type, int protocol, LPWSAPROTOCOL_INFOW lpProtocolInfo, GROUP g, DWORD dwFlags)
{

    log_debug("detour_WSASocketW(af:%x, type:%x, prot:%x, ... , g:%x, dwFlags:%x);",
              af, type, protocol,
              g, dwFlags);
    return g_fp_WSASocketW(af, type, protocol,
                           lpProtocolInfo, g, dwFlags);
}

typedef HANDLE(WINAPI *WSACreateEvent__t)(void);
WSACreateEvent__t g_fp_WSACreateEvent = NULL;
HANDLE WINAPI detour_WSACreateEvent(void)
{
    log_debug("detour_WSACreateEvent();");
    return g_fp_WSACreateEvent();
}

typedef BOOL(WINAPI *WSAResetEvent__t)(HANDLE hEvent);
WSAResetEvent__t g_fp_WSAResetEvent = NULL;
BOOL WINAPI detour_WSAResetEvent(HANDLE hEvent)
{
    log_debug("detour_WSAResetEvent();");
    return g_fp_WSAResetEvent(hEvent);
}

typedef int(WINAPI *WSASendTo__t)(
    SOCKET s,
    LPWSABUF lpBuffers, DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
    const struct sockaddr *lpTo, int iTolen,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
WSASendTo__t g_fp_WSASendTo = NULL;
int WINAPI detour_WSASendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr *lpTo, int iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    log_debug("detour_WSASendTo();");
    return g_fp_WSASendTo(s,
                          lpBuffers, dwBufferCount,
                          lpNumberOfBytesSent, dwFlags,
                          lpTo, iTolen,
                          lpOverlapped,
                          lpCompletionRoutine);
}

typedef int(WINAPI *WSARecvFrom__t)(
    SOCKET s,
    LPWSABUF lpBuffers, DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags,
    struct sockaddr *lpFrom, LPINT lpFromlen,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
WSARecvFrom__t g_fp_WSARecvFrom = NULL;
int WINAPI detour_WSARecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, struct sockaddr *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    log_debug("detour_WSARecvFrom();");
    return g_fp_WSARecvFrom(s,
                            lpBuffers, dwBufferCount,
                            lpNumberOfBytesRecvd, lpFlags,
                            lpFrom, lpFromlen,
                            lpOverlapped,
                            lpCompletionRoutine);
}

typedef DWORD(WINAPI *WSAWaitForMultipleEvents__t)(DWORD cEvents, const HANDLE *lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable);
WSAWaitForMultipleEvents__t g_fp_WSAWaitForMultipleEvents = NULL;
DWORD WINAPI detour_WSAWaitForMultipleEvents(
    DWORD cEvents, const HANDLE *lphEvents,
    BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable)
{
    log_debug("detour_WSAWaitForMultipleEvents();");
    return g_fp_WSAWaitForMultipleEvents(
        cEvents, lphEvents,
        fWaitAll, dwTimeout, fAlertable);
}

typedef BOOL(WINAPI *WSACloseEvent__t)(HANDLE hEvent);
WSACloseEvent__t g_fp_WSACloseEvent = NULL;
BOOL WINAPI detour_WSACloseEvent(HANDLE hEvent)
{
    log_debug("detour_WSACloseEvent();");
    return g_fp_WSACloseEvent(hEvent);
}

typedef int(WINAPI *WSACleanup__t)(void);
WSACleanup__t g_fp_WSACleanup = NULL;
int WINAPI detour_WSACleanup(void)
{
    log_debug("detour_WSACleanup()");
    return g_fp_WSACleanup();
}

void hook_reg_ws()
{
    g_h_libws2 = GetModuleHandle("ws2_32.dll");
    log_debug("h_ws2_32: %p", g_h_libws2);

    mhook_ex(L"ws2_32", "gethostbyname",
             &detour_gethostbyname, &g_fp_gethostbyname, NULL);

    mhook_ex(L"ws2_32", "getaddrinfo",
             &detour_getaddrinfo, &g_fp_getaddrinfo, NULL);

    // mhook_ex(L"ws2_32", "WSAGetLastError",
    //          &detour_WSAGetLastError, &g_fp_WSAGetLastError, NULL);

    // mhook_ex(L"ws2_32", "WSASetLastError",
    //          &detour_WSASetLastError, &g_fp_WSAGetLastError, NULL);

    // mhook_ex(L"ws2_32", "WSASocketW",
    //          &detour_WSASocketW, &g_fp_WSASocketW, NULL);

    // mhook_ex(L"ws2_32", "WSACreateEvent",
    //          &detour_WSACreateEvent, &g_fp_WSACreateEvent, NULL);

    //

    // mhook_ex(L"ws2_32", "WSAResetEvent",
    //          &detour_WSAResetEvent, &g_fp_WSAResetEvent, NULL);

    // mhook_ex(L"ws2_32", "WSASendTo",
    //          &detour_WSASendTo, &g_fp_WSASendTo, NULL);

    // mhook_ex(L"ws2_32", "WSARecvFrom",
    //          &detour_WSARecvFrom, &g_fp_WSARecvFrom, NULL);

    //

    // spammy
    // mhook_ex(L"ws2_32", "WSAWaitForMultipleEvents",
    //          &detour_WSAWaitForMultipleEvents, &g_fp_WSAWaitForMultipleEvents, NULL);

    // mhook_ex(L"ws2_32", "WSACloseEvent",
    //          &detour_WSACloseEvent, &g_fp_WSAWaitForMultipleEvents, NULL);

    // mhook_ex(L"ws2_32", "WSACleanup",
    //          &detour_WSACleanup, &g_fp_WSACleanup, NULL);
}

#endif