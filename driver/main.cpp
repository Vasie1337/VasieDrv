#include "hook.h"

EXTERN_C NTSTATUS DriverEntry(PMDL mdlPtr, void* a2) {
	if (!util::ClearPFN(mdlPtr))
		return STATUS_FAILED_DRIVER_ENTRY;

	return hook::init();
}