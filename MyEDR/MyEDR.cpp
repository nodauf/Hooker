// SylantStrike.cpp : Hooked API implementations
//

#include "pch.h"
#include "framework.h"
#include "MyEDR.h"
#include <psapi.h>
#include <string>

#include <TlHelp32.h>


//Pointer to the trampoline function used to call the original API
pNtProtectVirtualMemory pOriginalNtProtectVirtualMemory = nullptr;
pNtOpenProcess pOriginalNtOpenProcess = nullptr;


DWORD NTAPI NtProtectVirtualMemory(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG NumberOfBytesToProtect, IN ULONG NewAccessProtection, OUT PULONG OldAccessProtection) {

	//Check to see if the calling application is requesting RWX
	if ((NewAccessProtection & PAGE_EXECUTE_READWRITE) == PAGE_EXECUTE_READWRITE) {
		//It was, so notify the user of naughtly behaviour and terminate the running program
		MessageBox(nullptr, TEXT("You've been a naughty little hax0r!!!!, terminating program"), TEXT("Hax0r Detected"), MB_OK);
		//TerminateProcess(GetCurrentProcess(), 0xdead1337);
		//Unreachable code
		//return 0;
	}

	//No it wasn't, so just call the original function as normal
	return pOriginalNtProtectVirtualMemory(ProcessHandle, BaseAddress, NumberOfBytesToProtect, NewAccessProtection, OldAccessProtection);
}

DWORD NTAPI HookedOpenProcess(DWORD dwDesiredAccess, BOOL  bInheritHandle, DWORD dwProcessId)
{
	DWORD lsassPID = FindLsassPID();
    MessageBox(nullptr, TEXT("Open process hooked"), TEXT("MyEDR"), MB_OK);
	if (dwProcessId == lsassPID) {
		MessageBox(NULL, L"Mimikatz Attempt ?? Mimikatz, Are you there ??", L"EDR-Like", MB_OK);
		return NULL;
	}
	return pOriginalNtOpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
}

DWORD FindLsassPID()
{
	// Find lsass PID	

	DWORD lsassPID = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processEntry = {};
	processEntry.dwSize = sizeof(PROCESSENTRY32);
	LPCWSTR processName = L"";

	if (Process32First(snapshot, &processEntry)) {
		while (_wcsicmp(processName, L"lsass.exe") != 0) {
			Process32Next(snapshot, &processEntry);
			processName = processEntry.szExeFile;
			lsassPID = processEntry.th32ProcessID;
		}
	}
	return lsassPID;
}