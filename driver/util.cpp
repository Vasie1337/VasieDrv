#include "util.h"

bool util::DataCompare(const uint8_t* pData, const uint8_t* bMask, const char* szMask) {
	for (; *szMask; ++szMask, ++pData, ++bMask) if (*szMask == 'x' && *pData != *bMask) return 0;
	return (*szMask) == 0;
}

uint64_t util::FindPattern(uint64_t dwAddress, uint64_t dwLen, uint8_t* bMask, char* szMask) {
	for (uint64_t i = 0; i < dwLen; i++) if (DataCompare((uint8_t*)(dwAddress + i), bMask, szMask)) return (uint64_t)(dwAddress + i);
	return 0;
}

bool util::ClearPFN(PMDL mdl)
{
	PPFN_NUMBER mdl_pages = MmGetMdlPfnArray(mdl);
	if (!mdl_pages)
		return false;

	uint32_t mdl_page_count = ADDRESS_AND_SIZE_TO_SPAN_PAGES(MmGetMdlVirtualAddress(mdl), MmGetMdlByteCount(mdl));

	uint32_t null_pfn = 0x0;
	MM_COPY_ADDRESS source_address = { 0 };
	source_address.VirtualAddress = &null_pfn;

	for (uint32_t i = 0; i < mdl_page_count; i++)
	{
		size_t bytes = 0;
		MmCopyMemory(&mdl_pages[i], source_address, sizeof(uint32_t), MM_COPY_MEMORY_VIRTUAL, &bytes);
	}

	return true;
}

bool util::is_retop(uint8_t op)
{
	switch (op)
	{
	case 0xC2:
	case 0xC3:
	case 0xCA:
	case 0xCB:
		return true;
	}
	return false;
}

uint64_t util::scanforCodecaves(INT length)
{
	uint32_t bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

	if (!bytes)
		return 0;

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePool(NonPagedPool, bytes);

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	if (!NT_SUCCESS(status))
		return 0;

	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
	void* module_base = 0;
	void* module_size = 0;

	for (uint32_t i = 0; i < modules->NumberOfModules; i++)
	{
		if (!util::crt_strcmp(module[i].FullPathName, "\\SystemRoot\\System32\\drivers\\tcpip.sys", true))
		{
			continue;
		}

		IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)module[i].ImageBase;
		IMAGE_NT_HEADERS64* nt_headers = (IMAGE_NT_HEADERS64*)((uint8_t*)dos_header + dos_header->e_lfanew);

		uint64_t header_offset = (uint64_t)IMAGE_FIRST_SECTION(nt_headers);
		for (INT x = 0; x < nt_headers->FileHeader.NumberOfSections; ++x)
		{
			IMAGE_SECTION_HEADER* header = (IMAGE_SECTION_HEADER*)header_offset;

			if (strcmp((CHAR*)header->Name, ".text") == 0)
			{
				uint64_t start = (uint64_t)module[i].ImageBase + header->PointerToRawData;
				uint64_t size = header->SizeOfRawData;

				uint64_t match = 0;
				INT curlength = 0;
				bool ret = FALSE;

				for (uint64_t cur = start; cur < start + size; ++cur)
				{
					if (!ret && is_retop(*(uint8_t*)cur))
					{
						ret = TRUE;
					}
					else if (ret && *(uint8_t*)cur == 0xCC)
					{
						if (!match) match = cur;
						if (++curlength == length)
						{
							//DbgPrint("Name: %s\n", module[i].FullPathName);
							return match;
						}
					}
					else
					{
						match = curlength = 0;
						ret = FALSE;
					}
				}
				break;
			}

			header_offset += sizeof(IMAGE_SECTION_HEADER);
		}
	}

	if (modules)
		ExFreePool(modules);

	if (module_base <= 0)
		return 0;

	return (uint64_t)module_base;
}
