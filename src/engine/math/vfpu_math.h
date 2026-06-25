#ifndef VFPU_MATH_H
#define VFPU_MATH_H

#include <psptypes.h> // Необходимо для ScePspFVector3
#include "CloverTypes.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PI   3.14159265358979f
#define PI_2 1.57079632679489f

#define DEG_TO_RAD (PI / 180.f)
#define RAD_TO_DEG (180.f / PI)

/** * Базовые скалярные операции VFPU 
 */

/* Абсолютное значение */
float vfpu_absf(float x);

/* Округление вверх */
float vfpu_ceilf(float x);

/* Округление вниз */
float vfpu_floorf(float x);

/* Округление до ближайшего */
float vfpu_roundf(float x);

/* Отбрасывание дробной части (к нулю) */
float vfpu_truncf(float x);

/* Тригонометрия */
float vfpu_sinf(float radians);
float vfpu_cosf(float radians);
void  vfpu_sincosf(float radians, float *s, float *c);
float vfpu_tanf(float radians);
float vfpu_asinf(float x);
float vfpu_acosf(float x);
float vfpu_atanf(float x);
float vfpu_atan2f(float y, float x);

/* Гиперболические функции */
float vfpu_sinhf(float x);
float vfpu_coshf(float x);
float vfpu_tanhf(float x);

/* Степени и корни */
float vfpu_sqrtf(float x);
float vfpu_invsqrtf(float x); /* 1 / sqrt(x) */
float vfpu_powf(float x, float y);
float vfpu_pow2f(float x); /* 2^x */
float vfpu_expf(float x);  /* e^x */

/* Логарифмы */
float vfpu_logf(float x);   /* ln(x) */
float vfpu_log2f(float x);  /* log2(x) */
float vfpu_log10f(float x); /* log10(x) */

/* Остаток и сравнение */
float vfpu_fmodf(float x, float y);
float vfpu_maxf(float x, float y);
float vfpu_minf(float x, float y);

/* Утилиты */
float vfpu_deg2rad(float degrees);
float vfpu_rad2deg(float radians);

/* Случайные числа (Hardware RNG) */
void  vfpu_srand(unsigned int seed);
float vfpu_randf(float min, float max);
int   vfpu_randi(float min, float max);

/**
 * Векторные операции VFPU (3D)
 */

/* Нормализация вектора */
ScePspFVector3 *vfpu_vec_normalize(ScePspFVector3 *result, const ScePspFVector3 *vec);

/* Сложение векторов */
ScePspFVector3 *vfpu_vec_add(ScePspFVector3 *result, const ScePspFVector3 *vec1, const ScePspFVector3 *vec2);

/* Вычитание векторов */
ScePspFVector3 *vfpu_vec_sub(ScePspFVector3 *result, const ScePspFVector3 *vec1, const ScePspFVector3 *vec2);

/* Сравнение векторов (1 если равны, 0 если нет) */
int vfpu_vec_compare(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2);

/* Масштабирование вектора (умножение на скаляр) */
ScePspFVector3 *vfpu_vec_scale(ScePspFVector3 *result, const ScePspFVector3 *vec, float scalar);

/* Скалярное произведение (Dot Product) */
float vfpu_vec_dot(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2);

/* Длина вектора */
float vfpu_vec_length(const ScePspFVector3 *vec);

/* Угол между векторами */
float vfpu_vec_angle(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2);

/* Вращение вектора вокруг оси Z */
ScePspFVector3 *vfpu_vec_rotate_z(const ScePspFVector3 *vec, float radians, ScePspFVector3 *result);

// --- Дополнительные VFPU функции для работы с матрицами и векторами ---
CloverVec2f* CloverVec2Set( CloverVec2f* a, const CloverFloat x, const CloverFloat y );
CloverVec2i* CloverVec2Ceil( CloverVec2i* a, const CloverVec2f* b );
CloverVec2i* CloverVec2Trunc( CloverVec2i* a, const CloverVec2f* b );
CloverVec2i* CloverVec2Round( CloverVec2i* a, const CloverVec2f* b );
CloverVec2i* CloverVec2Floor( CloverVec2i* a, const CloverVec2f* b );
CloverVec2f* CloverVec2i2f( CloverVec2f* a, const CloverVec2i* b );
CloverVec2f* CloverVec2Add( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c );
CloverVec2f* CloverVec2Sub( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c );
CloverVec2f* CloverVec2Mul( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c );
CloverVec2f* CloverVec2Div( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c );
CloverVec2f* CloverVec2Neg( CloverVec2f* a, const CloverVec2f* b );
CloverVec2f* CloverVec2Abs( CloverVec2f* a, const CloverVec2f* b );
CloverVec2f* CloverVec2Rndn( CloverVec2f* a );
CloverVec2f* CloverVec2Rnd( CloverVec2f* a );
CloverVec2f* CloverVec2Rnd2( CloverVec2f* a );
CloverVec2f* CloverVec2Clamp( CloverVec2f* a, const CloverVec2f* b, CloverFloat min, CloverFloat max );
CloverVec2f* CloverVec2Min( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c );
CloverVec2f* CloverVec2Max( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c );
CloverFloat CloverVec2Sum( const CloverVec2f* a );
CloverFloat CloverVec2Avg( const CloverVec2f* a );
CloverVec2f* CloverVec2Sgn( CloverVec2f* a, const CloverVec2f* b );
CloverVec2f* CloverVec2Normalize( CloverVec2f* a, const CloverVec2f* b );
CloverFloat CloverVec2Length( const CloverVec2f* a );
CloverFloat CloverVec2SquareLength( const CloverVec2f* a );
CloverFloat CloverVec2Dist( const CloverVec2f* a, const CloverVec2f* b );
CloverFloat CloverVec2SquareDist( const CloverVec2f* a, const CloverVec2f* b );
CloverVec2f* CloverVec2Lerp( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c, CloverFloat t );
CloverVec2f* CloverVec2Scale( CloverVec2f* a, const CloverVec2f* b, CloverFloat t );
CloverFloat CloverVec2Dot( const CloverVec2f* a, const CloverVec2f* b );
CloverVec2f* CloverVec2Reflect( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c );
CloverVec2f* CloverVec2Refract( CloverVec2f* a, const CloverVec2f* b, const CloverVec2f* c, const CloverFloat eta );

CloverVec3f* CloverVec3Set( CloverVec3f* a, const CloverFloat x, const CloverFloat y, const CloverFloat z );
CloverVec3i* CloverVec3Ceil( CloverVec3i* a, const CloverVec3f* b );
CloverVec3i* CloverVec3Trunc( CloverVec3i* a, const CloverVec3f* b );
CloverVec3i* CloverVec3Round( CloverVec3i* a, const CloverVec3f* b );
CloverVec3i* CloverVec3Floor( CloverVec3i* a, const CloverVec3f* b );
CloverVec3f* CloverVec3i2f( CloverVec3f* a, const CloverVec3i* b );
CloverVec3f* CloverVec3Add( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c );
CloverVec3f* CloverVec3Sub( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c );
CloverVec3f* CloverVec3Mul( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c );
CloverVec3f* CloverVec3Div( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c );
CloverVec3f* CloverVec3Neg( CloverVec3f* a, const CloverVec3f* b );
CloverVec3f* CloverVec3Abs( CloverVec3f* a, const CloverVec3f* b );
CloverVec3f* CloverVec3Rndn( CloverVec3f* a );
CloverVec3f* CloverVec3Rnd( CloverVec3f* a );
CloverVec3f* CloverVec3Rnd2( CloverVec3f* a );
CloverVec3f* CloverVec3Clamp( CloverVec3f* a, const CloverVec3f* b, CloverFloat min, CloverFloat max );
CloverVec3f* CloverVec3Min( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c );
CloverVec3f* CloverVec3Max( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c );
CloverFloat CloverVec3Sum( const CloverVec3f* a );
CloverFloat CloverVec3Avg( const CloverVec3f* a );
CloverVec3f* CloverVec3Sgn( CloverVec3f* a, const CloverVec3f* b );
CloverVec3f* CloverVec3Normalize( CloverVec3f* a, const CloverVec3f* b );
CloverFloat CloverVec3Length( const CloverVec3f* a );
CloverFloat CloverVec3SquareLength( const CloverVec3f* a );
CloverFloat CloverVec3Dist( const CloverVec3f* a, const CloverVec3f* b );
CloverFloat CloverVec3SquareDist( const CloverVec3f* a, const CloverVec3f* b );
CloverVec3f* CloverVec3Lerp( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c, CloverFloat t );
CloverVec3f* CloverVec3Scale( CloverVec3f* a, const CloverVec3f* b, CloverFloat t );
CloverFloat CloverVec3Dot( const CloverVec3f* a, const CloverVec3f* b );
CloverVec3f* CloverVec3Cross( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c );
CloverVec3f* CloverVec3Reflect( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c );
CloverVec3f* CloverVec3Refract( CloverVec3f* a, const CloverVec3f* b, const CloverVec3f* c, const CloverFloat eta );



CloverVec4f* CloverVec4Set( CloverVec4f* a, const CloverFloat x, const CloverFloat y, const CloverFloat z, const CloverFloat w );
CloverVec4f* CloverVec4Set3( CloverVec4f* a, const CloverFloat x, const CloverFloat y, const CloverFloat z );
CloverVec4i* CloverVec4Ceil( CloverVec4i* a, const CloverVec4f* b );
CloverVec4i* CloverVec4Trunc( CloverVec4i* a, const CloverVec4f* b );
CloverVec4i* CloverVec4Round( CloverVec4i* a, const CloverVec4f* b );
CloverVec4i* CloverVec4Floor( CloverVec4i* a, const CloverVec4f* b );
CloverVec4f* CloverVec4i2f( CloverVec4f* a, const CloverVec4i* b );


/** Random normal vector.
  * Creates a random unit vector lying on the 3D unit sphere.
  * The fourth component lies in range [-1.0, 1.0[
  * @param a - Pointer to destination vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Rndn3( CloverVec4f* a );

/** Random normal vector.
  * Creates a random unit vector lying on the unit sphere.
  * @param a - Pointer to destination vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Rndn( CloverVec4f* a );

/** Random vector.
  * Creates random components in range [0.0, 1.0[
  * @param a - Pointer to destination vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Rnd( CloverVec4f* a );

/** Random vector.
  * Creates random components in range [-1.0, 1.0[
  * @param a - Pointer to destination vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Rnd2( CloverVec4f* a );

/** Add Vectors.
  * a = b.x+c.x | b.y+c.y | b.z+c.z | b.w+c.w
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Add( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Add Vectors.
  * a = b.x+c.x | b.y+c.y | b.z+c.z | b.w
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Add3( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Subtract Vectors.
  * a = b.x-c.x | b.y-c.y | b.z-c.z | b.w-c.w
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Sub( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Subtract Vectors.
  * a = b.x-c.x | b.y-c.y | b.z-c.z | b.w
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Sub3( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Multiply Vectors.
  * a = b.x*c.x | b.y*c.y | b.z*c.z | b.w*c.w
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Mul( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Multiply Vectors.
  * a = b.x*c.x | b.y*c.y | b.z*c.z | b.w
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Mul3( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Divide Vectors.
  * a = b.x/c.x | b.y/c.y | b.z/c.z | b.w/c.w
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Div( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Divide Vectors.
  * a = b.x/c.x | b.y/c.y | b.z/c.z | b.w
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Div3( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Negate Vector.
  * a = -b
  * @param a - Pointer to destination vector
  * @param b - Pointer to source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Neg( CloverVec4f* a, const CloverVec4f* b );

/** Negate Vector.
  * a = -b
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Neg3( CloverVec4f* a, const CloverVec4f* b );

/** Absolute of Vector.
  * a = abs(b.x) | abs(b.y) | abs(b.z) | abs(b.w)
  * @param a - Pointer to destination vector
  * @param b - Pointer to source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Abs( CloverVec4f* a, const CloverVec4f* b );

/** Clamp Vector.
  * a = clamp(b.x,min,max) | clamp(b.y,min,max) | clamp(b.z,min,max) | clamp(b.w,min,max)
  * @param a - Pointer to destination vector
  * @param b - Pointer to source vector
  * @param min - Minimum to clamp to
  * @param max - Maximum to clamp to
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Clamp( CloverVec4f* a, const CloverVec4f* b, CloverFloat min, CloverFloat max );

/** Clamp Vector.
  * a = clamp(b.x,min,max) | clamp(b.y,min,max) | clamp(b.z,min,max) | b.w
  * @param a - Pointer to destination vector
  * @param b - Pointer to source vector
  * @param min - Minimum to clamp to
  * @param max - Maximum to clamp to
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Clamp3( CloverVec4f* a, const CloverVec4f* b, CloverFloat min, CloverFloat max );

/** Minimum of Vectors.
  * a = min(b.x,c.x) | min(b.y,c.y) | min(b.z,c.z) | min(b.w,c.w)
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Min( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Maximum of Vectors.
  * a = max(b.x,c.x) | max(b.y,c.y) | max(b.z,c.z) | max(b.w,c.w)
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Max( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Sum of components.
  * return (a.x + a.y + a.z + a.w)
  * @param a - Pointer to source vector
  * @return Sum
  */
CloverFloat CloverVec4Sum( const CloverVec4f* a );

/** Sum of components.
  * return (a.x + a.y + a.z)
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to source vector
  * @return Sum
  */
CloverFloat CloverVec4Sum3( const CloverVec4f* a );

/** Average of components.
  * return (a.x + a.y + a.z + a.w)/4
  * @param a - Pointer to source vector
  * @return Average
  */
CloverFloat CloverVec4Avg( const CloverVec4f* a );

/** Average of components.
  * return (a.x + a.y + a.z)/3
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to source vector
  * @return Average
  */
CloverFloat CloverVec4Avg3( const CloverVec4f* a );

/** Vector sign.
  * a = sgn(b.x) | sgn(b.y) | sgn(b.z) | sgn(b.w)
  * @param a - Pointer to destination vector
  * @param b - Pointer to source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Sgn( CloverVec4f* a, const CloverVec4f* b );

/** Normalize Vector.
  * a = b / sqrt(b.b)
  * @param a - Pointer to destination vector
  * @param b - Pointer to source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Normalize( CloverVec4f* a, const CloverVec4f* b );

/** Normalize Vector.
  * a = b / sqrt(b.b)
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Normalize3( CloverVec4f* a, const CloverVec4f* b );

/** Length of Vector.
  * return sqrt( a.a )
  * @param a - Pointer to source vector
  * @return Lenght
  */
CloverFloat CloverVec4Length( const CloverVec4f* a );

/** Length of Vector.
  * return sqrt( a.a )
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to source vector
  * @return Lenght
  */
CloverFloat CloverVec4Length3( const CloverVec4f* a );

/** Square length of Vector.
  * return a.a
  * @param a - Pointer to source vector
  * @return Square lenght
  */
CloverFloat CloverVec4SquareLength( const CloverVec4f* a );

/** Square length of Vector.
  * return a.a
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to source vector
  * @return Square lenght
  */
CloverFloat CloverVec4SquareLength3( const CloverVec4f* a );

/** Distance between Vectors.
  * return sqrt( (b-a).(b-a) )
  * @param a - Pointer to first source vector
  * @param b - Pointer to second source vector
  * @return Distance
  */
CloverFloat CloverVec4Dist( const CloverVec4f* a, const CloverVec4f* b );

/** Distance between Vectors.
  * return sqrt( (b-a).(b-a) )
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to first source vector
  * @param b - Pointer to second source vector
  * @return Distance
  */
CloverFloat CloverVec4Dist3( const CloverVec4f* a, const CloverVec4f* b );

/** Square distance between Vectors.
  * return (b-a).(b-a)
  * @param a - Pointer to first source vector
  * @param b - Pointer to second source vector
  * @return Square distance
  */
CloverFloat CloverVec4SquareDist( const CloverVec4f* a, const CloverVec4f* b );

/** Square distance between Vectors.
  * return (b-a).(b-a)
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to first source vector
  * @param b - Pointer to second source vector
  * @return Square distance
  */
CloverFloat CloverVec4SquareDist3( const CloverVec4f* a, const CloverVec4f* b );

/** Linear interpolate between Vectors.
  * a = b * (1 - t) + c * t
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @param t - Scalar to interpolate with
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Lerp( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c, CloverFloat t );

/** Linear interpolate between Vectors.
  * a = b * (1 - t) + c * t
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @param t - Scalar to interpolate with
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Lerp3( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c, CloverFloat t );

/** Scale Vector.
  * a = b * t
  * @param a - Pointer to destination vector
  * @param b - Pointer to source vector
  * @param t - Scalar to scale b with
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Scale( CloverVec4f* a, const CloverVec4f* b, CloverFloat t );

/** Scale Vector.
  * a = b * t
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to source vector
  * @param t - Scalar to scale b with
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Scale3( CloverVec4f* a, const CloverVec4f* b, CloverFloat t );

/** Calculate Vector dotproduct.
  * return a.b
  * @param a - Pointer to first source vector
  * @param b - Pointer to second source vector
  * @return Dotproduct of a and b
  */
CloverFloat CloverVec4Dot( const CloverVec4f* a, const CloverVec4f* b );

/** Calculate Vector dotproduct.
  * return a.b
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to first source vector
  * @param b - Pointer to second source vector
  * @return Dotproduct of a and b
  */
CloverFloat CloverVec4Dot3( const CloverVec4f* a, const CloverVec4f* b );

/** Calculate Vector crossproduct.
  * a = b x c
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to first source vector
  * @param c - Pointer to second source vector
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Cross( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Reflect Vector.
  * Reflect b on the normale c.
  * a = b - 2*(b.c)*c
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to incoming vector
  * @param c - Pointer to surface normale
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Reflect( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c );

/** Refract Vector.
  * Refract b on the normale c as in Snell's law - eta = n1/n2.
  * <br>Vectors are treated as 3D vectors.
  * @param a - Pointer to destination vector
  * @param b - Pointer to incoming vector
  * @param c - Pointer to surface normale
  * @param eta - Relative refraction index n1/n2
  * @return Pointer to a
  */
CloverVec4f* CloverVec4Refract( CloverVec4f* a, const CloverVec4f* b, const CloverVec4f* c, const CloverFloat eta );

/**
  * Make a matrix identity.
  * Matrix will contain the following after call:<br>
  * | 1.0  0.0  0.0  0.0 |<br>
  * | 0.0  1.0  0.0  0.0 |<br>
  * | 0.0  0.0  1.0  0.0 |<br>
  * | 0.0  0.0  0.0  1.0 |<br>
  * @param a - Pointer to destination matrix
  * @return Pointer to a
  */
CloverMat4f* CloverMat4Identity( CloverMat4f* a );

/** Make a matrix zero.
  * Matrix will contain the following after call:<br>
  * | 0.0  0.0  0.0  0.0 |<br>
  * | 0.0  0.0  0.0  0.0 |<br>
  * | 0.0  0.0  0.0  0.0 |<br>
  * | 0.0  0.0  0.0  0.0 |<br>
  * @param a - Pointer to destination matrix
  * @return Pointer to a
  */
CloverMat4f* CloverMat4Zero( CloverMat4f* a );

/** Copy a matrix.
  * @param a - Pointer to destination matrix
  * @param b - Pointer to matrix to be copied
  * @return Pointer to a
  */
CloverMat4f* CloverMat4Copy( CloverMat4f* a, const CloverMat4f* b );

/** Matrix multiply (a = b*c).
  * @param a - Pointer to destination matrix
  * @param b - Pointer to first input matrix
  * @param c - Pointer to second input matrix
  * @return Pointer to a
  */
CloverMat4f* CloverMat4Mul( CloverMat4f* a, const CloverMat4f* b, const CloverMat4f* c );

/** Invert a matrix.
  * Full Inverse. For orthogonal matrices, triMat4Trans does the same.
  * @param a - Pointer to destination matrix
  * @param b - Pointer to matrix to be inverted
  * @return Pointer to a
  */
CloverMat4f* CloverMat4Inv( CloverMat4f* a, const CloverMat4f* b );

/** Transpose a matrix.
  * @param a - Pointer to destination matrix
  * @param b - Pointer to matrix to be transposed
  * @return Pointer to a
  */
CloverMat4f* CloverMat4Trans( CloverMat4f* a, const CloverMat4f* b );

/** Calculate matrix determinant.
  * @param a - Pointer to source matrix
  * @return Determinant of matrix a
  */
CloverFloat CloverMat4Det( const CloverMat4f* a );

/** Calculate matrix trace.
  * @param a - Pointer to source matrix
  * @return Trace of matrix a
  */
CloverFloat CloverMat4Trace( const CloverMat4f* a );

/** 4x4 Matrix-Vector multiply (a = b*c).
  * @param a - Pointer to dest vector
  * @param b - Pointer to source matrix
  * @param c - Pointer to source vector
  * @return Pointer to a
  */
CloverVec4f* CloverMat4Apply( CloverVec4f* a, const CloverMat4f* b, const CloverVec4f* c );

/** 3x3 Matrix-Vector multiply (a = b*c).
  * Vectors are treated as 3D Vectors and the matrix as 3D matrix.
  * This is useful if you only want to rotate a vector by the 4x4 Matrix (no translation).
  * @param a - Pointer to dest vector
  * @param b - Pointer to source matrix
  * @param c - Pointer to source vector
  * @return Pointer to a
  */
CloverVec4f* CloverMat4Apply3( CloverVec4f* a, const CloverMat4f* b, const CloverVec4f* c );

#ifdef __cplusplus
}
#endif

#endif // VFPU_MATH_H