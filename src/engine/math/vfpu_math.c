#include "vfpu_math.h"
#define __cloverasm__ asm volatile
// --------------------------------------------------------
// Скалярные операции
// --------------------------------------------------------

float vfpu_absf(float x)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "vabs.s   S000, S000\n"
        "mfv      %0, S000\n"
    : "=r"(result) : "r"(x));
    return result;
}

float vfpu_ceilf(float x)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "vf2iu.s  S000, S000, 0\n"
        "vi2f.s   S000, S000, 0\n"
        "mfv      %0, S000\n"
    : "=r"(result) : "r"(x));
    return result;
}

float vfpu_floorf(float x)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "vf2id.s  S000, S000, 0\n"
        "vi2f.s   S000, S000, 0\n"
        "mfv      %0, S000\n"
    : "=r"(result) : "r"(x));
    return result;
}

float vfpu_roundf(float x)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "vf2in.s  S000, S000, 0\n"
        "vi2f.s   S000, S000, 0\n"
        "mfv      %0, S000\n"
    : "=r"(result) : "r"(x));
    return result;
}

float vfpu_truncf(float x)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "vf2iz.s  S000, S000, 0\n"
        "vi2f.s   S000, S000, 0\n"
        "mfv      %0, S000\n"
    : "=r"(result) : "r"(x));
    return result;
}

// --------------------------------------------------------
// Тригонометрия
// --------------------------------------------------------

float vfpu_sinf(float radians)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_2_PI\n"
        "vmul.s  S000, S000, S001\n"
        "vsin.s  S000, S000\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(radians));
    return result;
}

float vfpu_cosf(float radians)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_2_PI\n"
        "vmul.s  S000, S000, S001\n"
        "vcos.s  S000, S000\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(radians));
    return result;
}

void vfpu_sincosf(float radians, float *s, float *c)
{
    __asm__ volatile (
        "mtv      %2, S002\n"
        "vcst.s   S003, VFPU_2_PI\n"
        "vmul.s   S002, S002, S003\n"
        "vrot.p   C000, S002, [s, c]\n"
        "mfv      %0, S000\n"
        "mfv      %1, S001\n"
    : "=r"(*s), "=r"(*c): "r"(radians));
}

float vfpu_tanf(float radians)
{
    float result;
    // result = sin(x)/cos(x);
    __asm__ volatile (
        "mtv      %1, S000\n"
        "vcst.s   S001, VFPU_2_PI\n"
        "vmul.s   S000, S000, S001\n"
        "vrot.p   C002, S000, [s, c]\n"
        "vdiv.s   S000, S002, S003\n"
        "mfv      %0, S000\n"
    : "=r"(result) : "r"(radians));
    return result;
}

float vfpu_asinf(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_PI_2\n"
        "vasin.s S000, S000\n"
        "vmul.s  S000, S000, S001\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

float vfpu_acosf(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_PI_2\n"
        "vasin.s S000, S000\n"
        "vocp.s  S000, S000\n"
        "vmul.s  S000, S000, S001\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

float vfpu_atanf(float x)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "vmul.s   S001, S000, S000\n"
        "vadd.s   S001, S001, S001[1]\n"
        "vrsq.s   S001, S001\n"
        "vmul.s   S000, S000, S001\n"
        "vasin.s  S000, S000\n"
        "vcst.s   S001, VFPU_PI_2\n"
        "vmul.s   S000, S000, S001\n"
        "mfv      %0, S000\n"
    : "=r"(result) : "r"(x));
    return result;
}

float vfpu_atan2f(float y, float x)
{
    float r;
    if (vfpu_absf(x) >= vfpu_absf(y))
    {
        r = vfpu_atanf(y/x);
        if (x < 0.0f)
            r += (y >= 0.0f ? PI : -PI);
    }
    else
    {
        r = -vfpu_atanf(x/y);
        r += (y < 0.0f ? -PI_2 : PI_2);
    }
    return r;
}

// --------------------------------------------------------
// Гиперболические функции (Заменены C-реализации на VFPU)
// --------------------------------------------------------

float vfpu_sinhf(float x)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "vcst.s   S001, VFPU_LN2\n"
        "vrcp.s   S001, S001\n"
        "vmov.s   S002, S000[|x|]\n"
        "vcmp.s   NE, S000, S002\n"
        "vmul.s   S002, S001, S002\n"
        "vexp2.s  S002, S002\n"
        "vrcp.s   S003, S002\n"
        "vsub.s   S002, S002, S003\n"
        "vmul.s   S002, S002, S002[1/2]\n"
        "vcmov.s  S002, S002[-x], 0\n"
        "mfv      %0, S002\n"
    : "=r"(result) : "r"(x));
    return result;
}

float vfpu_coshf(float x)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "vcst.s   S001, VFPU_LN2\n"
        "vrcp.s   S001, S001\n"
        "vmov.s   S002, S000[|x|]\n"
        "vmul.s   S002, S001, S002\n"
        "vexp2.s  S002, S002\n"
        "vrcp.s   S003, S002\n"
        "vadd.s   S002, S002, S003\n"
        "vmul.s   S002, S002, S002[1/2]\n"
        "mfv      %0, S002\n"
    : "=r"(result) : "r"(x));
    return result;
}

float vfpu_tanhf(float x)
{
    float result;
    __asm__ volatile (
        "mtv      %0, S000\n"
        "vadd.s   S000, S000, S000\n"
        "vcst.s   S001, VFPU_LN2\n"
        "vrcp.s   S001, S001\n"
        "vmul.s   S000, S000, S001\n"
        "vexp2.s  S000, S000\n"
        "vone.s   S001\n"
        "vbfy1.p  C002, C000\n"
        "vdiv.s   S000, S003, S002\n"
        "mfv      %0, S000\n"
    : "=r"(result): "r"(x));
    return result;
}

// --------------------------------------------------------
// Степени и логарифмы (Заменены C-реализации на VFPU)
// --------------------------------------------------------

float vfpu_sqrtf(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vsqrt.s S000, S000\n"
        "mfv     %0, S000\n"
    : "=r"(result) : "r"(x));
    return result;
}

float vfpu_invsqrtf(float x)
{
    float result;
    __asm__ volatile (
        "mtv        %1, S000\n"
        "vrsq.s     S000, S000\n"
        "mfv        %0, S000\n"
    : "=r"(result): "r"(x));
    return result;
}

float vfpu_powf(float x, float y)
{
    float result;
    // result = exp2f(y * log2f(x));
    __asm__ volatile (
        "mtv      %1, S000\n"
        "mtv      %2, S001\n"
        "vlog2.s  S001, S001\n"
        "vmul.s   S000, S000, S001\n"
        "vexp2.s  S000, S000\n"
        "mfv      %0, S000\n"
    : "=r"(result) : "r"(y), "r"(x)); // Обратите внимание: порядок регистров
    return result;
}

float vfpu_pow2f(float x)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "vexp2.s  S000, S000\n"
        "mfv      %0, S000\n"
    : "=r"(result) : "r"(x));
    return result;
}

float vfpu_expf(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_LN2\n"
        "vrcp.s  S001, S001\n"
        "vmul.s  S000, S000, S001\n"
        "vexp2.s S000, S000\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

float vfpu_logf(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_LOG2E\n"
        "vrcp.s  S001, S001\n"
        "vlog2.s S000, S000\n"
        "vmul.s  S000, S000, S001\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

float vfpu_log2f(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vlog2.s S000, S000\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

float vfpu_log10f(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_LOG2TEN\n"
        "vrcp.s  S001, S001\n"
        "vlog2.s S000, S000\n"
        "vmul.s  S000, S000, S001\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

// --------------------------------------------------------
// Остаток и мин/макс
// --------------------------------------------------------

float vfpu_fmodf(float x, float y)
{
    float result;
    __asm__ volatile (
        "mtv       %2, S001\n"
        "mtv       %1, S000\n"
        "vrcp.s    S002, S001\n"
        "vmul.s    S003, S000, S002\n"
        "vf2iz.s   S002, S003, 0\n"
        "vi2f.s    S003, S002, 0\n"
        "vmul.s    S003, S003, S001\n"
        "vsub.s    S000, S000, S003\n"
        "mfv       %0, S000\n"
    : "=r"(result) : "r"(x), "r"(y));
    return result;
}

float vfpu_maxf(float x, float y)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "mtv      %2, S001\n"
        "vmax.s   S002, S000, S001\n"
        "mfv      %0, S002\n"
    : "=r"(result) : "r"(x), "r"(y));
    return result;
}

float vfpu_minf(float x, float y)
{
    float result;
    __asm__ volatile (
        "mtv      %1, S000\n"
        "mtv      %2, S001\n"
        "vmin.s   S002, S000, S001\n"
        "mfv      %0, S002\n"
    : "=r"(result) : "r"(x), "r"(y));
    return result;
}

// --------------------------------------------------------
// Утилиты
// --------------------------------------------------------

float vfpu_deg2rad(float degrees)
{
    return degrees * DEG_TO_RAD;
}

float vfpu_rad2deg(float radians)
{
    return radians * RAD_TO_DEG;
}

void vfpu_srand(unsigned int seed)
{
    __asm__ volatile (
        "mtv %0, S000\n"
        "vrnds.s S000"
        : : "r"(seed));
}

float vfpu_randf(float min, float max)
{
    float result;
    // pgeMath реализация: генерация нормализованного float (0..1) и масштабирование
    __asm__ volatile (
        "mtv      %1, S000\n"
        "mtv      %2, S001\n"
        "vsub.s   S001, S001, S000\n" // range = max - min
        "vrndf1.s S002\n"             // генерирует 1..2
        "vone.s   S003\n"
        "vsub.s   S002, S002, S003\n" // преобразуем в 0..1
        "vmul.s   S001, S002, S001\n" // * range
        "vadd.s   S000, S000, S001\n" // + min
        "mfv      %0, S000\n"
        : "=r"(result) : "r"(min), "r"(max));
    return result;
}

int vfpu_randi(float min, float max)
{
    return (int)vfpu_randf(min, max);
}

// --------------------------------------------------------
// Векторные операции (добавлены из pgeMath)
// --------------------------------------------------------

ScePspFVector3 *vfpu_vec_add(ScePspFVector3 *result, const ScePspFVector3 *vec1, const ScePspFVector3 *vec2)
{   
    __asm__ volatile(
        ".set           push\n"
        ".set           noreorder\n"
        "lv.s           S000, 0 + %1\n"
        "lv.s           S001, 4 + %1\n"
        "lv.s           S002, 8 + %1\n"
        "lv.s           S010, 0 + %2\n"
        "lv.s           S011, 4 + %2\n"
        "lv.s           S012, 8 + %2\n"
        "vadd.t         C000, C000, C010\n"
        "sv.s           S000, 0 + %0\n"
        "sv.s           S001, 4 + %0\n"
        "sv.s           S002, 8 + %0\n"
        ".set           pop\n"
        : "=m"(*result) : "m"(*vec1), "m"(*vec2));
    
    return result;
}

ScePspFVector3 *vfpu_vec_sub(ScePspFVector3 *result, const ScePspFVector3 *vec1, const ScePspFVector3 *vec2)
{   
    __asm__ volatile(
        ".set           push\n"
        ".set           noreorder\n"
        "lv.s           S000, 0 + %1\n"
        "lv.s           S001, 4 + %1\n"
        "lv.s           S002, 8 + %1\n"
        "lv.s           S010, 0 + %2\n"
        "lv.s           S011, 4 + %2\n"
        "lv.s           S012, 8 + %2\n"
        "vsub.t         C000, C000, C010\n"
        "sv.s           S000, 0 + %0\n"
        "sv.s           S001, 4 + %0\n"
        "sv.s           S002, 8 + %0\n"
        ".set           pop\n"
        : "=m"(*result) : "m"(*vec1), "m"(*vec2));
    
    return result;
}

int vfpu_vec_compare(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2)
{
    return(vec1->x == vec2->x && vec1->y == vec2->y && vec1->z == vec2->z);
}

ScePspFVector3 *vfpu_vec_scale(ScePspFVector3 *result, const ScePspFVector3 *vec, float scalar)
{
    __asm__ volatile (
        ".set           push\n"
        ".set           noreorder\n"
        "mfc1           $8,   %2\n"
        "mtv            $8,   S010\n"
        "lv.s           S000, 0 + %1\n"
        "lv.s           S001, 4 + %1\n"
        "lv.s           S002, 8 + %1\n"
        "vscl.t         C000, C000, S010\n"
        "sv.s           S000, 0 + %0\n"
        "sv.s           S001, 4 + %0\n"
        "sv.s           S002, 8 + %0\n"
        ".set           pop\n"
        : "=m"(*result) : "m"(*vec), "f"(scalar): "$8");
        
    return result;
}

float vfpu_vec_dot(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2)
{
    float result;
    
    __asm__ volatile (
        ".set           push\n"
        ".set           noreorder\n"
        "lv.s           S000, 0 + %1\n"
        "lv.s           S001, 4 + %1\n"
        "lv.s           S002, 8 + %1\n"
        "lv.s           s010, 0 + %2\n"
        "lv.s           s011, 4 + %2\n"
        "lv.s           s012, 8 + %2\n"
        "vdot.t         S000, C000, C010\n"
        "sv.s           S000, %0\n"
        ".set           pop\n"
        : "=m"(result) : "m"(*vec1), "m"(*vec2));

    return result;
}

float vfpu_vec_length(const ScePspFVector3 *vec)
{
    float result;
    
    __asm__ volatile (
        ".set           push\n"
        ".set           noreorder\n"
        "lv.s           S000, 0 + %1\n"
        "lv.s           S001, 4 + %1\n"
        "lv.s           S002, 8 + %1\n"
        "vdot.t         S000, C000, C000\n"
        "vsqrt.s        S000, S000\n"
        "sv.s           S000, %0\n"
        ".set           pop\n"
        : "=m"(result) : "m"(*vec));

    return result;
}

ScePspFVector3 *vfpu_vec_normalize(ScePspFVector3 *result, const ScePspFVector3 *vec)
{
    __asm__ volatile(
        ".set           push\n"
        ".set           noreorder\n"
        "lv.s           S000, 0 + %1\n"
        "lv.s           S001, 4 + %1\n"
        "lv.s           S002, 8 + %1\n"
        "vdot.t         S010, C000, C000\n"
        "vzero.s        S011\n"
        "vcmp.s         EZ, S010\n"
        "vrsq.s         S010, S010\n"
        "vcmovt.s       S010, S011, 0\n"
        "vscl.t         C000[-1:1,-1:1,-1:1], C000, S010\n"
        "sv.s           S000, 0 + %0\n"
        "sv.s           S001, 4 + %0\n"
        "sv.s           S002, 8 + %0\n"
        ".set           pop\n"
        : "=m"(*result) : "m"(*vec));
        
    return result;
}
// Реализации векторно-матричных операций используя VFPU (Большинство реализаций вырезано из openTri, отредактировано и оптимизировано)
// NOTE: This library depends on CloverVec4f and CloverMatrix4f being aligned on 16 bytes!

CloverVec2f* CloverVec2Set( CloverVec2f* a, const CloverFloat x, const CloverFloat y )
{
	a->x = x;
	a->y = y;
	return a;
}

CloverVec3f* CloverVec3Set( CloverVec3f* a, const CloverFloat x, const CloverFloat y, const CloverFloat z )
{
	a->x = x;
	a->y = y;
	a->z = z;
	return a;
}

CloverVec4f* CloverVec4Set( CloverVec4f* a, const CloverFloat x, const CloverFloat y, const CloverFloat z, const CloverFloat w )
{
	a->x = x;
	a->y = y;
	a->z = z;
	a->w = w;
	return a;
}

CloverVec4f* CloverVec4Set3( CloverVec4f* a, const CloverFloat x, const CloverFloat y, const CloverFloat z )
{
	a->x = x;
	a->y = y;
	a->z = z;
	a->w = 1;
	return a;
}

CloverColor4f* CloverColor4Set( CloverColor4f* c, const CloverFloat r, const CloverFloat g, const CloverFloat b, const CloverFloat a )
{
	c->r = r;
	c->g = g;
	c->b = b;
	c->a = a;
	return c;
}

CloverColor4f* CloverColor4Set3( CloverColor4f* c, const CloverFloat r, const CloverFloat g, const CloverFloat b )
{
	c->r = r;
	c->g = g;
	c->b = b;
	c->a = 1;
	return c;
}

CloverColor4f* CloverColor4From4i( CloverColor4f* a, CloverColor4i* b )
{
	CloverFloat f = 1.0f / 255.0f;
	a->r = b->r*f;
	a->g = b->g*f;
	a->b = b->b*f;
	a->a = b->a*f;
	return a;
}

CloverColor4f* CloverColor4From8888( CloverColor4f* a, CloverColor8888* b )
{
	CloverFloat f = 1.0f / 255.0f;
	a->r = b->r*f;
	a->g = b->g*f;
	a->b = b->b*f;
	a->a = b->a*f;
	return a;
}

CloverVec2i* CloverVec2Ceil( CloverVec2i* a, const CloverVec2f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"vf2id.p		c000, c000, 0\n"		// c000 = ceil(c000 * 1)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec2i* CloverVec2Trunc( CloverVec2i* a, const CloverVec2f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"vf2iz.p		c000, c000, 0\n"		// c000 = trunc(c000 * 1)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec2i* CloverVec2Round( CloverVec2i* a, const CloverVec2f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"vf2in.p		c000, c000, 0\n"		// c000 = round(c000 * 1)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec2i* CloverVec2Floor( CloverVec2i* a, const CloverVec2f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"vf2iu.p		c000, c000, 0\n"		// c000 = floor(c000 * 1)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}


CloverVec3i* CloverVec3Ceil( CloverVec3i* a, const CloverVec3f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"lv.s			s002, 8 + %1\n"			// s002 = b->z
		"vf2id.t		c000, c000, 0\n"		// c000 = ceil(c000 * 1)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec3i* CloverVec3Trunc( CloverVec3i* a, const CloverVec3f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"lv.s			s002, 8 + %1\n"			// s002 = b->z
		"vf2iz.t		c000, c000, 0\n"		// c000 = trunc(c000 * 1)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec3i* CloverVec3Round( CloverVec3i* a, const CloverVec3f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"lv.s			s002, 8 + %1\n"			// s002 = b->z
		"vf2in.t		c000, c000, 0\n"		// c000 = round(c000 * 1)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec3i* CloverVec3Floor( CloverVec3i* a, const CloverVec3f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"lv.s			s002, 8 + %1\n"			// s002 = b->z
		"vf2iu.t		c000, c000, 0\n"		// c000 = floor(c000 * 1)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}


CloverVec4i* CloverVec4Ceil( CloverVec4i* a, const CloverVec4f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *b
		"vf2id.q		c000, c000, 0\n"		// c000 = ceil(c000 * 1)
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec4i* CloverVec4Trunc( CloverVec4i* a, const CloverVec4f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *b
		"vf2iz.q		c000, c000, 0\n"		// c000 = trunc(c000 * 1)
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec4i* CloverVec4Round( CloverVec4i* a, const CloverVec4f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *b
		"vf2in.q		c000, c000, 0\n"		// c000 = round(c000 * 1)
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec4i* CloverVec4Floor( CloverVec4i* a, const CloverVec4f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *b
		"vf2iu.q		c000, c000, 0\n"		// c000 = floor(c000 * 1)
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}



CloverVec2f* CloverVec2i2f( CloverVec2f* a, const CloverVec2i* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s000 = b->x
		"lv.s			s011, 4 + %1\n"			// s001 = b->y
		"vi2f.p			c000, c010, 0\n"		// c000 = c010
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return(a);
}

CloverVec3f* CloverVec3i2f( CloverVec3f* a, const CloverVec3i* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s000 = b->x
		"lv.s			s011, 4 + %1\n"			// s001 = b->y
		"lv.s			s012, 8 + %1\n"			// s002 = b->z
		"vi2f.t			c000, c010, 0\n"		// c000 = c010
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return(a);
}

CloverVec4f* CloverVec4i2f( CloverVec4f* a, const CloverVec4i* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q 			c010, %1\n"				// c010 = *b
		"vi2f.q			c000, c010, 0\n"		// c000 = c010
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return(a);
}


CloverVec2f* CloverVec2Rndn( CloverVec2f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vrndf2.p		c000\n"					// c000 = rnd(2.0, 4.0)
		"vsub.p			c000, c000, c000[3,3]\n"// c000 = | -1.0 ... 1.0 |
		"vdot.p			s010, c000, c000\n"		// s010 = x*x + y*y
		"vrsq.s			s010, s010\n"			// s010 = 1.0 / s010
		"vscl.p			c000[-1:1,-1:1], c000, s010\n"			// c000 = c000 / s010
		"sv.s			s000, %0\n"
		"sv.s			s001, 4 + %0\n"
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);
	return(a);
}


CloverVec3f* CloverVec3Rndn( CloverVec3f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vrndf2.t		c000\n"					// c000 = rnd(2.0, 4.0)
		"vsub.t			c000, c000, c000[3,3,3]\n"	// c000 = | -1.0 ... 1.0 |
		"vdot.t			s010, c000, c000\n"		// s010 = x*x + y*y + z*z
		"vrsq.s			s010, s010\n"			// s010 = 1.0 / s010
		"vscl.t			c000[-1:1,-1:1,-1:1], c000, s010\n"			// c000 = c000 / s010
		"sv.s			s000, 0 + %0\n"
		"sv.s			s001, 4 + %0\n"
		"sv.s			s002, 8 + %0\n"
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);
	return(a);
}


CloverVec4f* CloverVec4Rndn( CloverVec4f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vrndf2.q		c000\n"					// c000 = rnd(2.0, 4.0)
		"vsub.q			c000, c000, c000[3,3,3,3]\n"// c000 = | -1.0 ... 1.0 |
		"vdot.q			s010, c000, c000\n"		// s010 = x*x + y*y + z*z + w*w
		"vrsq.s			s010, s010\n"			// s010 = 1.0 / s010
		"vscl.q			c000[-1:1,-1:1,-1:1,-1:1], c000, s010\n"		// c000 = c000 / s010
		"sv.q			c000, %0\n"
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);
	return(a);
}


CloverVec4f* CloverVec4Rndn3( CloverVec4f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vrndf2.q		c000\n"					// c000 = rnd(2.0, 4.0)
		"vsub.q			c000, c000, c000[3,3,3,3]\n"// c000 = | -1.0 ... 1.0 |
		"vdot.t			s010, c000, c000\n"		// s010 = x*x + y*y + z*z
		"vrsq.s			s010, s010\n"			// s010 = 1.0 / s010
		"vscl.t			c000[-1:1,-1:1,-1:1], c000, s010\n"		// c000 = c000 / s010
		"sv.q			c000, %0\n"
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);
	return(a);
}


CloverVec2f* CloverVec2Rnd2( CloverVec2f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vrndf2.p		c000\n"					// c000 = rnd(2.0, 4.0)
		"vsub.p			c000, c000, c000[3,3]\n"// c000 = | -1.0 ... 1.0 |
		"sv.s			s000, %0\n"
		"sv.s			s001, 4 + %0\n"
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);
	return(a);
}


CloverVec3f* CloverVec3Rnd2( CloverVec3f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vrndf2.t		c000\n"					// c000 = rnd(2.0, 4.0)
		"vsub.t			c000, c000, c000[3,3,3]\n"	// c000 = | -1.0 ... 1.0 |
		"sv.s			s000, 0 + %0\n"
		"sv.s			s001, 4 + %0\n"
		"sv.s			s002, 8 + %0\n"
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);
	return(a);
}


CloverVec4f* CloverVec4Rnd2( CloverVec4f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vrndf2.q		c000\n"					// c000 = rnd(2.0, 4.0)
		"vsub.q			c000, c000, c000[3,3,3,3]\n"// c000 = | -1.0 ... 1.0 |
		"sv.q			c000, %0\n"
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);
	return(a);
}


CloverVec2f* CloverVec2Rnd( CloverVec2f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vone.p			c010\n"
		"vrndf1.p		c000\n"					// c000 = rnd(1.0, 2.0)
		"vsub.p			c000, c000, c010\n"		// c000 = | 0.0 ... 1.0 |
		"sv.s			s000, %0\n"
		"sv.s			s001, 4 + %0\n"
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);
	return(a);
}


CloverVec3f* CloverVec3Rnd( CloverVec3f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vone.t			c010\n"
		"vrndf1.t		c000\n"					// c000 = rnd(1.0, 2.0)
		"vsub.t			c000, c000, c010\n"		// c000 = | 0.0 ... 1.0 |
		"sv.s			s000, 0 + %0\n"
		"sv.s			s001, 4 + %0\n"
		"sv.s			s002, 8 + %0\n"
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);
	return(a);
}


CloverVec4f* CloverVec4Rnd( CloverVec4f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vone.q			c010\n"
		"vrndf1.q		c000\n"					// c000 = rnd(1.0, 2.0)
		"vsub.q			c000, c000, c010\n"// c000 = | 0.0 ... 1.0 |
		"sv.q			c000, %0\n"
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);
	return(a);
}


CloverVec2f* CloverVec2Add( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"vadd.p			c000, c010, c020\n"		// c000 = c010 + c020
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec3f* CloverVec3Add( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s012, 8 + %1\n"			// s012 = b->z
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"lv.s			s022, 8 + %2\n"			// s022 = c->z
		"vadd.t			c000, c010, c020\n"		// c000 = c010 + c020
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec4f* CloverVec4Add( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q 			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vadd.q			c000, c010, c020\n"		// c000 = c010 + c020
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec4f* CloverVec4Add3( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q 			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vadd.t			c010, c010, c020\n"		// c010 = c010 + c020
		"sv.q			c010, %0\n"				// *a = c010
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec2f* CloverVec2Sub( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"vsub.p			c000, c010, c020\n"		// c000 = c010 - c020
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec3f* CloverVec3Sub( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s012, 8 + %1\n"			// s012 = b->z
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"lv.s			s022, 8 + %2\n"			// s022 = c->z
		"vsub.t			c000, c010, c020\n"		// c000 = c010 - c020
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec4f* CloverVec4Sub( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q 			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vsub.q			c000, c010, c020\n"		// c000 = c010 - c020
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec4f* CloverVec4Sub3( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q 			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vsub.t			c010, c010, c020\n"		// c010 = c010 - c020
		"sv.q			c010, %0\n"				// *a = c010
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec2f* CloverVec2Mul( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"vmul.p			c000, c010, c020\n"		// c000 = c010 * c020
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec3f* CloverVec3Mul( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s012, 8 + %1\n"			// s012 = b->z
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"lv.s			s022, 8 + %2\n"			// s022 = c->z
		"vmul.t			c000, c010, c020\n"		// c000 = c010 * c020
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec4f* CloverVec4Mul( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q 			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vmul.q			c000, c010, c020\n"		// c000 = c010 * c020
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec4f* CloverVec4Mul3( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q 			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vmul.t			c010, c010, c020\n"		// c000 = c010 * c020
		"sv.q			c010, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec2f* CloverVec2Div( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"vdiv.p			c000, c010, c020\n"		// c000 = c010 / c020
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec3f* CloverVec3Div( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s012, 8 + %1\n"			// s012 = b->z
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"lv.s			s022, 8 + %2\n"			// s022 = c->z
		"vdiv.t			c000, c010, c020\n"		// c000 = c010 / c020
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec4f* CloverVec4Div( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q 			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vdiv.q			c000, c010, c020\n"		// c000 = c010 / c020
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}

CloverVec4f* CloverVec4Div3( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q 			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vdiv.t			c010, c010, c020\n"		// c000 = c010 / c020
		"sv.q			c010, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}


// FIXME: pointer aliasing problem with sCloverct-aliasing rules on GCC!
CloverVec2f* CloverVec2Neg(CloverVec2f *a, const CloverVec2f *b)
{
	CloverVec2i* ai = (CloverVec2i*)a;
	const CloverVec2i* bi = (const CloverVec2i*)b;

	ai->x = bi->x ^ 0x80000000U;
	ai->y = bi->y ^ 0x80000000U;
	
	return (a);
}

CloverVec3f* CloverVec3Neg(CloverVec3f *a, const CloverVec3f *b)
{
	CloverVec3i* ai = (CloverVec3i*)a;
	const CloverVec3i* bi = (const CloverVec3i*)b;

	ai->x = bi->x ^ 0x80000000U;
	ai->y = bi->y ^ 0x80000000U;
	ai->z = bi->z ^ 0x80000000U;
	
	return (a);
}

CloverVec4f* CloverVec4Neg(CloverVec4f* a, const CloverVec4f* b)
{
	CloverVec4i* ai = (CloverVec4i*)a;
	const CloverVec4i* bi = (const CloverVec4i*)b;

	ai->x = bi->x ^ 0x80000000U;
	ai->y = bi->y ^ 0x80000000U;
	ai->z = bi->z ^ 0x80000000U;
	ai->w = bi->w ^ 0x80000000U;
	
	return (a);
}

CloverVec4f* CloverVec4Neg3(CloverVec4f* a, const CloverVec4f* b)
{
	CloverVec4i* ai = (CloverVec4i*)a;
	const CloverVec4i* bi = (const CloverVec4i*)b;

	ai->x = bi->x ^ 0x80000000U;
	ai->y = bi->y ^ 0x80000000U;
	ai->z = bi->z ^ 0x80000000U;
	
	return (a);
}


CloverVec2f* CloverVec2Abs(CloverVec2f *a, const CloverVec2f *b)
{
	CloverVec2i* ai = (CloverVec2i*)a;
	const CloverVec2i* bi = (const CloverVec2i*)b;

	ai->x = bi->x & 0x7FFFFFFFU;
	ai->y = bi->y & 0x7FFFFFFFU;

	return (a);
}

CloverVec3f* CloverVec3Abs(CloverVec3f *a, const CloverVec3f *b)
{
	CloverVec3i* ai = (CloverVec3i*)a;
	const CloverVec3i* bi = (const CloverVec3i*)b;

	ai->x = bi->x & 0x7FFFFFFFU;
	ai->y = bi->y & 0x7FFFFFFFU;
	ai->z = bi->z & 0x7FFFFFFFU;

	return (a);
}

CloverVec4f* CloverVec4Abs(CloverVec4f* a, const CloverVec4f* b)
{
	CloverVec4i* ai = (CloverVec4i*)a;
	const CloverVec4i* bi = (const CloverVec4i*)b;

	ai->x = bi->x & 0x7FFFFFFFU;
	ai->y = bi->y & 0x7FFFFFFFU;
	ai->z = bi->z & 0x7FFFFFFFU;
	ai->w = bi->w & 0x7FFFFFFFU;
	
	return (a);
}

CloverVec4f* CloverVec4Abs3(CloverVec4f* a, const CloverVec4f* b)
{
	CloverVec4i* ai = (CloverVec4i*)a;
	const CloverVec4i* bi = (const CloverVec4i*)b;

	ai->x = bi->x & 0x7FFFFFFFU;
	ai->y = bi->y & 0x7FFFFFFFU;
	ai->z = bi->z & 0x7FFFFFFFU;
	
	return (a);
}


CloverVec2f* CloverVec2Clamp(CloverVec2f *a, const CloverVec2f *b, CloverFloat min, CloverFloat max)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %2\n"				// t0 = min
		"mfc1			$9,   %3\n"				// t1 = max
		"mtv			$8,   s010\n"			// s010 = t0 = min
		"mtv			$9,   s011\n"			// s011 = t1 = max
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"vmax.p			c000, c000, c010[X,X]\n"	// c000 = max(c000, [s010,s010,s010,s010])
		"vmin.p			c000, c000, c010[Y,Y]\n"	// c000 = min(c000, [s011,s011,s011,s011])
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "f"(min), "f"(max)
		: "$8", "$9"
	);
	return (a);
}

CloverVec2f* CloverVec2Min(CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"vmin.p			c000, c010, c020\n"		// c000 = min(c010, c020)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);

	return (a);
}

CloverVec2f* CloverVec2Max(CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"vmax.p			c000, c010, c020\n"		// c000 = max(c010, c020)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);

	return (a);
}


CloverVec3f* CloverVec3Clamp(CloverVec3f *a, const CloverVec3f *b, CloverFloat min, CloverFloat max)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %2\n"				// t0 = min
		"mfc1			$9,   %3\n"				// t1 = max
		"mtv			$8,   s010\n"			// s010 = t0 = min
		"mtv			$9,   s011\n"			// s011 = t1 = max
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"lv.s			s002, 8 + %1\n"			// s002 = b->z
		"vmax.t			c000, c000, c010[X,X,X]\n"	// c000 = max(c000, [s010,s010,s010,s010])
		"vmin.t			c000, c000, c010[Y,Y,Y]\n"	// c000 = min(c000, [s011,s011,s011,s011])
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "f"(min), "f"(max)
		: "$8", "$9"
	);
	return (a);
}

CloverVec3f* CloverVec3Min(CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s012, 8 + %1\n"			// s012 = b->z
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"lv.s			s022, 8 + %2\n"			// s022 = c->z
		"vmin.t			c000, c010, c020\n"		// c000 = min(c010, c020)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);

	return (a);
}

CloverVec3f* CloverVec3Max(CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s012, 8 + %1\n"			// s012 = b->z
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"lv.s			s022, 8 + %2\n"			// s022 = c->z
		"vmax.t			c000, c010, c020\n"		// c000 = max(c010, c020)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);

	return (a);
}

CloverVec4f* CloverVec4Clamp(CloverVec4f* a, const CloverVec4f* b, CloverFloat min, CloverFloat max)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %2\n"				// t0 = min
		"mfc1			$9,   %3\n"				// t1 = max
		"mtv			$8,   s010\n"			// s010 = t0 = min
		"mtv			$9,   s011\n"			// s011 = t1 = max
		"lv.q			c000, %1\n"				// c000 = *b
		"vmax.q			c000, c000, c010[X,X,X,X]\n"	// c000 = max(c000, [s010,s010,s010,s010])
		"vmin.q			c000, c000, c010[Y,Y,Y,Y]\n"	// c000 = min(c000, [s011,s011,s011,s011])
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "f"(min), "f"(max)
		: "$8", "$9"
	);
	return (a);
}

CloverVec4f* CloverVec4Clamp3(CloverVec4f* a, const CloverVec4f* b, CloverFloat min, CloverFloat max)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %2\n"				// t0 = min
		"mfc1			$9,   %3\n"				// t1 = max
		"mtv			$8,   s010\n"			// s010 = t0 = min
		"mtv			$9,   s011\n"			// s011 = t1 = max
		"lv.q			c000, %1\n"				// c000 = *b
		"vmax.t			c000, c000, c010[X,X,X]\n"	// c000 = max(c000, [s010,s010,s010])
		"vmin.t			c000, c000, c010[Y,Y,Y]\n"	// c000 = min(c000, [s011,s011,s011])
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "f"(min), "f"(max)
		: "$8", "$9"
	);
	return (a);
}

CloverVec4f* CloverVec4Min(CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vmin.q			c000, c010, c020\n"		// c000 = min(c010, c020)
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);

	return (a);
}

CloverVec4f* CloverVec4Max(CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vmax.q			c000, c010, c020\n"		// c000 = max(c010, c020)
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);

	return (a);
}


CloverFloat CloverVec2Sum(const CloverVec2f* a)
{
	CloverFloat v;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"vfad.p			s000, c000\n"			// s000 = funneladd(c000)
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a)
	);
	return (v);
}

CloverFloat CloverVec2Avg(const CloverVec2f* a)
{
	CloverFloat v;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"vavg.p			s000, c000\n"			// s000 = funnelavg(c000)
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a)
	);
	return (v);
}

CloverFloat CloverVec3Sum(const CloverVec3f* a)
{
	CloverFloat v;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"lv.s			s002, 8 + %1\n"			// s002 = a->z
		"vfad.t			s000, c000\n"			// s000 = funneladd(c000)
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a)
	);
	return (v);
}

CloverFloat CloverVec3Avg(const CloverVec3f* a)
{
	CloverFloat v;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"lv.s			s002, 8 + %1\n"			// s002 = a->z
		"vavg.t			s000, c000\n"			// s000 = funnelavg(c000)
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a)
	);
	return (v);
}

CloverFloat CloverVec4Sum(const CloverVec4f* a)
{
	CloverFloat v;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"vfad.q			s000, c000\n"			// s000 = funneladd(c000)
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a)
	);
	return (v);
}

CloverFloat CloverVec4Sum3(const CloverVec4f* a)
{
	CloverFloat v;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"vfad.t			s000, c000\n"			// s000 = funneladd(c000)
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a)
	);
	return (v);
}

CloverFloat CloverVec4Avg(const CloverVec4f* a)
{
	CloverFloat v;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"vavg.q			s000, c000\n"			// s000 = funnelavg(c000)
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a)
	);
	return (v);
}

CloverFloat CloverVec4Avg3(const CloverVec4f* a)
{
	CloverFloat v;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"vavg.t			s000, c000\n"			// s000 = funnelavg(c000)
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a)
	);
	return (v);
}


CloverVec2f* CloverVec2Sgn(CloverVec2f* a, const CloverVec2f* b)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"vsgn.p			c000, c000\n"			// c000 = sign(c000)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec2f* CloverVec2Normalize(CloverVec2f* a, const CloverVec2f* b)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"vdot.p			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002 + s003*s003
		"vzero.s		s011\n"					// s011 = 0
		"vcmp.s			EZ, s010\n"				// CC[0] = (s010==0.0f)
		"vrsq.s			s010, s010\n"			// s010 = 1.0 / sqrt(s010)
		"vcmovt.s		s010, s011, 0\n"		// if (CC[0]) s010 = s011
		"vscl.p			c000[-1:1,-1:1], c000, s010\n"	// c000 = c000 * s010
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverFloat CloverVec2Length(const CloverVec2f* a)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"vsqrt.s		s010, s010\n"			// s010 = sqrt(s010)
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a)
	);
	return (f);
}

CloverFloat CloverVec2SquareLength(const CloverVec2f* a)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a)
	);
	return (f);
}

CloverFloat CloverVec2Dist(const CloverVec2f* a, const CloverVec2f* b)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"lv.s			s010, 0 + %2\n"			// s000 = b->x
		"lv.s			s011, 4 + %2\n"			// s001 = b->y
		"vsub.t			c000, c000, c010\n"		// c000 = c000 - c010
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"vsqrt.s		s010, s010\n"			// s010 = sqrt(s010)
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a), "m"(*b)
	);
	return (f);
}

CloverFloat CloverVec2SquareDist(const CloverVec2f* a, const CloverVec2f* b)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"lv.s			s010, 0 + %2\n"			// s000 = b->x
		"lv.s			s011, 4 + %2\n"			// s001 = b->y
		"vsub.t			c000, c000, c010\n"		// c000 = c000 - c010
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a), "m"(*b)
	);
	return (f);
}

CloverVec2f* CloverVec2Lerp(CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c, CloverFloat t)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %3\n"				// t0   = t
		"mtv			$8,   s030\n"			// s030 = t0
		"lv.s			s010, 0 + %1\n"			// s000 = b->x
		"lv.s			s011, 4 + %1\n"			// s001 = b->y
		"lv.s			s020, 0 + %2\n"			// s000 = c->x
		"lv.s			s021, 4 + %2\n"			// s001 = c->y
		"vsub.p			c000, c020, c010\n"		// c000 = c020 - c010 = (v2 - v1)
		"vscl.p			c000, c000, s030\n"		// c000 = c000 * s030 = (v2 - v1) * t
		"vadd.p			c010, c010, c000\n"		// c010 = c010 + c000 = v1 + t * (v2 - v1)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c), "f"(t)
		: "$8"
	);
	return (a);
}

CloverVec2f* CloverVec2Scale(CloverVec2f* a, const CloverVec2f* b, CloverFloat t)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %2\n"				// t0   = t
		"mtv			$8,   s010\n"			// s010 = t0
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"vscl.p			c000, c000, s010\n"		// c000 = c000 * s010
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "f"(t)
		: "$8"
	);

	return (a);
}



CloverVec3f* CloverVec3Sgn(CloverVec3f* a, const CloverVec3f* b)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"lv.s			s002, 8 + %1\n"			// s002 = b->z
		"vsgn.t			c000, c000\n"			// c000 = sign(c000)
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec3f* CloverVec3Normalize(CloverVec3f* a, const CloverVec3f* b)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"lv.s			s002, 8 + %1\n"			// s002 = b->z
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002 + s003*s003
		"vzero.s		s011\n"					// s011 = 0
		"vcmp.s			EZ, s010\n"				// CC[0] = (s010==0.0f)
		"vrsq.s			s010, s010\n"			// s010 = 1.0 / sqrt(s010)
		"vcmovt.s		s010, s011, 0\n"		// if (CC[0]) s010 = s011
		"vscl.t			c000[-1:1,-1:1,-1:1], c000, s010\n"	// c000 = c000 * s010
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverFloat CloverVec3Length(const CloverVec3f* a)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"lv.s			s002, 8 + %1\n"			// s002 = a->z
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"vsqrt.s		s010, s010\n"			// s010 = sqrt(s010)
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a)
	);
	return (f);
}

CloverFloat CloverVec3SquareLength(const CloverVec3f* a)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"lv.s			s002, 8 + %1\n"			// s002 = a->z
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a)
	);
	return (f);
}

CloverFloat CloverVec3Dist(const CloverVec3f* a, const CloverVec3f* b)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"lv.s			s002, 8 + %1\n"			// s002 = a->z
		"lv.s			s010, 0 + %2\n"			// s010 = b->x
		"lv.s			s011, 4 + %2\n"			// s011 = b->y
		"lv.s			s012, 8 + %2\n"			// s012 = b->z
		"vsub.t			c000, c000, c010\n"		// c000 = c000 - c010
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"vsqrt.s		s010, s010\n"			// s010 = sqrt(s010)
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a), "m"(*b)
	);
	return (f);
}

CloverFloat CloverVec3SquareDist(const CloverVec3f* a, const CloverVec3f* b)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s000, 0 + %1\n"			// s000 = a->x
		"lv.s			s001, 4 + %1\n"			// s001 = a->y
		"lv.s			s002, 8 + %1\n"			// s002 = a->z
		"lv.s			s010, 0 + %2\n"			// s010 = b->x
		"lv.s			s011, 4 + %2\n"			// s011 = b->y
		"lv.s			s012, 8 + %2\n"			// s012 = b->z
		"vsub.t			c000, c000, c010\n"		// c000 = c000 - c010
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a), "m"(*b)
	);
	return (f);
}

CloverVec3f* CloverVec3Lerp(CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c, CloverFloat t)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %3\n"				// t0   = t
		"mtv			$8,   s030\n"			// s030 = t0
		"lv.s			s010, 0 + %1\n"			// s000 = b->x
		"lv.s			s011, 4 + %1\n"			// s001 = b->y
		"lv.s			s012, 8 + %1\n"			// s002 = b->z
		"lv.s			s020, 0 + %2\n"			// s010 = c->x
		"lv.s			s021, 4 + %2\n"			// s011 = c->y
		"lv.s			s022, 8 + %2\n"			// s012 = c->z
		"vsub.t			c000, c020, c010\n"		// c000 = c020 - c010 = (v2 - v1)
		"vscl.t			c000, c000, s030\n"		// c000 = c000 * s030 = (v2 - v1) * t
		"vadd.t			c010, c010, c000\n"		// c010 = c010 + c000 = v1 + t * (v2 - v1)
		"sv.s			s010, 0 + %0\n"			// a->x = s000
		"sv.s			s011, 4 + %0\n"			// a->y = s001
		"sv.s			s012, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c), "f"(t)
		: "$8"
	);
	return (a);
}

CloverVec3f* CloverVec3Scale(CloverVec3f* a, const CloverVec3f* b, CloverFloat t)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %2\n"				// t0   = t
		"mtv			$8,   s010\n"			// s010 = t0
		"lv.s			s000, 0 + %1\n"			// s000 = b->x
		"lv.s			s001, 4 + %1\n"			// s001 = b->y
		"lv.s			s002, 8 + %1\n"			// s002 = b->z
		"vscl.t			c000, c000, s010\n"		// c000 = c000 * s010
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "f"(t)
		: "$8"
	);

	return (a);
}



CloverVec4f* CloverVec4Sgn(CloverVec4f* a, const CloverVec4f* b)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *b
		"vsgn.q			c000, c000\n"			// c000 = sign(c000)
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec4f* CloverVec4Normalize(CloverVec4f* a, const CloverVec4f* b)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *b
		"vdot.q			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002 + s003*s003
		"vzero.s		s011\n"					// s011 = 0
		"vcmp.s			EZ, s010\n"				// CC[0] = (s010==0.0f)
		"vrsq.s			s010, s010\n"			// s010 = 1.0 / sqrt(s010)
		"vcmovt.s		s010, s011, 0\n"		// if (CC[0]) s010 = s011
		"vscl.q			c000[-1:1,-1:1,-1:1,-1:1], c000, s010\n"	// c000 = c000 * s010
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverVec4f* CloverVec4Normalize3(CloverVec4f* a, const CloverVec4f* b)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *b
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"vzero.s		s011\n"					// s011 = 0
		"vcmp.s			EZ, s010\n"				// CC[0] = (s010==0.0f)
		"vrsq.s			s010, s010\n"			// s010 = 1.0 / sqrt(s010)
		"vcmovt.s		s010, s011, 0\n"		// if (CC[0]) s010 = s011
		"vscl.t			c000[-1:1,-1:1,-1:1], c000, s010\n"	// c000 = c000 * s010
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}

CloverFloat CloverVec4Length(const CloverVec4f* a)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"vdot.q			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002 + s003*s003
		"vsqrt.s		s010, s010\n"			// s010 = sqrt(s010)
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a)
	);
	return (f);
}

CloverFloat CloverVec4Length3(const CloverVec4f* a)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"vsqrt.s		s010, s010\n"			// s010 = sqrt(s010)
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a)
	);
	return (f);
}

CloverFloat CloverVec4SquareLength(const CloverVec4f* a)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"vdot.q			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002 + s003*s003
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a)
	);
	return (f);
}

CloverFloat CloverVec4SquareLength3(const CloverVec4f* a)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a)
	);
	return (f);
}

CloverFloat CloverVec4Dist(const CloverVec4f* a, const CloverVec4f* b)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"lv.q			c010, %2\n"				// c010 = *b
		"vsub.q			c000, c000, c010\n"		// c000 = c000 - c010
		"vdot.q			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002 + s003*s003
		"vsqrt.s		s010, s010\n"			// s010 = sqrt(s010)
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a), "m"(*b)
	);
	return (f);
}

CloverFloat CloverVec4Dist3(const CloverVec4f* a, const CloverVec4f* b)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"lv.q			c010, %2\n"				// c010 = *b
		"vsub.t			c000, c000, c010\n"		// c000 = c000 - c010
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"vsqrt.s		s010, s010\n"			// s010 = sqrt(s010)
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a), "m"(*b)
	);
	return (f);
}

CloverFloat CloverVec4SquareDist(const CloverVec4f* a, const CloverVec4f* b)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"lv.q			c010, %2\n"				// c010 = *b
		"vsub.q			c000, c000, c010\n"		// c000 = c000 - c010
		"vdot.q			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002 + s003*s003
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a), "m"(*b)
	);
	return (f);
}

CloverFloat CloverVec4SquareDist3(const CloverVec4f* a, const CloverVec4f* b)
{
	CloverFloat f;
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *a
		"lv.q			c010, %2\n"				// c010 = *b
		"vsub.t			c000, c000, c010\n"		// c000 = c000 - c010
		"vdot.t			s010, c000, c000\n"		// s010 = s000*s000 + s001*s001 + s002*s002
		"sv.s			s010, %0\n"				// f    = s010
		".set			pop\n"					// restore assember option
		: "=m"(f)
		: "m"(*a), "m"(*b)
	);
	return (f);
}

CloverVec4f* CloverVec4Lerp(CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c, CloverFloat t)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %3\n"				// t0   = t
		"mtv			$8,   s030\n"			// s030 = t0
		"lv.q			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vsub.q			c000, c020, c010\n"		// c000 = c020 - c010 = (v2 - v1)
		"vscl.q			c000, c000, s030\n"		// c000 = c000 * s030 = (v2 - v1) * t
		"vadd.q			c010, c010, c000\n"		// c010 = c010 + c000 = v1 + t * (v2 - v1)
		"sv.q			c010, %0\n"				// *a = c010
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c), "f"(t)
		: "$8"
	);
	return (a);
}

CloverVec4f* CloverVec4Lerp3(CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c, CloverFloat t)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %3\n"				// t0   = t
		"mtv			$8,   s030\n"			// s030 = t0
		"lv.q			c010, %1\n"				// c010 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"vsub.t			c000, c020, c010\n"		// c000 = c020 - c010 = (v2 - v1)
		"vscl.t			c000, c000, s030\n"		// c000 = c000 * s030 = (v2 - v1) * t
		"vadd.t			c010, c010, c000\n"		// c010 = c010 + c000 = v1 + t * (v2 - v1)
		"sv.q			c010, %0\n"				// *a = c010
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c), "f"(t)
		: "$8"
	);
	return (a);
}

CloverVec4f* CloverVec4Scale(CloverVec4f* a, const CloverVec4f* b, CloverFloat t)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %2\n"				// t0   = t
		"mtv			$8,   s010\n"			// s010 = t0
		"lv.q			c000, %1\n"				// c000 = *b
		"vscl.q			c000, c000, s010\n"		// c000 = c000 * s010
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "f"(t)
		: "$8"
	);

	return (a);
}

CloverVec4f* CloverVec4Scale3(CloverVec4f* a, const CloverVec4f* b, CloverFloat t)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"mfc1			$8,   %2\n"				// t0   = t
		"mtv			$8,   s010\n"			// s010 = t0
		"lv.q			c000, %1\n"				// c000 = *b
		"vscl.t			c000, c000, s010\n"		// c000 = c000 * s010
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "f"(t)
		: "$8"
	);

	return (a);
}



CloverFloat CloverVec2Dot(const CloverVec2f* a, const CloverVec2f* b)
{
	CloverFloat v;

	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"vdot.p			s000, c010, c020\n"		// s000 = c010 dot c020
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a), "m"(*b)
	);
	
	return v;
}

CloverFloat CloverVec3Dot(const CloverVec3f* a, const CloverVec3f* b)
{
	CloverFloat v;

	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s012, 8 + %1\n"			// s012 = b->z
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"lv.s			s022, 8 + %2\n"			// s022 = c->z
		"vdot.t			s000, c010, c020\n"		// s000 = c010 dot c020
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a), "m"(*b)
	);
	
	return v;
}

CloverFloat CloverVec4Dot(const CloverVec4f* a, const CloverVec4f* b)
{
	CloverFloat v;

	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c010, %1\n"				// c010 = *a
		"lv.q			c020, %2\n"				// c020 = *b
		"vdot.q			s000, c010, c020\n"		// s000 = c010 dot c020
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a), "m"(*b)
	);
	
	return v;
}

CloverFloat CloverVec4Dot3(const CloverVec4f* a, const CloverVec4f* b)
{
	CloverFloat v;

	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c010, %1\n"				// c010 = *a
		"lv.q			c020, %2\n"				// c020 = *b
		"vdot.t			s000, c010, c020\n"		// s000 = c010 dot c020
		"sv.s			s000, %0\n"				// v    = s000
		".set			pop\n"					// restore assember option
		: "=m"(v)
		: "m"(*a), "m"(*b)
	);
	
	return v;
}


CloverVec3f* CloverVec3Cross(CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c)
{
    __cloverasm__ (
        ".set           push\n"                 // save assembler option
        ".set           noreorder\n"            // suppress reordering
        "lv.s           s010, 0 + %1\n"         // s010 = b->x
        "lv.s           s011, 4 + %1\n"         // s011 = b->y
        "lv.s           s012, 8 + %1\n"         // s012 = b->z
        "lv.s           s020, 0 + %2\n"         // s020 = c->x
        "lv.s           s021, 4 + %2\n"         // s021 = c->y
        "lv.s           s022, 8 + %2\n"         // s022 = c->z
        "vcrsp.t        c000, c010, c020\n"     // c000 = c010 x c020 (используем c000 как промежуточный)
        "sv.s           s000, 0 + %0\n"         // a->x = s000
        "sv.s           s001, 4 + %0\n"         // a->y = s001
        "sv.s           s002, 8 + %0\n"         // a->z = s002
        ".set           pop\n"                  // restore assembler option
        : "=m"(*a)
        : "m"(*b), "m"(*c)
    );
    return (a);
}


CloverVec4f* CloverVec4Cross(CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c)
{
    __cloverasm__ (
        ".set           push\n"                 // save assembler option
        ".set           noreorder\n"            // suppress reordering
        "lv.q           c010, %1\n"             // c010 = *b
        "lv.q           c020, %2\n"             // c020 = *c
        "vcrsp.t        c000, c010, c020\n"     // c000 = c010 x c020 (используем c000 как промежуточный)
        "sv.q           c000, %0\n"             // *a = c000
        ".set           pop\n"                  // restore assembler option
        : "=m"(*a)
        : "m"(*b), "m"(*c)
    );
    return (a);
}




CloverVec2f* CloverVec2Reflect(CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c)
{
    __cloverasm__ (
        ".set           push\n"                 // save assembler option
        ".set           noreorder\n"            // suppress reordering
        "lv.s           s010, 0 + %1\n"         // s010 = b->x
        "lv.s           s011, 4 + %1\n"         // s011 = b->y
        "lv.s           s020, 0 + %2\n"         // s020 = c->x
        "lv.s           s021, 4 + %2\n"         // s021 = c->y
        "vdot.p         s031, c010, c020\n"     // s031 = s010*s020 + s011*s021
        "vfim.s         s030, -2.0\n"           // s030 = -2.0f
        "vmul.s         s032, s030, s031\n"     // s032 = s030 * s031
        "vscl.p         c020, c020, s032\n"     // c020 = c020 * s032
        "vadd.p         c000, c010, c020\n"     // c000 = c010 + c020
        "vdot.p         s033, c000, c000\n"     // s033 = c000 * c000
        "vcmp.s         EZ, s033\n"             // CC[0] = (s033==0.0f)
        "vrsq.s         s033, s033\n"           // s033 = 1.0 / sqrt(s033)
        "vcmovt.s       s033, s033[0], 0\n"     // if (CC[0]) s033 = 0
        "vscl.p         c000[-1:1,-1:1], c000, s033\n"
                                                // c000 = c000 * s033
        "sv.s           s000, 0 + %0\n"         // a->x = s000
        "sv.s           s001, 4 + %0\n"         // a->y = s001
        ".set           pop\n"                  // restore assembler option
        : "=m"(*a)
        : "m"(*b), "m"(*c)
    );
    return (a);
}


CloverVec2f* CloverVec2Refract(CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c, const CloverFloat eta)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"lv.s			s030, %3\n"				// s030 = eta
		"vdot.p			s031, c010, c020\n"		// s031 = s010*s020 + s011*s021 + s012*s022
		"vscl.p			c010, c010, s030\n"		// c010 = c010 * s030 = *b * eta
		"vmul.s			s032, s030, s030\n"		// s032 = s030 * s030 = eta * eta
		"vmul.s			s033, s031, s031\n"		// s033 = s031 * s031 = d * d
		"vmul.s			s031, s031, s030\n"		// s031 = s031 * s030 = d * eta
		"vocp.s			s033, s033\n"			// s033 = 1.0 - s033  = 1 - d*d
		"vmul.s			s033, s032, s033\n"		// s033 = s033 * s032 = eta * eta * (1 - d*d)
		"vocp.s			s033, s033\n"			// s033 = 1.0 - s033  = 1 - eta * eta * (1 - d*d)
		"vsqrt.s		s033, s033\n"			// s033 = sqrt(s033)  = sqrt(1 - eta * eta * (1 - d*d))
		"vsub.s			s031, s031, s033\n"		// s031 = s031 - s032 = d * eta - sqrt(1 - eta * eta * (1 - d*d))
		"vscl.p			c020, c020, s031\n"		// c020 = c020 * s031
		"vadd.p			c000, c010, c020\n"		// c000 = c010 + c020
		"vdot.p			s033, c000, c000\n"		// s033 = c000 * c000
		"vcmp.p			ES, c000\n"				// CC[4] = isinfornan(s000) | isinfornan(s001) | isinfornan(s002)
		"vrsq.s			s033, s033\n"			// s033 = 1.0 / sqrt(s033)
		"vscl.p			c000[-1:1,-1:1], c000, s033\n"
												// c000 = c000 * s033
		"vcmovt.p		c000, c000[0,0], 4\n"	// if (CC[4]) c000 = 0
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c), "m"(eta)
	);
	return (a);
}


CloverVec3f* CloverVec3Reflect(CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c)
{
    __cloverasm__ (
        ".set           push\n"                 // save assembler option
        ".set           noreorder\n"            // suppress reordering
        "lv.s           s010, 0 + %1\n"         // s010 = b->x
        "lv.s           s011, 4 + %1\n"         // s011 = b->y
        "lv.s           s012, 8 + %1\n"         // s012 = b->z
        "lv.s           s020, 0 + %2\n"         // s020 = c->x
        "lv.s           s021, 4 + %2\n"         // s021 = c->y
        "lv.s           s022, 8 + %2\n"         // s022 = c->z
        "vdot.t         s031, c010, c020\n"     // s031 = s010*s020 + s011*s021 + s012*s022
        "vfim.s         s030, -2.0\n"           // s030 = -2.0f
        "vmul.s         s032, s030, s031\n"     // s032 = s030 * s031
        "vscl.t         c020, c020, s032\n"     // c020 = c020 * s032
        "vadd.t         c000, c010, c020\n"     // c000 = c010 + c020
        "vdot.t         s033, c000, c000\n"     // s033 = c000 * c000
        "vcmp.s         EZ, s033\n"             // CC[0] = (s033==0.0f)
        "vrsq.s         s033, s033\n"           // s033 = 1.0 / sqrt(s033)
        "vcmovt.s       s033, s033[0], 0\n"     // if (CC[0]) s033 = 0
        "vscl.t         c000[-1:1,-1:1,-1:1], c000, s033\n"
                                                // c000 = c000 * s033
        "sv.s           s000, 0 + %0\n"         // a->x = s000
        "sv.s           s001, 4 + %0\n"         // a->y = s001
        "sv.s           s002, 8 + %0\n"         // a->z = s002
        ".set           pop\n"                  // restore assembler option
        : "=m"(*a)
        : "m"(*b), "m"(*c)
    );
    return (a);
}


CloverVec3f* CloverVec3Refract(CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c, const CloverFloat eta)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.s			s010, 0 + %1\n"			// s010 = b->x
		"lv.s			s011, 4 + %1\n"			// s011 = b->y
		"lv.s			s012, 8 + %1\n"			// s012 = b->z
		"lv.s			s020, 0 + %2\n"			// s020 = c->x
		"lv.s			s021, 4 + %2\n"			// s021 = c->y
		"lv.s			s022, 8 + %2\n"			// s022 = c->z
		"lv.s			s030, %3\n"				// s030 = eta
		"vdot.t			s031, c010, c020\n"		// s031 = s010*s020 + s011*s021 + s012*s022
		"vscl.t			c010, c010, s030\n"		// c010 = c010 * s030 = *b * eta
		"vmul.s			s032, s030, s030\n"		// s032 = s030 * s030 = eta * eta
		"vmul.s			s033, s031, s031\n"		// s033 = s031 * s031 = d * d
		"vmul.s			s031, s031, s030\n"		// s031 = s031 * s030 = d * eta
		"vocp.s			s033, s033\n"			// s033 = 1.0 - s033  = 1 - d*d
		"vmul.s			s033, s032, s033\n"		// s033 = s033 * s032 = eta * eta * (1 - d*d)
		"vocp.s			s033, s033\n"			// s033 = 1.0 - s033  = 1 - eta * eta * (1 - d*d)
		"vsqrt.s		s033, s033\n"			// s033 = sqrt(s033)  = sqrt(1 - eta * eta * (1 - d*d))
		"vsub.s			s031, s031, s033\n"		// s031 = s031 - s032 = d * eta - sqrt(1 - eta * eta * (1 - d*d))
		"vscl.t			c020, c020, s031\n"		// c020 = c020 * s031
		"vadd.t			c000, c010, c020\n"		// c000 = c010 + c020
		"vdot.t			s033, c000, c000\n"		// s033 = c000 * c000
		"vcmp.t			ES, c000\n"				// CC[4] = isinfornan(s000) | isinfornan(s001) | isinfornan(s002)
		"vrsq.s			s033, s033\n"			// s033 = 1.0 / sqrt(s033)
		"vscl.t			c000[-1:1,-1:1,-1:1], c000, s033\n"
												// c000 = c000 * s033
		"vcmovt.t		c000, c000[0,0,0], 4\n"	// if (CC[4]) c000 = 0
		"sv.s			s000, 0 + %0\n"			// a->x = s000
		"sv.s			s001, 4 + %0\n"			// a->y = s001
		"sv.s			s002, 8 + %0\n"			// a->z = s002
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c), "m"(eta)
	);
	return (a);
}


CloverVec4f* CloverVec4Reflect(CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c)
{
    __cloverasm__ (
        ".set           push\n"                 // save assembler option
        ".set           noreorder\n"            // suppress reordering
        "lv.q           c010, %1\n"             // c010 = *b
        "lv.q           c020, %2\n"             // c020 = *c
        "vdot.t         s031, c010, c020\n"     // s031 = s000*s020 + s001*s021 + s002*s022
        "vfim.s         s030, -2.0\n"           // s030 = -2.0f
        "vmul.s         s032, s030, s031\n"     // s032 = s030 * s031
        "vscl.t         c020, c020, s032\n"     // c020 = c020 * s032
        "vadd.t         c000, c010, c020\n"     // c000 = c010 + c020
        "vdot.t         s033, c000, c000\n"     // s033 = c000 * c000
        "vcmp.s         EZ, s033\n"             // CC[0] = (s033==0.0f)
        "vrsq.s         s033, s033\n"           // s033 = 1.0 / sqrt(s033)
        "vcmovt.s       s033, s033[0], 0\n"     // if (CC[0]) s033 = 0
        "vscl.t         c000[-1:1,-1:1,-1:1], c000, s033\n"
                                                // c000 = c000 * s033
        "sv.q           c000, %0\n"             // *a  = c000
        ".set           pop\n"                  // restore assembler option
        : "=m"(*a)
        : "m"(*b), "m"(*c)
    );
    return (a);
}



CloverVec4f* CloverVec4Refract(CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c, const CloverFloat eta)
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000, %1\n"				// c000 = *b
		"lv.q			c020, %2\n"				// c020 = *c
		"lv.s			s030, %3\n"				// s030 = eta
		"vdot.t			s031, c000, c020\n"		// s031 = s000*s020 + s001*s021 + s002*s022 = d := b*c
		"vscl.t			c000, c000, s030\n"		// c000 = c000 * s030 = b * eta
		"vmul.s			s032, s030, s030\n"		// s032 = s030 * s030 = eta * eta
		"vmul.s			s033, s031, s031\n"		// s033 = s031 * s031 = d * d
		"vmul.s			s031, s031, s030\n"		// s031 = s031 * s030 = d * eta
		"vocp.s			s033, s033\n"			// s033 = 1.0 - s033  = 1 - d*d
		"vmul.s			s033, s032, s033\n"		// s033 = s033 * s032 = eta * eta * (1 - d*d)
		"vocp.s			s033, s033\n"			// s033 = 1.0 - s033  = 1 - eta * eta * (1 - d*d)
		"vsqrt.s		s033, s033\n"			// s033 = sqrt(s033)  = sqrt(1 - eta * eta * (1 - d*d)) =: phi2
		"vsub.s			s031, s031, s033\n"		// s031 = s031 - s032 = d * eta - phi2
		"vscl.t			c020, c020, s031\n"		// c020 = c020 * s031 = c * (d * eta - phi2)
		"vadd.t			c000, c000, c020\n"		// c000 = c000 + c020 = b * eta + c * (d * eta - phi2) =: v (refracted CloverVec)
		"vdot.t			s033, c000, c000\n"		// s033 = c000 * c000 = v * v
		"vcmp.t			ES, c000\n"				// CC[4] = isinfornan(s000) | isinfornan(s001) | isinfornan(s002)
		"vrsq.s			s033, s033\n"			// s033 = 1.0 / sqrt(s033) = 1.0 / sqrt(v*v)
		"vscl.t			c000[-1:1,-1:1,-1:1], c000, s033\n"
												// c000 = c000 * s033 = v / sqrt(v*v) = unitize(v)
		"vcmovt.t		c000, c000[0,0,0], 4\n"	// if (CC[4]) c000 = 0
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c), "m"(eta)
	);
	return (a);
}




CloverMat4f* CloverMat4Identity( CloverMat4f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vmidt.q		e000\n"					// e000 = 0
		"sv.q			c000,  0 + %0\n"		// pm->x = c000
		"sv.q			c010, 16 + %0\n"		// pm->y = c010
		"sv.q			c020, 32 + %0\n"		// pm->z = c020
		"sv.q			c030, 48 + %0\n"		// pm->w = c030
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);

	return (a);
}


CloverMat4f* CloverMat4Zero( CloverMat4f* a )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"vmzero.q		e000\n"					// e000 = 0
		"sv.q			c000,  0 + %0\n"		// pm->x = c000
		"sv.q			c010, 16 + %0\n"		// pm->y = c010
		"sv.q			c020, 32 + %0\n"		// pm->z = c020
		"sv.q			c030, 48 + %0\n"		// pm->w = c030
		".set			pop\n"					// restore assember option
		: "=m"(*a)
	);

	return (a);
}


CloverMat4f* CloverMat4Copy( CloverMat4f* a, const CloverMat4f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000,  0 + %1\n"		// c000 = b->x
		"lv.q			c010, 16 + %1\n"		// c010 = b->y
		"lv.q			c020, 32 + %1\n"		// c020 = b->z
		"lv.q			c030, 48 + %1\n"		// c030 = b->w
		"sv.q			c000,  0 + %0\n"		// a->x = c000
		"sv.q			c010, 16 + %0\n"		// a->y = c010
		"sv.q			c020, 32 + %0\n"		// a->z = c020
		"sv.q			c030, 48 + %0\n"		// a->w = c030
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return(a);
}


CloverMat4f* CloverMat4Mul( CloverMat4f* a, const CloverMat4f* b, const CloverMat4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c100,  0 + %1\n"		// c100 = b->x
		"lv.q			c110, 16 + %1\n"		// c110 = b->y
		"lv.q			c120, 32 + %1\n"		// c120 = b->z
		"lv.q			c130, 48 + %1\n"		// c130 = b->w
		"lv.q			c200,  0 + %2\n"		// c200 = c->x
		"lv.q			c210, 16 + %2\n"		// c210 = c->y
		"lv.q			c220, 32 + %2\n"		// c220 = c->z
		"lv.q			c230, 48 + %2\n"		// c230 = c->w
		"vmmul.q		e000, e200, e100\n"		// e000 = e100 * e200
		"sv.q			c000,  0 + %0\n"		// a->x = c000
		"sv.q			c010, 16 + %0\n"		// a->y = c010
		"sv.q			c020, 32 + %0\n"		// a->z = c020
		"sv.q			c030, 48 + %0\n"		// a->w = c030
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}


CloverMat4f* CloverMat4Inv( CloverMat4f* a, const CloverMat4f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c100,  0 + %1\n"		// c100 = b->x
		"lv.q			c110, 16 + %1\n"		// c110 = b->y
		"lv.q			c120, 32 + %1\n"		// c120 = b->z
		"lv.q			c000, 48 + %1\n"		// c000 = b->w
		"vzero.t		c130\n"					// c130 = (0, 0, 0)
		"vtfm3.t		c010, m100, c000\n"		// c010 = [e100]tr * c000
		"sv.q			r100,  0 + %0\n"		// a->x = r100
		"sv.q			r101, 16 + %0\n"		// a->y = r101
		"vneg.t			c000, c010\n"			// c000 = (s010, s011, s012, s003)
		"sv.q			r102, 32 + %0\n"		// a->z = r102
		"sv.q			c000, 48 + %0\n"		// a->w = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return (a);
}


CloverFloat CloverMat4Det( const CloverMat4f* a )
{
	CloverFloat v;
	__cloverasm__ (
		".set			push\n"						// save assember option
		".set			noreorder\n"				// suppress reordering
		"lv.q			c000,  0 + %1\n"			// c000 = a->x = (a11,a21,a31,a41)
		"lv.q			c010, 16 + %1\n"			// c010 = a->y = (a12,a22,a32,a42)
		"lv.q			c020, 32 + %1\n"			// c020 = a->z = (a13,a23,a33,a43)
		"lv.q			c030, 48 + %1\n"			// c030 = a->w = (a14,a24,a34,a44)
		"vmul.t			c100, c011[X,Z,Y], c021[Y,X,Z]\n"	// c100 = (a22*a33, a23*a42, a32*a43)
		"vmul.t			c110, c001[X,Z,Y], c021[Y,X,Z]\n"	// c110 = (a21*a33, a23*a41, a31*a43)
		"vmul.t			c120, c001[X,Z,Y], c011[Y,X,Z]\n"	// c120 = (a21*a32, a22*a41, a31*a42)
		"vmul.t			c130, c001[X,Z,Y], c011[Y,X,Z]\n"	// c130 = (a21*a32, a22*a41, a31*a42)
		"vmul.t			c200, c011[Z,Y,X], c021[Y,X,Z]\n"	// c200 = (a33*a42, a23*a32, a22*a43)
		"vmul.t			c210, c001[Z,Y,X], c021[Y,X,Z]\n"	// c210 = (a33*a41, a23*a31, a21*a43)
		"vmul.t			c220, c001[Z,Y,X], c011[Y,X,Z]\n"	// c220 = (a32*a41, a22*a31, a21*a42)
		"vmul.t			c230, c001[Z,Y,X], c011[Y,X,Z]\n"	// c230 = (a32*a41, a22*a31, a21*a42)
		"vdot.t			s100, c100, c031[Z,Y,X]\n"	// c100 = (a22*a33*a44 + a23*a34*a42 + a24*a32*a43)
		"vdot.t			s110, c110, c031[Z,Y,X]\n"	// c110 = (a21*a33*a44 + a23*a34*a41 + a24*a31*a43)
		"vdot.t			s120, c120, c031[Z,Y,X]\n"	// c120 = (a21*a32*a44 + a22*a34*a41 + a24*a31*a42)
		"vdot.t			s130, c130, c021[Z,Y,X]\n"	// c130 = (a21*a32*a43 + a22*a33*a41 + a23*a31*a42)
		"vdot.t			s200, c200, c031[X,Z,Y]\n"	// c200 = (a24*a33*a42 + a23*a32*a44 + a22*a34*a43)
		"vdot.t			s210, c210, c031[X,Z,Y]\n"	// c210 = (a24*a33*a41 + a23*a31*a44 + a21*a34*a43)
		"vdot.t			s220, c220, c031[X,Z,Y]\n"	// c220 = (a24*a32*a41 + a22*a31*a44 + a21*a34*a42)
		"vdot.t			s230, c230, c021[X,Z,Y]\n"	// c230 = (a23*a32*a41 + a22*a31*a43 + a21*a33*a42)
		"vsub.q			r100, r100, r200\n"			// r100 = r100 - r200 = (s100-s200, s110-s210, s120-s220, s130-s230)
		"vmul.q			r101, r000, r100\n"			// r101 = r000 * r100 = (s000*s100, s010*s110, s020*s120, s030*s130)
		"vfad.q			s000, r101[X,-Y,Z,-W]\n"	// s000 = s101 - s111 + s121 - s131
		"sv.s			s000, %0\n"					// v = s000
		".set			pop\n"						// restore assember option
		: "=m"(v)
		: "m"(*a)
	);
	return (v);
}


CloverFloat CloverMat4Trace( const CloverMat4f* a )
{
	return (a->m[0] + a->m[5] + a->m[10] + a->m[15]);
}


CloverMat4f* CloverMat4Trans( CloverMat4f* a, const CloverMat4f* b )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c000,  0 + %1\n"		// c000 = b->x
		"lv.q			c010, 16 + %1\n"		// c010 = b->y
		"lv.q			c020, 32 + %1\n"		// c020 = b->z
		"lv.q			c030, 48 + %1\n"		// c030 = b->w
		"sv.q			r000,  0 + %0\n"		// a->x = r000
		"sv.q			r001, 16 + %0\n"		// a->y = r010
		"sv.q			r002, 32 + %0\n"		// a->z = r020
		"sv.q			r003, 48 + %0\n"		// a->w = r030
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b)
	);
	return(a);
}


CloverVec4f* CloverMat4Apply( CloverVec4f* a, const CloverMat4f* b, const CloverVec4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c100,  0 + %1\n"		// c100 = b->x
		"lv.q			c110, 16 + %1\n"		// c110 = b->y
		"lv.q			c120, 32 + %1\n"		// c120 = b->z
		"lv.q			c130, 48 + %1\n"		// c130 = b->w
		"lv.q			c200, %2\n"				// c200 = *c
		"vtfm4.q		c000, e100, c200\n"		// c000 = e100 * c200
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}


CloverVec4f* CloverMat4Apply3( CloverVec4f* a, const CloverMat4f* b, const CloverVec4f* c )
{
	__cloverasm__ (
		".set			push\n"					// save assember option
		".set			noreorder\n"			// suppress reordering
		"lv.q			c100,  0 + %1\n"		// c100 = b->x
		"lv.q			c110, 16 + %1\n"		// c110 = b->y
		"lv.q			c120, 32 + %1\n"		// c120 = b->z
		"lv.q			c200, %2\n"				// c200 = *c
		"vmov.s			s003, s203\n"			// s003 = s203
		"vtfm3.t		c000, e100, c200\n"		// c000 = e100 * c200
		"sv.q			c000, %0\n"				// *a = c000
		".set			pop\n"					// restore assember option
		: "=m"(*a)
		: "m"(*b), "m"(*c)
	);
	return(a);
}