#include "hook.h"

INT64 __fastcall hook::Hook(hook::data* a1, void* a2, void* a3, void* a4, void* a5, void* a6)
{
    if ((!a1) || (ExGetPreviousMode() != UserMode || a1->code != 74633))
        return Original(a1, a2, a3, a4, a5, a6);

    PEPROCESS target = process::findbypid(a1->target);

    if (!target)
        return NULL;

    switch (a1->type)
    {
    case READ:
        process::read(target, a1->addr, IoGetCurrentProcess(), a1->buff, a1->size);
        break;
    case WRITE:
        process::write(IoGetCurrentProcess(), a1->buff, target, a1->addr, a1->size);
        break;
    case BASE:
        process::getbase(target, IoGetCurrentProcess(), a1->buff);
        break;
    }

    return NULL;
}

bool hook::setupCodecave(uint64_t* Codecave)
{
    uint8_t shellcode[] = { 0x48, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x89, 0xC3, 0x31, 0xC0, 0xFF, 0xE3 };
    *(void**)&shellcode[2] = reinterpret_cast<void*>(&Hook);

    uint64_t address = util::scanforCodecaves(sizeof(shellcode));
    if (!address)
        return false;

    uint32_t cr0 = __readcr0();
    cr0 = cr0 &~ 0x10000;
    __writecr0(cr0);

    RtlCopyMemory((void*)address, &shellcode, sizeof(shellcode));

    cr0 = __readcr0();
    cr0 = cr0 | 0x10000;
    __writecr0(cr0);

    *Codecave = address;

    return true;
}

NTSTATUS hook::init() 
{
    PEPROCESS process = process::findbyname(L"winlogon.exe");
    if (!process)
        return STATUS_UNSUCCESSFUL;
    
    uint64_t base = process::getimage(L"\\SystemRoot\\System32\\win32kbase.sys");
    if (!base)
        return STATUS_UNSUCCESSFUL;
    
    uint64_t uDataPtr = util::FindPattern(
        base,
        -1,
        (uint8_t*)"\x74\x2E\x8B\x84\x24\x00\x00\x00\x00",
        "xxxxx????"
    );
    if (!uDataPtr) 
        return STATUS_UNSUCCESSFUL;
    
    uDataPtr = (uint64_t)(uDataPtr)-0xA;
    uDataPtr = (uint64_t)uDataPtr + *(PINT)((uint8_t*)uDataPtr + 3) + 7;

    uint64_t Codecave;
    if (!hook::setupCodecave(&Codecave))
        return STATUS_UNSUCCESSFUL;

    KeAttachProcess(process);
    
    *(void**)&Original = InterlockedExchangePointer((void**)uDataPtr, (void*)Codecave);

    KeDetachProcess();

    return STATUS_SUCCESS;
}