#pragma once
#include <ntifs.h>
#include <windef.h>
#include <intrin.h>

#include "util.h"
#include "process.h"

class mem
{
public:
	static DWORD GetUserDirectoryTableBaseOffset();
	static ULONG_PTR GetProcessCr3(PEPROCESS pProcess);
	static ULONG_PTR GetKernelDirBase();
	static NTSTATUS ReadPhysicalAddress(PVOID TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesRead);
	static NTSTATUS WritePhysicalAddress(PVOID TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesWritten);
	static uint64_t TranslateLinearAddress(uint64_t directoryTableBase, uint64_t virtualAddress);
	static NTSTATUS ReadVirtual(uint64_t dirbase, uint64_t address, uint8_t* buffer, SIZE_T size, SIZE_T* read);
	static NTSTATUS WriteVirtual(uint64_t dirbase, uint64_t address, uint8_t* buffer, SIZE_T size, SIZE_T* written);
	static NTSTATUS ReadProcessMemory(int pid, PVOID Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* read);
	static NTSTATUS WriteProcessMemory(int pid, PVOID Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* written);
};

