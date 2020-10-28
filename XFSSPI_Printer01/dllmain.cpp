// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include "pch.h"
#include "SPIPrinter.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_h_services.clear();

        g_lock_mutex = CreateMutex(NULL, FALSE, NULL);
        g_wfs_queue_mutex = CreateMutex(NULL, FALSE, NULL);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        CloseHandle(g_lock_mutex);
        CloseHandle(g_wfs_queue_mutex);
        break;
    }
    return TRUE;
}

