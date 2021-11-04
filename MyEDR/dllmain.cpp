// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "minhook/include/MinHook.h"
#include "MyEDR.h"

DWORD WINAPI InitHooksThread(LPVOID param) {

    //MinHook itself requires initialisation, lets do this
    //before we hook specific API calls.
    if (MH_Initialize() != MH_OK) {
        OutputDebugString(TEXT("Failed to initalize MinHook library\n"));
        return -1;
    }

    //Now that we have initialised MinHook, lets prepare to hook OpenProcess from ntdll.dll
    MH_STATUS statusOpenProcess = MH_CreateHookApi(TEXT("kernel32"), "OpenProcess", HookedOpenProcess,
        reinterpret_cast<LPVOID*>(&pOriginalNtOpenProcess));
    //Now that we have initialised MinHook, lets prepare to hook NtProtectVirtualMemory from ntdll.dll
    MH_STATUS status = MH_CreateHookApi(TEXT("ntdll"), "NtProtectVirtualMemory", NtProtectVirtualMemory,
                                           reinterpret_cast<LPVOID*>(&pOriginalNtProtectVirtualMemory));  


    //Enable our hooks so they become active
    status = MH_EnableHook(MH_ALL_HOOKS);

    return status;
}

DWORD WINAPI RemoveHooksThread() {
    MH_STATUS status = MH_DisableHook(MH_ALL_HOOKS);
     status =   MH_RemoveHook(MH_ALL_HOOKS);

    return status;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        //We are not interested in callbacks when a thread is created
        DisableThreadLibraryCalls(hModule);

        MessageBox(NULL, L"We are in", L"MyEDR", MB_OK);

        //We need to create a thread when initialising our hooks since
        //DllMain is prone to lockups if executing code inline.
        HANDLE hThread = CreateThread(nullptr, 0, InitHooksThread, nullptr, 0, nullptr);
        if (hThread != nullptr) {
            CloseHandle(hThread);
        }
        break;
    }
    case DLL_PROCESS_DETACH:
        MessageBox(NULL, L"Bye Bye !!", L"MyEDR", MB_OK);
        RemoveHooksThread();

        break;
    }
    return TRUE;
}

