#pragma once
#include "util.h"

class process {
public:
	static PEPROCESS findbypid(uint64_t pid);
	static PEPROCESS findbyname(wchar_t* name);

	static uint64_t getimage(wchar_t* name);
	static void getbase(PEPROCESS src, PEPROCESS dest, uint64_t buf);
	static SIZE_T read(PEPROCESS src, uint64_t adr, PEPROCESS dest, uint64_t buf, SIZE_T size);
	static SIZE_T write(PEPROCESS src, uint64_t buf, PEPROCESS dest, uint64_t adr, SIZE_T size);
};

