#ifndef KDLIBMODE

#include <Windows.h>
#include <string>
#include <vector>
#include <filesystem>

#include "kdmapper.hpp"

HANDLE iqvw64e_device_handle;


LONG WINAPI SimplestCrashHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
	if (ExceptionInfo && ExceptionInfo->ExceptionRecord)
		Log(L"[!!] Crash at addr 0x" << ExceptionInfo->ExceptionRecord->ExceptionAddress << L" by 0x" << std::hex << ExceptionInfo->ExceptionRecord->ExceptionCode << std::endl);
	else
		Log(L"[!!] Crash" << std::endl);

	if (iqvw64e_device_handle)
		intel_driver::Unload(iqvw64e_device_handle);

	return EXCEPTION_EXECUTE_HANDLER;
}

int wmain(const int argc, wchar_t** argv) {
	SetUnhandledExceptionFilter(SimplestCrashHandler);

	bool free = false;
	bool mdlMode = true;
	bool indPagesMode = false;
	bool passAllocationPtr = false;

	if (free) {
		Log(L"[+] Free pool memory after usage enabled" << std::endl);
	}

	if (mdlMode) {
		Log(L"[+] Mdl memory usage enabled" << std::endl);
	}

	if (indPagesMode) {
		Log(L"[+] Allocate Independent Pages mode enabled" << std::endl);
	}

	if (passAllocationPtr) {
		Log(L"[+] Pass Allocation Ptr as first param enabled" << std::endl);
	}

	int drvIndex = -1;
	for (int i = 1; i < argc; i++) {
		if (std::filesystem::path(argv[i]).extension().string().compare(".sys") == 0) {
			drvIndex = i;
			break;
		}
	}

	if (drvIndex <= 0) {
		return -1;
	}

	const std::wstring driver_path = argv[drvIndex];

	if (!std::filesystem::exists(driver_path)) {
		Log(L"[-] File " << driver_path << L" doesn't exist" << std::endl);
		return -1;
	}

	iqvw64e_device_handle = intel_driver::Load();

	if (iqvw64e_device_handle == INVALID_HANDLE_VALUE)
		return -1;

	std::vector<uint8_t> raw_image = { 0 };
	if (!utils::ReadFileToMemory(driver_path, &raw_image)) {
		Log(L"[-] Failed to read image to memory" << std::endl);
		intel_driver::Unload(iqvw64e_device_handle);
		return -1;
	}

	kdmapper::AllocationMode mode = kdmapper::AllocationMode::AllocatePool;

	if (mdlMode && indPagesMode) {
		Log(L"[-] Too many allocation modes" << std::endl);
		intel_driver::Unload(iqvw64e_device_handle);
		return -1;
	}
	else if (mdlMode) {
		mode = kdmapper::AllocationMode::AllocateMdl;
	}
	else if (indPagesMode) {
		mode = kdmapper::AllocationMode::AllocateIndependentPages;
	}

	NTSTATUS exitCode = 0;
	if (!kdmapper::MapDriver(iqvw64e_device_handle, raw_image.data(), 0, 0, free, true, mode, passAllocationPtr, 0, &exitCode)) {
		Log(L"[-] Failed to map " << driver_path << std::endl);
		intel_driver::Unload(iqvw64e_device_handle);
		return -1;
	}

	if (!intel_driver::Unload(iqvw64e_device_handle)) {
		Log(L"[-] Warning failed to fully unload vulnerable driver " << std::endl);
	}
	Log(L"[+] success" << std::endl);
}

#endif