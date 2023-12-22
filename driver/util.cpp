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