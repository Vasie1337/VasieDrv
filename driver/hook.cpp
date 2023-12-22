#include "hook.h"
#include "mem.h"
#include "skCrypter.h"
#include "../3rd/CodeVirtualizer/VirtualizerSDK.h"
#include "junkcode.h"

INT64 __fastcall hook::Hook(hook::data* a1, __int64 a2)
{
    if ((!a1) || (ExGetPreviousMode() != UserMode || a1->code != 74633))
        return Original((__int64)a1, a2);

    PEPROCESS target = process::findbypid(a1->target);
    PEPROCESS client = IoGetCurrentProcess();

    size_t returnSize{};

    if (!target)
        return NULL;

    switch (a1->type)
    {
    case READ:
        mem::ReadProcessMemory(
            a1->target,
            (void*)a1->addr,
            (void*)a1->buff,
            a1->size,
            &returnSize
        );
        break;
    case WRITE:
        mem::WriteProcessMemory(
            a1->target,
            (void*)a1->addr,
            (void*)a1->buff,
            a1->size,
            &returnSize
        );
        break;
    case BASE:
        process::getbase(
            target,
            client, 
            a1->buff
        );
        break;
    }
    
    return NULL;
}

NTSTATUS hook::init()
{
    PEPROCESS process = process::findbyname(skCrypt(L"winlogon.exe"));
    if (!process)
        return STATUS_UNSUCCESSFUL;
    
    uint64_t base = process::getimage(skCrypt(L"\\SystemRoot\\System32\\win32k.sys"));
    if (!base)
        return STATUS_UNSUCCESSFUL;
    
    uint64_t uDataPtr = util::FindPattern(
        base,
        -1,
        (uint8_t*)"\x74\x1C\x4C\x8B\x54\x24\x00\x4C\x89\x54\x24\x00\x4C\x8B\x54\x24\x00\x4C\x89\x54\x24\x00\xFF\x15\x00\x00\x00\x00\xEB\x05\xB8\x00\x00\x00\x00\x48\x83\xC4\x48\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x28\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x08",
        "xxxxxx?xxxx?xxxx?xxxx?xx????xxx????xxxxxxxxxxxxxxxxxxxxxx????xxxxx"
    );
    if (!uDataPtr) 
        return STATUS_UNSUCCESSFUL;
    
    uDataPtr = (uint64_t)(uDataPtr)-0xA;
    uDataPtr = (uint64_t)uDataPtr + *(PINT)((uint8_t*)uDataPtr + 3) + 7;

    KeAttachProcess(process);
    
    *(void**)&Original = InterlockedExchangePointer((void**)uDataPtr, (void*)hook::Hook);

    KeDetachProcess();

    return STATUS_SUCCESS;
}