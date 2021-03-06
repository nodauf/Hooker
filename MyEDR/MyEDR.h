// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the CYLANTSTRIKE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// CYLANTSTRIKE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CYLANTSTRIKE_EXPORTS
#define CYLANTSTRIKE_API __declspec(dllexport)
#else
#define CYLANTSTRIKE_API __declspec(dllimport)
#endif

typedef DWORD (NTAPI *pNtProtectVirtualMemory)(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG NumberOfBytesToProtect, IN ULONG NewAccessProtection, OUT PULONG OldAccessProtection);

extern pNtProtectVirtualMemory pOriginalNtProtectVirtualMemory;

typedef DWORD(NTAPI* pNtOpenProcess)(DWORD dwDesiredAccess, BOOL  bInheritHandle, DWORD dwProcessId);

extern pNtOpenProcess pOriginalNtOpenProcess;


DWORD NTAPI NtProtectVirtualMemory(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG NumberOfBytesToProtect, IN ULONG NewAccessProtection, OUT PULONG OldAccessProtection);

DWORD NTAPI  HookedOpenProcess(DWORD dwDesiredAccess, BOOL  bInheritHandle, DWORD dwProcessId);
DWORD FindLsassPID();

