#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
#include <list>
#include <d3d9.h>
#include <d3dx9.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>
#include "Addrs-Offset.h"


#define M_PI	3.14159265358979323846264338327950288419716939937510

using namespace std;

//Vector3
class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	float x;
	float y;
	float z;

	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator/(Vector3 v)
	{
		return Vector3(x / v.x, y / v.y, z / v.z);
	}
};

class Player
{
public:
	ULONG64 BaseObject;
	ULONG64 GameObject;
	ULONG64 Unknown;
	ULONG64 VisualState;
	float health;
	Vector3 position;
	ULONG64 head;
	ULONG64 chest;
	std::string name;
	std::string objName;
	INT16 tag;
	INT flag;

};

enum Bone_List : int {
	head = 133,
	chest = 29
};
