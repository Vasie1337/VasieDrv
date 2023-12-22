#pragma once
#include <ntifs.h>
#include <windef.h>
#include <wdm.h>

#include "ia32.hpp"

#include "../3rd/VMProtect/VMProtectSDK.h"

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

#define to_lower(c_char) ((c_char >= 'A' && c_char <= 'Z') ? (c_char + 32) : c_char)

#define WINDOWS_1803 17134
#define WINDOWS_1809 17763
#define WINDOWS_1903 18362
#define WINDOWS_1909 18363
#define WINDOWS_2004 19041
#define WINDOWS_20H2 19569
#define WINDOWS_21H1 20180

#define PAGE_OFFSET_SIZE 12

static const uint64_t PMASK = (~0xfull << 8) & 0xfffffffffull;

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation = 0x0B
} SYSTEM_INFORMATION_CLASS,
* PSYSTEM_INFORMATION_CLASS;

typedef struct _RTL_PROCESS_MODULE_INFORMATION {
	HANDLE Section;
	void* MappedBase;
	void* ImageBase;
	uint32_t ImageSize;
	uint32_t Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR  FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES {
	uint32_t NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

typedef struct _LDR_DATA_TABLE_ENTRY {
    PVOID Reserved1[2];
    LIST_ENTRY InMemoryOrderLinks;
    PVOID Reserved2[2];
    PVOID DllBase;
    PVOID Reserved3[2];
    UNICODE_STRING FullDllName;
    BYTE Reserved4[8];
    PVOID Reserved5[3];
#pragma warning(push)
#pragma warning(disable: 4201) // we'll always use the Microsoft compiler
    union {
        ULONG CheckSum;
        PVOID Reserved6;
    } DUMMYUNIONNAME;
#pragma warning(pop)
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

//0x280 bytes (sizeof)
struct _KTHREAD
{
    struct _DISPATCHER_HEADER Header;                                       //0x0
    VOID* SListFaultAddress;                                                //0x10
    ULONGLONG QuantumTarget;                                                //0x18
    VOID* InitialStack;                                                     //0x20
    VOID* volatile StackLimit;                                              //0x24
    VOID* StackBase;                                                        //0x28
    ULONG ThreadLock;                                                       //0x2c
    volatile ULONGLONG CycleTime;                                           //0x30
    volatile ULONG HighCycleTime;                                           //0x38
    VOID* ServiceTable;                                                     //0x3c
    ULONG CurrentRunTime;                                                   //0x40
    ULONG ExpectedRunTime;                                                  //0x44
    VOID* KernelStack;                                                      //0x48
    struct _XSAVE_FORMAT* StateSaveArea;                                    //0x4c
    struct _KSCHEDULING_GROUP* volatile SchedulingGroup;                    //0x50
    uint8_t  WaitRegister[0x1];                                             //0x54
    volatile UCHAR Running;                                                 //0x55
    UCHAR Alerted[2];                                                       //0x56
    union
    {
        struct
        {
            ULONG AutoBoostActive : 1;                                        //0x58
            ULONG ReadyTransition : 1;                                        //0x58
            ULONG WaitNext : 1;                                               //0x58
            ULONG SystemAffinityActive : 1;                                   //0x58
            ULONG Alertable : 1;                                              //0x58
            ULONG UserStackWalkActive : 1;                                    //0x58
            ULONG ApcInterruptRequest : 1;                                    //0x58
            ULONG QuantumEndMigrate : 1;                                      //0x58
            ULONG UmsDirectedSwitchEnable : 1;                                //0x58
            ULONG TimerActive : 1;                                            //0x58
            ULONG SystemThread : 1;                                           //0x58
            ULONG ProcessDetachActive : 1;                                    //0x58
            ULONG CalloutActive : 1;                                          //0x58
            ULONG ScbReadyQueue : 1;                                          //0x58
            ULONG ApcQueueable : 1;                                           //0x58
            ULONG ReservedStackInUse : 1;                                     //0x58
            ULONG UmsPerformingSyscall : 1;                                   //0x58
            ULONG TimerSuspended : 1;                                         //0x58
            ULONG SuspendedWaitMode : 1;                                      //0x58
            ULONG SuspendSchedulerApcWait : 1;                                //0x58
            ULONG CetUserShadowStack : 1;                                     //0x58
            ULONG BypassProcessFreeze : 1;                                    //0x58
            ULONG Reserved : 10;                                              //0x58
        };
        LONG MiscFlags;                                                     //0x58
    };
    union
    {
        struct
        {
            ULONG ThreadFlagsSpare : 2;                                       //0x5c
            ULONG AutoAlignment : 1;                                          //0x5c
            ULONG DisableBoost : 1;                                           //0x5c
            ULONG AlertedByThreadId : 1;                                      //0x5c
            ULONG QuantumDonation : 1;                                        //0x5c
            ULONG EnableStackSwap : 1;                                        //0x5c
            ULONG GuiThread : 1;                                              //0x5c
            ULONG DisableQuantum : 1;                                         //0x5c
            ULONG ChargeOnlySchedulingGroup : 1;                              //0x5c
            ULONG DeferPreemption : 1;                                        //0x5c
            ULONG QueueDeferPreemption : 1;                                   //0x5c
            ULONG ForceDeferSchedule : 1;                                     //0x5c
            ULONG SharedReadyQueueAffinity : 1;                               //0x5c
            ULONG FreezeCount : 1;                                            //0x5c
            ULONG TerminationApcRequest : 1;                                  //0x5c
            ULONG AutoBoostEntriesExhausted : 1;                              //0x5c
            ULONG KernelStackResident : 1;                                    //0x5c
            ULONG TerminateRequestReason : 2;                                 //0x5c
            ULONG ProcessStackCountDecremented : 1;                           //0x5c
            ULONG RestrictedGuiThread : 1;                                    //0x5c
            ULONG VpBackingThread : 1;                                        //0x5c
            ULONG ThreadFlagsSpare2 : 1;                                      //0x5c
            ULONG EtwStackTraceApcInserted : 8;                               //0x5c
        };
        volatile LONG ThreadFlags;                                          //0x5c
    };
    volatile UCHAR Tag;                                                     //0x60
    UCHAR SystemHeteroCpuPolicy;                                            //0x61
    UCHAR UserHeteroCpuPolicy : 7;                                            //0x62
    UCHAR ExplicitSystemHeteroCpuPolicy : 1;                                  //0x62
    UCHAR Spare0;                                                           //0x63
    ULONG SystemCallNumber;                                                 //0x64
    VOID* FirstArgument;                                                    //0x68
    struct _KTRAP_FRAME* TrapFrame;                                         //0x6c
    union
    {
        struct _KAPC_STATE ApcState;                                        //0x70
        struct
        {
            UCHAR ApcStateFill[23];                                         //0x70
            CHAR Priority;                                                  //0x87
        };
    };
    ULONG UserIdealProcessor;                                               //0x88
    ULONG ContextSwitches;                                                  //0x8c
    volatile UCHAR State;                                                   //0x90
    CHAR Spare12;                                                           //0x91
    UCHAR WaitIrql;                                                         //0x92
    CHAR WaitMode;                                                          //0x93
    volatile LONG WaitStatus;                                               //0x94
    struct _KWAIT_BLOCK* WaitBlockList;                                     //0x98
    union
    {
        struct _LIST_ENTRY WaitListEntry;                                   //0x9c
        struct _SINGLE_LIST_ENTRY SwapListEntry;                            //0x9c
    };
    struct _DISPATCHER_HEADER* volatile Queue;                              //0xa4
    VOID* Teb;                                                              //0xa8
    ULONGLONG RelativeTimerBias;                                            //0xb0
    struct _KTIMER Timer;                                                   //0xb8
    union
    {
        struct _KWAIT_BLOCK WaitBlock[4];                                   //0xe0
        struct
        {
            UCHAR WaitBlockFill8[20];                                       //0xe0
            struct _KTHREAD_COUNTERS* ThreadCounters;                       //0xf4
        };
        struct
        {
            UCHAR WaitBlockFill9[44];                                       //0xe0
            struct _XSTATE_SAVE* XStateSave;                                //0x10c
        };
        struct
        {
            UCHAR WaitBlockFill10[68];                                      //0xe0
            VOID* volatile Win32Thread;                                     //0x124
        };
        struct
        {
            UCHAR WaitBlockFill11[88];                                      //0xe0
            ULONG WaitTime;                                                 //0x138
            union
            {
                struct
                {
                    SHORT KernelApcDisable;                                 //0x13c
                    SHORT SpecialApcDisable;                                //0x13e
                };
                ULONG CombinedApcDisable;                                   //0x13c
            };
        };
    };
    struct _LIST_ENTRY QueueListEntry;                                      //0x140
    union
    {
        volatile ULONG NextProcessor;                                       //0x148
        struct
        {
            ULONG NextProcessorNumber : 31;                                   //0x148
            ULONG SharedReadyQueue : 1;                                       //0x148
        };
    };
    LONG QueuePriority;                                                     //0x14c
    struct _KPROCESS* Process;                                              //0x150
    union
    {
        struct _GROUP_AFFINITY UserAffinity;                                //0x154
        struct
        {
            UCHAR UserAffinityFill[6];                                      //0x154
            CHAR PreviousMode;                                              //0x15a
            CHAR BasePriority;                                              //0x15b
            union
            {
                CHAR PriorityDecrement;                                     //0x15c
                struct
                {
                    UCHAR ForegroundBoost : 4;                                //0x15c
                    UCHAR UnusualBoost : 4;                                   //0x15c
                };
            };
            UCHAR Preempted;                                                //0x15d
            UCHAR AdjustReason;                                             //0x15e
            CHAR AdjustIncrement;                                           //0x15f
        };
    };
    ULONG AffinityVersion;                                                  //0x160
    union
    {
        struct _GROUP_AFFINITY Affinity;                                    //0x164
        struct
        {
            UCHAR AffinityFill[6];                                          //0x164
            UCHAR ApcStateIndex;                                            //0x16a
            UCHAR WaitBlockCount;                                           //0x16b
            ULONG IdealProcessor;                                           //0x16c
        };
    };
    ULONG ReadyTime;                                                        //0x170
    union
    {
        struct _KAPC_STATE SavedApcState;                                   //0x174
        struct
        {
            UCHAR SavedApcStateFill[23];                                    //0x174
            UCHAR WaitReason;                                               //0x18b
        };
    };
    CHAR SuspendCount;                                                      //0x18c
    CHAR Saturation;                                                        //0x18d
    USHORT SListFaultCount;                                                 //0x18e
    union
    {
        struct _KAPC SchedulerApc;                                          //0x190
        struct
        {
            UCHAR SchedulerApcFill0[1];                                     //0x190
            UCHAR ResourceIndex;                                            //0x191
        };
        struct
        {
            UCHAR SchedulerApcFill1[3];                                     //0x190
            UCHAR QuantumReset;                                             //0x193
        };
        struct
        {
            UCHAR SchedulerApcFill2[4];                                     //0x190
            ULONG KernelTime;                                               //0x194
        };
        struct
        {
            UCHAR SchedulerApcFill3[36];                                    //0x190
            struct _KPRCB* volatile WaitPrcb;                               //0x1b4
        };
        struct
        {
            UCHAR SchedulerApcFill4[40];                                    //0x190
            VOID* LegoData;                                                 //0x1b8
        };
        struct
        {
            UCHAR SchedulerApcFill5[47];                                    //0x190
            UCHAR CallbackNestingLevel;                                     //0x1bf
        };
    };
    ULONG UserTime;                                                         //0x1c0
    struct _KEVENT SuspendEvent;                                            //0x1c4
    struct _LIST_ENTRY ThreadListEntry;                                     //0x1d4
    struct _LIST_ENTRY MutantListHead;                                      //0x1dc
    UCHAR AbEntrySummary;                                                   //0x1e4
    UCHAR AbWaitEntryCount;                                                 //0x1e5
    UCHAR AbAllocationRegionCount;                                          //0x1e6
    CHAR SystemPriority;                                                    //0x1e7
    struct _KLOCK_ENTRY* LockEntries;                                       //0x1e8
    struct _SINGLE_LIST_ENTRY PropagateBoostsEntry;                         //0x1ec
    struct _SINGLE_LIST_ENTRY IoSelfBoostsEntry;                            //0x1f0
    UCHAR PriorityFloorCounts[16];                                          //0x1f4
    UCHAR PriorityFloorCountsReserved[16];                                  //0x204
    ULONG PriorityFloorSummary;                                             //0x214
    volatile LONG AbCompletedIoBoostCount;                                  //0x218
    volatile LONG AbCompletedIoQoSBoostCount;                               //0x21c
    volatile SHORT KeReferenceCount;                                        //0x220
    UCHAR AbOrphanedEntrySummary;                                           //0x222
    UCHAR AbOwnedEntryCount;                                                //0x223
    ULONG ForegroundLossTime;                                               //0x224
    union
    {
        struct _LIST_ENTRY GlobalForegroundListEntry;                       //0x228
        struct
        {
            struct _SINGLE_LIST_ENTRY ForegroundDpcStackListEntry;          //0x228
            ULONG InGlobalForegroundList;                                   //0x22c
        };
    };
    struct _KSCB* QueuedScb;                                                //0x230
    ULONGLONG NpxState;                                                     //0x238
    volatile ULONG ThreadTimerDelay;                                        //0x240
    union
    {
        volatile LONG ThreadFlags3;                                         //0x244
        struct
        {
            ULONG BamQosLevel : 8;                                            //0x244
            ULONG PpmPolicy : 2;                                              //0x244
            ULONG ThreadFlags3Reserved2 : 22;                                 //0x244
        };
    };
    VOID* volatile AbWaitObject;                                            //0x248
    ULONG ReservedPreviousReadyTimeValue;                                   //0x24c
    ULONGLONG KernelWaitTime;                                               //0x250
    ULONGLONG UserWaitTime;                                                 //0x258
    ULONG Spare29[3];                                                       //0x260
    ULONG EndPadding[5];                                                    //0x26c
};

//0x4e0 bytes (sizeof)
struct _ETHREAD
{
    struct _KTHREAD Tcb;                                                    //0x0
    union _LARGE_INTEGER CreateTime;                                        //0x280
    union
    {
        union _LARGE_INTEGER ExitTime;                                      //0x288
        struct _LIST_ENTRY KeyedWaitChain;                                  //0x288
    };
    VOID* ChargeOnlySession;                                                //0x290
    union
    {
        struct _LIST_ENTRY PostBlockList;                                   //0x294
        struct
        {
            VOID* ForwardLinkShadow;                                        //0x294
            VOID* StartAddress;                                             //0x298
        };
    };
    union
    {
        struct _TERMINATION_PORT* TerminationPort;                          //0x29c
        struct _ETHREAD* ReaperLink;                                        //0x29c
        VOID* KeyedWaitValue;                                               //0x29c
    };
    ULONG ActiveTimerListLock;                                              //0x2a0
    struct _LIST_ENTRY ActiveTimerListHead;                                 //0x2a4
    struct _CLIENT_ID Cid;                                                  //0x2ac
    union
    {
        struct _KSEMAPHORE KeyedWaitSemaphore;                              //0x2b4
        struct _KSEMAPHORE AlpcWaitSemaphore;                               //0x2b4
    };
    uint8_t ClientSecurity[0x4];                                            //0x2c8
    struct _LIST_ENTRY IrpList;                                             //0x2cc
    ULONG TopLevelIrp;                                                      //0x2d4
    struct _DEVICE_OBJECT* DeviceToVerify;                                  //0x2d8
    VOID* Win32StartAddress;                                                //0x2dc
    VOID* LegacyPowerObject;                                                //0x2e0
    struct _LIST_ENTRY ThreadListEntry;                                     //0x2e4
    struct _EX_RUNDOWN_REF RundownProtect;                                  //0x2ec
    uint8_t ThreadLock[0x4];                                               //0x2f0
    ULONG ReadClusterSize;                                                  //0x2f4
    volatile LONG MmLockOrdering;                                           //0x2f8
    union
    {
        ULONG CrossThreadFlags;                                             //0x2fc
        struct
        {
            ULONG Terminated : 1;                                             //0x2fc
            ULONG ThreadInserted : 1;                                         //0x2fc
            ULONG HideFromDebugger : 1;                                       //0x2fc
            ULONG ActiveImpersonationInfo : 1;                                //0x2fc
            ULONG HardErrorsAreDisabled : 1;                                  //0x2fc
            ULONG BreakOnTermination : 1;                                     //0x2fc
            ULONG SkipCreationMsg : 1;                                        //0x2fc
            ULONG SkipTerminationMsg : 1;                                     //0x2fc
            ULONG CopyTokenOnOpen : 1;                                        //0x2fc
            ULONG ThreadIoPriority : 3;                                       //0x2fc
            ULONG ThreadPagePriority : 3;                                     //0x2fc
            ULONG RundownFail : 1;                                            //0x2fc
            ULONG UmsForceQueueTermination : 1;                               //0x2fc
            ULONG IndirectCpuSets : 1;                                        //0x2fc
            ULONG DisableDynamicCodeOptOut : 1;                               //0x2fc
            ULONG ExplicitCaseSensitivity : 1;                                //0x2fc
            ULONG PicoNotifyExit : 1;                                         //0x2fc
            ULONG DbgWerUserReportActive : 1;                                 //0x2fc
            ULONG ForcedSelfTrimActive : 1;                                   //0x2fc
            ULONG SamplingCoverage : 1;                                       //0x2fc
            ULONG ReservedCrossThreadFlags : 8;                               //0x2fc
        };
    };
    union
    {
        ULONG SameThreadPassiveFlags;                                       //0x300
        struct
        {
            ULONG ActiveExWorker : 1;                                         //0x300
            ULONG MemoryMaker : 1;                                            //0x300
            ULONG StoreLockThread : 2;                                        //0x300
            ULONG ClonedThread : 1;                                           //0x300
            ULONG KeyedEventInUse : 1;                                        //0x300
            ULONG SelfTerminate : 1;                                          //0x300
            ULONG RespectIoPriority : 1;                                      //0x300
            ULONG ActivePageLists : 1;                                        //0x300
            ULONG SecureContext : 1;                                          //0x300
            ULONG ZeroPageThread : 1;                                         //0x300
            ULONG WorkloadClass : 1;                                          //0x300
            ULONG ReservedSameThreadPassiveFlags : 20;                        //0x300
        };
    };
    union
    {
        ULONG SameThreadApcFlags;                                           //0x304
        struct
        {
            UCHAR OwnsProcessAddressSpaceExclusive : 1;                       //0x304
            UCHAR OwnsProcessAddressSpaceShared : 1;                          //0x304
            UCHAR HardFaultBehavior : 1;                                      //0x304
            volatile UCHAR StartAddressInvalid : 1;                           //0x304
            UCHAR EtwCalloutActive : 1;                                       //0x304
            UCHAR SuppressSymbolLoad : 1;                                     //0x304
            UCHAR Prefetching : 1;                                            //0x304
            UCHAR OwnsVadExclusive : 1;                                       //0x304
            UCHAR SystemPagePriorityActive : 1;                               //0x305
            UCHAR SystemPagePriority : 3;                                     //0x305
            UCHAR AllowUserWritesToExecutableMemory : 1;                      //0x305
            UCHAR AllowKernelWritesToExecutableMemory : 1;                    //0x305
            UCHAR OwnsVadShared : 1;                                          //0x305
        };
    };
    UCHAR CacheManagerActive;                                               //0x308
    UCHAR DisablePageFaultClustering;                                       //0x309
    UCHAR ActiveFaultCount;                                                 //0x30a
    UCHAR LockOrderState;                                                   //0x30b
    ULONG PerformanceCountLowReserved;                                      //0x30c
    LONG PerformanceCountHighReserved;                                      //0x310
    ULONG AlpcMessageId;                                                    //0x314
    union
    {
        VOID* AlpcMessage;                                                  //0x318
        ULONG AlpcReceiveAttributeSet;                                      //0x318
    };
    struct _LIST_ENTRY AlpcWaitListEntry;                                   //0x31c
    LONG ExitStatus;                                                        //0x324
    ULONG CacheManagerCount;                                                //0x328
    ULONG IoBoostCount;                                                     //0x32c
    ULONG IoQoSBoostCount;                                                  //0x330
    ULONG IoQoSThrottleCount;                                               //0x334
    ULONG KernelStackReference;                                             //0x338
    struct _LIST_ENTRY BoostList;                                           //0x33c
    struct _LIST_ENTRY DeboostList;                                         //0x344
    ULONG BoostListLock;                                                    //0x34c
    ULONG IrpListLock;                                                      //0x350
    VOID* ReservedForSynchTracking;                                         //0x354
    struct _SINGLE_LIST_ENTRY CmCallbackListHead;                           //0x358
    struct _GUID* ActivityId;                                               //0x35c
    struct _SINGLE_LIST_ENTRY SeLearningModeListHead;                       //0x360
    VOID* VerifierContext;                                                  //0x364
    VOID* AdjustedClientToken;                                              //0x368
    VOID* WorkOnBehalfThread;                                               //0x36c
    uint8_t PropertySet[0x7];                                               //0x370
    VOID* PicoContext;                                                      //0x37c
    ULONG UserFsBase;                                                       //0x380
    ULONG UserGsBase;                                                       //0x384
    struct _THREAD_ENERGY_VALUES* EnergyValues;                             //0x388
    union
    {
        ULONG SelectedCpuSets;                                              //0x38c
        ULONG* SelectedCpuSetsIndirect;                                     //0x38c
    };
    struct _EJOB* Silo;                                                     //0x390
    struct _UNICODE_STRING* ThreadName;                                     //0x394
    VOID* SparePointer;                                                     //0x398
    ULONG LastExpectedRunTime;                                              //0x39c
    ULONG HeapData;                                                         //0x3a0
    struct _LIST_ENTRY OwnerEntryListHead;                                  //0x3a4
    ULONG DisownedOwnerEntryListLock;                                       //0x3ac
    struct _LIST_ENTRY DisownedOwnerEntryListHead;                          //0x3b0
    uint8_t LockEntries[0x120];                                             //0x3b8
    VOID* CmDbgInfo;                                                        //0x4d8
};

typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid specific
    WORD   e_res2[10];                  // Reserved words
    LONG   e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
    WORD    Machine;
    WORD    NumberOfSections;
    DWORD   TimeDateStamp;
    DWORD   PointerToSymbolTable;
    DWORD   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;
    WORD    Characteristics;
} IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16

typedef struct _IMAGE_DATA_DIRECTORY {
    DWORD   VirtualAddress;
    DWORD   Size;
} IMAGE_DATA_DIRECTORY, * PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
    WORD        Magic;
    uint8_t     MajorLinkerVersion;
    uint8_t     MinorLinkerVersion;
    DWORD       SizeOfCode;
    DWORD       SizeOfInitializedData;
    DWORD       SizeOfUninitializedData;
    DWORD       AddressOfEntryPoint;
    DWORD       BaseOfCode;
    uint64_t    ImageBase;
    DWORD       SectionAlignment;
    DWORD       FileAlignment;
    WORD        MajorOperatingSystemVersion;
    WORD        MinorOperatingSystemVersion;
    WORD        MajorImageVersion;
    WORD        MinorImageVersion;
    WORD        MajorSubsystemVersion;
    WORD        MinorSubsystemVersion;
    DWORD       Win32VersionValue;
    DWORD       SizeOfImage;
    DWORD       SizeOfHeaders;
    DWORD       CheckSum;
    WORD        Subsystem;
    WORD        DllCharacteristics;
    ULONGLONG   SizeOfStackReserve;
    uint64_t    SizeOfStackCommit;
    uint64_t    SizeOfHeapReserve;
    uint64_t    SizeOfHeapCommit;
    DWORD       LoaderFlags;
    DWORD       NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, * PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_NT_HEADERS64 {
    DWORD Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, * PIMAGE_NT_HEADERS64;

#define IMAGE_SIZEOF_SHORT_NAME              8

typedef struct _IMAGE_SECTION_HEADER {
    uint8_t    Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
        DWORD   PhysicalAddress;
        DWORD   VirtualSize;
    } Misc;
    DWORD   VirtualAddress;
    DWORD   SizeOfRawData;
    DWORD   PointerToRawData;
    DWORD   PointerToRelocations;
    DWORD   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    DWORD   Characteristics;
} IMAGE_SECTION_HEADER, * PIMAGE_SECTION_HEADER;

#define IMAGE_FIRST_SECTION( ntheader ) ((PIMAGE_SECTION_HEADER)        \
    ((uintptr_t)(ntheader) +                                            \
     FIELD_OFFSET( IMAGE_NT_HEADERS64, OptionalHeader ) +                 \
     ((ntheader))->FileHeader.SizeOfOptionalHeader   \
    ))

EXTERN_C NTSTATUS NTAPI MmCopyVirtualMemory(PEPROCESS SourceProcess, void* SourceAddress, PEPROCESS TargetProcess, void* TargetAddress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize);
EXTERN_C NTSTATUS ZwQuerySystemInformation(uint32_t InfoClass, void* Buffer, uint32_t Length, uint32_t* ReturnLength);
EXTERN_C void* NTAPI PsGetProcessSectionBaseAddress(PEPROCESS Process);
