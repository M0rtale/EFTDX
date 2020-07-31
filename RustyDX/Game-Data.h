#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <vector>

#include "Addrs-Offset.h"
#include "Memory.h"
#include "Utils.h"

//all IDA reversal functions, just in case the assembly code isn't reliable enough.

#define BYTEn(x, n) (*((BYTE *)&(x) + n))
#define WORDn(x, n) (*((WORD *)&(x) + n))
#define DWORDn(x, n) (*((DWORD *)&(x) + n))

#define IDA_LOBYTE(x) BYTEn(x, 0)
#define IDA_LOWORD(x) WORDn(x, 0)
#define IDA_LODWORD(x) DWORDn(x, 0)
#define IDA_HIBYTE(x) BYTEn(x, 1)
#define IDA_HIWORD(x) WORDn(x, 1)
#define IDA_HIDWORD(x) DWORDn(x, 1)

#define BYTE1(x) BYTEn(x, 1)
#define BYTE2(x) BYTEn(x, 2)
#define WORD1(x) WORDn(x, 1)
#define DWORD1(x) DWORDn(x, 1)
#define WORD2(x)   WORDn(x,  2)

#define OFFSET_IDCALC              0x3E20
static uint8_t __ROL1__(uint8_t x, unsigned int count) {
	count %= 8;
	return (x << count) | (x >> (8 - count));
}
static uint16_t __ROL2__(uint16_t x, unsigned int count) {
	count %= 16;
	return (x << count) | (x >> (16 - count));
}
static uint32_t __ROL4__(uint32_t x, unsigned int count) {
	count %= 32;
	return (x << count) | (x >> (32 - count));
}
static uint64_t __ROL8__(uint64_t x, unsigned int count) {
	count %= 64;
	return (x << count) | (x >> (64 - count));
}
static uint8_t __ROR1__(uint8_t x, unsigned int count) {
	count %= 8;
	return (x << (8 - count)) | (x >> count);
}
static uint16_t __ROR2__(uint16_t x, unsigned int count) {
	count %= 16;
	return (x << (16 - count)) | (x >> count);
}
static uint32_t __ROR4__(uint32_t x, unsigned int count) {
	count %= 32;
	return (x << (32 - count)) | (x >> count);
}
static uint64_t __ROR8__(uint64_t x, unsigned int count) {
	count %= 64;
	return (x << (64 - count)) | (x >> count);
}

class Game_Data
{
public:
	Game_Data()
	{
		_Memory = new Memory();
	}
	~Game_Data() = default;
public:
	bool Hook_Start();
public:
	Memory * _Memory = nullptr;
	bool _overrides = true;
private:
	ULONG64 _PID = 0;
public:
	ULONG64 _Rusty_Addr = 0;
	static ULONG64 _Camera_Addr;
	static std::vector<Player> _Player_Vector;
	static std::vector<Player> _Obj_Vector;
	static std::vector<Player> _Animal_Vector;
	static std::vector<Player> _Turret_Vector;
	static std::vector<Player> _Vehicle_Vector;
	ULONG64 *_Networked_List;
	ULONG64 _Networked_size;
	ULONG64 BaseNetworkable;
	static Player localplayer;
	void Get_Data(bool box, bool animal, bool turret, bool vehicle);
	ULONG _Test_ULONG = 0;
	float _Test_Float = 0;
	ULONG64 _Test_ULONG64 = 0;
	INT16 _Test_INT16 = 0;
	Vector3 _Test_Vector3 = Vector3();
	char* _Test_char = 0;
	INT _Test_INT = 0;
	D3DXMATRIX _Test_MATRIX = D3DXMATRIX();
};

