#include "driver.h"

void c_driver::initComm(LPCWSTR targetName)
{
	LoadLibraryA("user32.dll");
	LoadLibraryA("win32u.dll");
	LoadLibraryA("ntdll.dll");
	*(void**)&Original = GetProcAddress(
		GetModuleHandleA("win32u.dll"),
		"NtUserFunctionalizeDisplayConfig"
	);

	PROCESSENTRY32 entry;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	entry.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsnap, &entry)) {
		do {
			if (!lstrcmpi(entry.szExeFile, targetName)) {
				c_driver::targetPid = entry.th32ProcessID;
			}
		} while (Process32Next(hsnap, &entry));
	}
	CloseHandle(hsnap);
}

uint64_t c_driver::getBase()
{
	uint64_t Buffer{};

	data d{};
	d.code = 74633;
	d.buff = (uint64_t)&Buffer;
	d.type = BASE;
	d.target = targetPid;

	Original((__int64)&d, NULL);

	return targetBase = Buffer;
}
