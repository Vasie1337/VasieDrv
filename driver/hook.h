#pragma once
#include <ntifs.h>
#include <windef.h>
#include <intrin.h>

#include "util.h"
#include "process.h"

class hook {
	enum type {
		READ,
		WRITE,
		BASE,
	};
	struct data {
		int code;

		uint64_t addr;
		uint64_t buff;
		SIZE_T size;

		uint64_t target;

		enum type type;
	};
	static inline INT64(__fastcall* Original)(void*, void*, void*, void*, void*, void*);
	static INT64 __fastcall Hook(hook::data* a1, void* a2, void* a3, void* a4, void* a5, void* a6);

	static bool setupCodecave(uint64_t* Codecave);
public:
	static NTSTATUS init();
};

