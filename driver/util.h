#pragma once
#include "nt.h"

class util {
public:
	static bool DataCompare(const uint8_t* pData, const uint8_t* bMask, const char* szMask);
	static uint64_t FindPattern(uint64_t dwAddress, uint64_t dwLen, uint8_t* bMask, char* szMask);
	static bool ClearPFN(PMDL mdl);
	static bool is_retop(uint8_t op);
	static uint64_t scanforCodecaves(INT length);

	template <typename str_type, typename str_type_2>
	static bool crt_strcmp(str_type str, str_type_2 in_str, bool two);
};

template<typename str_type, typename str_type_2>
inline bool util::crt_strcmp(str_type str, str_type_2 in_str, bool two)
{
	if (!str || !in_str)
		return false;

	wchar_t c1, c2;

	do
	{
		c1 = *str++; c2 = *in_str++;
		c1 = to_lower(c1); c2 = to_lower(c2);

		if (!c1 && (two ? !c2 : 1))
			return true;

	} while (c1 == c2);

	return false;
}
