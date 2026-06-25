//Типы данных созданы для быстрой совместимости с функциями-операциями над векторами и матрицами из openTri//

#ifndef __CLOVERTYPES_H__
#define __CLOVERTYPES_H__

#define ALIGN16 __attribute__((aligned(16)))

typedef signed int CloverSInt;
typedef unsigned int CloverUInt;

typedef signed long long CloverS64;
typedef unsigned long long CloverU64;
typedef signed long CloverS32;
typedef unsigned long CloverU32;
typedef signed short CloverS16;
typedef unsigned short CloverU16;
typedef signed char CloverS8;
typedef unsigned char CloverU8;
typedef unsigned char CloverBool;
typedef float CloverFloat;


typedef struct CloverVec2
{
	CloverFloat	x, y;
} CloverVec2, CloverVec2f;
typedef struct CloverVec2S32
{
	CloverS32		x, y;
} CloverVec2S32, CloverVec2i;
typedef struct CloverVec2U32
{
	CloverU32		x, y;
} CloverVec2U32;
typedef struct CloverVec2S16
{
	CloverS16		x, y;
} CloverVec2S16;
typedef struct CloverVec2U16
{
	CloverU16		x, y;
} CloverVec2U16;
typedef struct CloverVec2S8
{
	CloverS8		x, y;
} CloverVec2S8;
typedef struct CloverVec2U8
{
	CloverU8		x, y;
} CloverVec2U8;


typedef struct CloverVec3
{
	CloverFloat	x, y, z;
} CloverVec3, CloverVec3f;
typedef struct CloverVec3S32
{
	CloverS32		x, y, z;
} CloverVec3S32, CloverVec3i;
typedef struct CloverVec3U32
{
	CloverU32		x, y, z;
} CloverVec3U32;
typedef struct CloverVec3S16
{
	CloverS16		x, y, z;
} CloverVec3S16;
typedef struct CloverVec3U16
{
	CloverU16		x, y, z;
} CloverVec3U16;
typedef struct CloverVec3S8
{
	CloverS8		x, y, z;
} CloverVec3S8;
typedef struct CloverVec3U8
{
	CloverU8		x, y, z;
} CloverVec3U8;


typedef struct CloverVec4
{
	CloverFloat	x, y, z, w;
} CloverVec4 ALIGN16, CloverVec4f ALIGN16, CloverQuat ALIGN16;
typedef struct CloverVec4S32
{
	CloverS32		x, y, z, w;
} CloverVec4S32, CloverVec4i ALIGN16, CloverQuati ALIGN16;
typedef struct CloverVec4U32
{
	CloverU32		x, y, z, w;
} CloverVec4U32;
typedef struct CloverVec4S16
{
	CloverS16		x, y, z, w;
} CloverVec4S16;
typedef struct CloverVec4U16
{
	CloverU16		x, y, z, w;
} CloverVec4U16;
typedef struct CloverVec4S8
{
	CloverS8		x, y, z, w;
} CloverVec4S8;
typedef struct CloverVec4U8
{
	CloverU8		x, y, z, w;
} CloverVec4U8;

typedef union CloverMat3
{
	struct
	{
		CloverVec3f	x, y, z;
	};
	CloverFloat		m[9];
	CloverFloat		md[3][3];
} CloverMat3, CloverMat3f;
typedef union CloverMat4
{
	struct
	{
		CloverVec4f	x, y, z, w;
	};
	CloverFloat		m[16];
	CloverFloat		md[4][4];
} CloverMat4 ALIGN16, CloverMat4f ALIGN16;

typedef struct CloverColor3
{
	CloverFloat	r, g, b;
} CloverColor3, CloverColor3f;
typedef struct CloverColor3i
{
	CloverS32		r, g, b;
} CloverColor3i;
typedef struct CloverColor4
{
	CloverFloat	r, g, b, a;
} CloverColor4 ALIGN16, CloverColor4f ALIGN16;
typedef struct CloverColor4i
{
	CloverS32		r, g, b, a;
} CloverColor4i ALIGN16;
typedef struct CloverColor8880
{
	CloverU8		r, g, b;
} CloverColor3b, CloverColor8880;
typedef union CloverColor8888
{
	struct
	{
		CloverU8		r, g, b, a;
	};
	CloverU32			color;		// For fast r,g,b,a -> color conversion
} CloverColor4b, CloverColor8888;
typedef union CloverColor4444
{
	struct
	{
		CloverU16		r:4, g:4, b:4, a:4;
	};
	CloverU16			color;		// For fast r,g,b,a -> color conversion
} CloverColor4444;
typedef union CloverColor5551
{
	struct
	{
		CloverU16		r:5, g:5, b:5, a:1;
	};
	CloverU16			color;		// For fast r,g,b,a -> color conversion
} CloverColor5551;
typedef union CloverColor5650
{
	struct
	{
		CloverU16		r:5, g:6, b:5;
	};
	CloverU16			color;		// For fast r,g,b,a -> color conversion
} CloverColor5650;

#endif // __CloverTYPES_H__