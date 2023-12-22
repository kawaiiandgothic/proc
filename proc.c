#include <Windows.h>
#include <TlHelp32.h>

#include "proc.h"

int proc_init(const char* window_name)
{
	HWND window = FindWindowA(0, window_name);
	if (!window)
		return 0;
	GetWindowThreadProcessId(window, &proc_id);
	proc_handle = OpenProcess(PROCESS_VM_READ, 0, proc_id);

	return 1;
}

size_t proc_module(const wchar_t* module_name)
{
	MODULEENTRY32 module_entry;
	module_entry.dwSize = sizeof(MODULEENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, proc_id);
	Module32First(hSnapshot, &module_entry);
	do {
		if (wcscmp(module_entry.szModule, module_name) == 0) {
			CloseHandle(hSnapshot);
			return module_entry.modBaseAddr;
		}
	} while (Module32Next(hSnapshot, &module_entry));

	CloseHandle(hSnapshot);
	return 0;
}

void* proc_read(void* address)
{
	void* buffer = 0;
	ReadProcessMemory(proc_handle, address, &buffer, sizeof(buffer), 0);
	return buffer;
}
