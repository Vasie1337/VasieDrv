#include "process.h"

PEPROCESS process::findbypid(uint64_t pid) {
	PEPROCESS targetProcess = NULL;
	PsLookupProcessByProcessId((HANDLE)pid, &targetProcess);
	
	return targetProcess;
}

PEPROCESS process::findbyname(wchar_t* name) {
	CHAR image_name[15]{};
	PEPROCESS sys_process = PsInitialSystemProcess;
	PEPROCESS cur_entry = sys_process;

	do
	{
		RtlCopyMemory((void*)(&image_name), (void*)((uint64_t)cur_entry + 0x5A8), sizeof(image_name));

		if (util::crt_strcmp(image_name, name, true))
		{
			DWORD active_threads;
			RtlCopyMemory((void*)&active_threads, (void*)((uint64_t)cur_entry + 0x5F0), sizeof(active_threads));

			if (active_threads)
				return cur_entry;
		}

		PLIST_ENTRY list = (PLIST_ENTRY)((uint64_t)(cur_entry) + 0x448);
		cur_entry = (PEPROCESS)((uint64_t)list->Flink - 0x448);

	} while (cur_entry != sys_process);

	return 0;
}

uint64_t process::getimage(wchar_t* name)
{
	uint32_t bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

	if (!bytes) 
		return 0;

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x504D5448);

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	if (!NT_SUCCESS(status)) 
		return 0;

	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
	void* module_base = 0;
	void* module_size = 0;

	for (uint32_t i = 0; i < modules->NumberOfModules; i++)
	{
		if (util::crt_strcmp(module[i].FullPathName, name, true))
		{
			module_base = module[i].ImageBase;
			module_size = (void*)module[i].ImageSize;
			break;
		}
	}

	if (modules) 
		ExFreePoolWithTag(modules, 0);

	if (module_base <= 0) 
		return 0;

	return (uint64_t)module_base;
}

void process::getbase(PEPROCESS src, PEPROCESS dest, uint64_t buf) {
	SIZE_T returnSize = 0;
	uint64_t uBase = (uint64_t)PsGetProcessSectionBaseAddress(src);
	MmCopyVirtualMemory(IoGetCurrentProcess(), &uBase, dest, (void*)buf, sizeof(uBase), 0, &returnSize);
}

SIZE_T process::read(PEPROCESS src, uint64_t adr, PEPROCESS dest, uint64_t buf, SIZE_T size) {
	SIZE_T returnSize = 0;
	MmCopyVirtualMemory(src, (void*)adr, dest, (void*)buf, size, NULL, &returnSize);

	return returnSize;
}

SIZE_T process::write(PEPROCESS src, uint64_t buf, PEPROCESS dest, uint64_t adr, SIZE_T size) {
	SIZE_T returnSize = 0;
	MmCopyVirtualMemory(src, (void*)buf, dest, (void*)adr, size, NULL, &returnSize);

	return returnSize;
}