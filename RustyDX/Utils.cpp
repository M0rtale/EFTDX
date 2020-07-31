#include "Utils.h"
#include "Memory.h"
#include <d3d11.h>
#include <sstream>

//defining data
int s_width = 2560;
int s_height = 1440;
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
HWND hWnd;
HWND twnd;
MARGINS  margin = { 0,0,s_width,s_height };
LPD3DXFONT pFont;
//ImFont* m_pTabs = 0;
ID3DXLine* d3dLine;
int Height = GetSystemMetrics(SM_CYSCREEN);
int Width = GetSystemMetrics(SM_CXSCREEN);

ImFont* m_pDefault = NULL;
ImFont* m_pChinese = NULL;
float _Test_Type_Float = 0.f;

//static FCameraCacheEntry Ocameracache;

int ActorIds[4];
int uaz[3];
int dacia[4];
int motorbike[5];
int buggy[3];
int boat = 0;
int itemtype[2];
//

ULONG _Test_ULONG = 0;
//ULONG64 _Test_ULONG64 = 0;
Vector3 _Test_Vector3;

// CONVERTERS
std::string VariableText(const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);

	char buffer[2048];
	vsprintf(buffer, format, argptr);

	va_end(argptr);

	return buffer;
}

D3DXMATRIX views = D3DXMATRIX();
void updateCamera(ULONG64 camera)
{
	Memory::_This->Read_Memory(camera + ENTITYPTR_MATRIX, &views, sizeof(D3DXMATRIX));
	camera = camera + 0;
}

Vector3 WorldToScreen(Vector3 origin)
{
	Vector3 out;
	D3DXMATRIX temp;

	D3DXMatrixTranspose(&temp, &views);

	D3DXVECTOR3 translationVector = D3DXVECTOR3(temp._41, temp._42, temp._43);
	D3DXVECTOR3 up = D3DXVECTOR3(temp._21, temp._22, temp._23);
	D3DXVECTOR3 right = D3DXVECTOR3(temp._11, temp._12, temp._13);

	float w = D3DXVec3Dot(&translationVector, (D3DXVECTOR3*)&origin) + temp._44;

	if (w < 0.098f)
		return Vector3(0,0,0);

	float y = D3DXVec3Dot(&up, (D3DXVECTOR3*)&origin) + temp._24;
	float x = D3DXVec3Dot(&right, (D3DXVECTOR3*)&origin) + temp._14;

	out.x = (int)(Width / 2) * (1.f + x / w);
	out.y = (int)(Height / 2) * (1.f - y / w);

	return out;
}


D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}


D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

void FillRGB(float x, float y, float w, float h, int r, int g, int b, int a)
{
	/*D3DXVECTOR2 vLine[2];

	d3dLine->SetWidth(w);

	vLine[0].x = x + w / 2;
	vLine[0].y = y;
	vLine[1].x = x + w / 2;
	vLine[1].y = y + h;

	d3dLine->Begin();
	d3dLine->Draw(vLine, 2, D3DCOLOR_RGBA(r, g, b, a));
	d3dLine->End();*/

	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), D3DCOLOR_ARGB(a, r, g, b));

}

void DrawBar(int x, int y, float value)
{
	float l, r, g;

	g = value * 2.55;
	r = 255 - g;
	l = value / 3;
	FillRGB(x - (l / 2) - 1, y - 1, l + 2 + 13, 5, 0, 0, 0, 255);
	FillRGB(x - (l / 2), y, l + 13, 3, 0, g, r, 255);
}

struct TransformAccessReadOnly
{
	uint64_t pTransformData;
};

struct TransformData
{
	uint64_t pTransformArray;
	uint64_t pTransformIndices;
};


struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct Matrix34
{
	Vector4 vec0;
	Vector4 vec1;
	Vector4 vec2;
};


Vector3 get_bone_pos(uint64_t pTransform)
{
	if (!pTransform) {
		return Vector3();
	}

	__m128 result;

	const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
	const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
	const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

	TransformAccessReadOnly pTransformAccessReadOnly;
	Memory::_This->Read_Memory(pTransform + 0x38, &pTransformAccessReadOnly, sizeof(TransformAccessReadOnly));

	unsigned int index;
	Memory::_This->Read_Memory(pTransform + 0x40, &index, sizeof(unsigned int));

	TransformData transformData;
	Memory::_This->Read_Memory(pTransformAccessReadOnly.pTransformData + 0x18, &transformData, sizeof(TransformData));

	if (transformData.pTransformArray && transformData.pTransformIndices)
	{
		//result = *(__m128*)((ULONGLONG)transformData.pTransformArray + 0x30 * index);
		Memory::_This->Read_Memory(transformData.pTransformArray + 0x30 * index, &result, sizeof(__m128));

		//int transformIndex = *(int*)((ULONGLONG)transformData.pTransformIndices + 0x4 * index);
		int transformIndex;
		Memory::_This->Read_Memory(transformData.pTransformIndices + 0x4 * index, &transformIndex, sizeof(int));

		int pSafe = 0;
		while (transformIndex >= 0 && pSafe++ < 200)
		{

			//Matrix34 matrix34 = *(Matrix34*)((ULONGLONG)transformData.pTransformArray + 0x30 * transformIndex);
			Matrix34 matrix34;
			Memory::_This->Read_Memory(transformData.pTransformArray + 0x30 * transformIndex, &matrix34, sizeof(Matrix34));

			__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));	// xxxx
			__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));	// yyyy
			__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));	// zwxy
			__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));	// wzyw
			__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));	// zzzz
			__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));	// yxwy
			__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

			result = _mm_add_ps(
				_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
						tmp7)), *(__m128*)(&matrix34.vec0));

			//transformIndex = *(int*)((ULONGLONG)transformData.pTransformIndices + 0x4 * transformIndex);
			Memory::_This->Read_Memory(transformData.pTransformIndices + 0x4 * transformIndex, &transformIndex, sizeof(int));
		}
	}

	return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}