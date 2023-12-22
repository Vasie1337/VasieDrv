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
	static inline __int64(__fastcall* Original)(__int64 a1, __int64 a2);
	static __int64 __fastcall Hook(hook::data* a1, __int64 a2);

public:
	static NTSTATUS init();
};

