#include "Memory.h"
#include "Addrs-Offset.h"
#include <fstream>


Memory *Memory::_This = nullptr;

static DWORD GetPIDForProcess(const char *Str)
{
	BOOL            working = 0;
	PROCESSENTRY32 lppe = { 0 };
	DWORD            targetPid = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot)
	{
		lppe.dwSize = sizeof(lppe);
		working = Process32First(hSnapshot, &lppe);
		while (working)
		{
			if (std::string(lppe.szExeFile) == Str)
			{
				targetPid = lppe.th32ProcessID;
				break;
			}
			working = Process32Next(hSnapshot, &lppe);
		}
	}

	CloseHandle(hSnapshot);
	return targetPid;
}


bool Memory::Init()
{
	LONG64 PID = GetPIDForProcess("EscapeFromTarkov.exe");
	if (PID == 0)
	{
		MessageBoxA(NULL, "ERROR-Process!", NULL, NULL);
		exit(0);
	}
	_Handle = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
	return false;
}


bool Memory::Read_Memory(ULONG64 Addr, void *Buffer, ULONG64 Size)
{
	return ReadProcessMemory(_Handle, (LPVOID)Addr, Buffer, Size, NULL);
}

bool Memory::Write_Memory(ULONG64 Addr, void *Buffer, ULONG64 Size)
{
	return WriteProcessMemory(_Handle, (LPVOID)Addr, &Buffer, Size, NULL);
}

void Memory::Dump_Names()
{
	ULONG64 _Test_ULONG64 = 0;
	INT16 _Test_INT16 = 0;

	std::ofstream out("Dump.txt");

	ULONG64 ObjectManager = Memory::_This->Read_Memory(Memory::_This->Base + EFT_OBJECTMANAGER, _Test_ULONG64);
	ULONG64 taggedObjectsPtr = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_TAGGEDLIST, _Test_ULONG64);
	ULONG64 firstObject = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_TAGGEDLISTFIRST, _Test_ULONG64);

	while (firstObject != taggedObjectsPtr)
	{
		ULONG64 BaseObject = taggedObjectsPtr;
		ULONG64 GameObject = Memory::_This->Read_Memory(taggedObjectsPtr + BASEENT_GAMEOBJECT, _Test_ULONG64);
		ULONG64 tag = Memory::_This->Read_Memory(GameObject + GAMEOBJECT_TAG, _Test_INT16);
		ULONG64 NamePtr = Memory::_This->Read_Memory(GameObject + GAMEOBJECT_OBJECTNAMEPTR, _Test_ULONG64);
		char world_name_char[100] = { 0 };
		Memory::_This->Read_Memory(NamePtr, world_name_char, 50);

		out << world_name_char << " has tag " << tag << " is at position " << BaseObject << std::endl;

		taggedObjectsPtr = Memory::_This->Read_Memory(taggedObjectsPtr + TAGGEDLIST_NEXTTAGGEDENT, _Test_ULONG64);
	}

	out << "\n\n\n\n now continuing active \n\n\n\n";

	ObjectManager = Memory::_This->Read_Memory(Memory::_This->Base + EFT_OBJECTMANAGER, _Test_ULONG64);
	taggedObjectsPtr = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_ACTIVELIST, _Test_ULONG64);
	firstObject = Memory::_This->Read_Memory(ObjectManager + OBJECTMANAGER_ACTIVELISTLAST, _Test_ULONG64);

	while (firstObject != taggedObjectsPtr)
	{
		ULONG64 BaseObject = taggedObjectsPtr;
		ULONG64 GameObject = Memory::_This->Read_Memory(taggedObjectsPtr + BASEENT_GAMEOBJECT, _Test_ULONG64);
		ULONG64 tag = Memory::_This->Read_Memory(GameObject + GAMEOBJECT_TAG, _Test_INT16);
		ULONG64 NamePtr = Memory::_This->Read_Memory(GameObject + GAMEOBJECT_OBJECTNAMEPTR, _Test_ULONG64);
		char world_name_char[100] = { 0 };
		Memory::_This->Read_Memory(NamePtr, world_name_char, 50);

		out << world_name_char << " has tag " << tag << " is at position " << BaseObject << std::endl;

		taggedObjectsPtr = Memory::_This->Read_Memory(taggedObjectsPtr + TAGGEDLIST_NEXTTAGGEDENT, _Test_ULONG64);
	}

	out.close();
}

void* Memory::Alloc_Memory(ULONG64 Size)
{
	return false;
}

ULONG64 Memory::Get_Model_Addr()
{
	return Base;
}