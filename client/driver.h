#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <locale>
#include <memory>
#include <stdexcept>

class c_driver
{
private:
	static inline __int64 (__fastcall* Original)(__int64 a1, __int64 a2);
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
public:
	static inline uint64_t targetPid;
	static inline uint64_t targetBase;

	static void initComm(LPCWSTR targetName);

	static uint64_t getBase();

	template<typename T>
	static T readMem(uint64_t Addres, SIZE_T Size = sizeof(T));

	template<typename T>
	static void writeMem(uint64_t Addres, T Buffer, SIZE_T Size = sizeof(T));
};

template<typename T>
inline T c_driver::readMem(uint64_t Addres, SIZE_T Size)
{
	T Buffer{};

	data d{};
	d.code = 74633;
	d.addr = Addres;
	d.buff = (uint64_t)&Buffer;
	d.size = Size;
	d.type = READ;
	d.target = targetPid;

	Original((__int64)&d, NULL);

	return Buffer;
}

template<typename T>
inline void c_driver::writeMem(uint64_t Addres, T Buffer, SIZE_T Size)
{
	data d{};
	d.code = 74633;
	d.addr = Addres;
	d.buff = (uint64_t)&Buffer;
	d.size = Size;
	d.type = WRITE;
	d.target = targetPid;

	Original((__int64)&d, NULL);
}
