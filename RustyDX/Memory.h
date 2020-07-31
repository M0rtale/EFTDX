#pragma once

#include <windows.h>
#include <TlHelp32.h>
#include <iostream>


class Memory
{
public:
	Memory()
	{
		_This = this;
	}
	~Memory() = default;
public:
	bool Init(); // TODO: Add your own Read Write
	bool Read_Memory(ULONG64 Addr, void *Buffer, ULONG64 Size);
	bool Write_Memory(ULONG64 Addr, void *Buffer, ULONG64 Size);
	void Dump_Names();
	void* Alloc_Memory(ULONG64 Size);
	ULONG64 Get_Model_Addr();
	ULONG64 Base = 0;
public:
	static Memory *_This;
public:
	template<typename T>
	T Read_Memory(ULONG64 Addr, T t)
	{
		T ret;
		ReadProcessMemory(_Handle, (LPVOID)Addr, &ret, sizeof(T), NULL);
		return ret;
	}

	template<typename T>
	T Read_Memory(ULONG64 Addr)
	{
		T ret;
		ReadProcessMemory(_Handle, (LPVOID)Addr, &ret, sizeof(T), NULL);
		return ret;
	}
private:
	HANDLE _Handle;

};

