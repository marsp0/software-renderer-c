#include "math.h"

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

/*      Vector2      */

vec2_t vec2_new(float x, float y)
{
    vec2_t vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

vec2_t vec2_from_scalar(float s)
{
    return vec2_new(s, s);
}

vec2_t vec2_add(vec2_t v1, vec2_t v2)
{
    return vec2_new(v1.x + v2.x, v1.y + v2.y);
}

vec2_t vec2_sub(vec2_t v1, vec2_t v2)
{
    return vec2_new(v1.x - v2.x, v1.y - v2.y);
}

vec2_t vec2_scale(vec2_t v, float scale)
{
    return vec2_new(v.x * scale, v.y * scale);
}

vec2_t vec2_negate(vec2_t v)
{
    return vec2_new(-v.x, -v.y);
}

vec2_t vec2_normalize(vec2_t v)
{
    float magnitude = vec2_magnitude(v);
    return vec2_scale(v, 1.f/magnitude);
}

vec2_t vec2_hadamard(vec2_t v1, vec2_t v2)
{
    return vec2_new(v1.x * v2.x, v1.y * v2.y);
}

vec2_t vec2_clamp(vec2_t v, float min, float max)
{
    float x = f_max(f_min(v.x, max), min);
    float y = f_max(f_min(v.y, max), min);
    return vec2_new(x, y);
}

float vec2_dot(vec2_t v1, vec2_t v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float vec2_magnitude(vec2_t v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

float vec2_magnitude_sq(vec2_t v)
{
    return v.x * v.x + v.y * v.y;
}

void vec2_print(vec2_t v)
{
    printf("vec(%.3f, %.3f)\n", v.x, v.y);
}

/*      Vector3      */

vec3_t vec3_new(float x, float y, float z)
{
    vec3_t vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

vec3_t vec3_from_scalar(float s)
{
    return vec3_new(s, s, s);
}

vec3_t vec3_add(vec3_t v1, vec3_t v2)
{
    return vec3_new(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3_t vec3_sub(vec3_t v1, vec3_t v2)
{
    return vec3_new(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec3_t vec3_cross(vec3_t v1, vec3_t v2)
{
    float x = v1.y * v2.z - v1.z * v2.y;
    float y = v1.z * v2.x - v1.x * v2.z;
    float z = v1.x * v2.y - v1.y * v2.x;
    return vec3_new(x, y, z);
}

vec3_t vec3_scale(vec3_t v, float scale)
{
    return vec3_new(v.x * scale, v.y * scale, v.z * scale);
}

vec3_t vec3_negate(vec3_t v)
{
    return vec3_new(-v.x, -v.y, -v.z);
}

vec3_t vec3_normalize(vec3_t v)
{
    float magnitude = vec3_magnitude(v);
    return vec3_scale(v, 1.f/magnitude);
}

vec3_t vec3_hadamard(vec3_t v1, vec3_t v2)
{
    return vec3_new(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

vec3_t vec3_clamp(vec3_t v, float min, float max)
{
    float x = f_max(f_min(v.x, max), min);
    float y = f_max(f_min(v.y, max), min);
    float z = f_max(f_min(v.z, max), min);
    return vec3_new(x, y, z);
}

float vec3_dot(vec3_t v1, vec3_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vec3_magnitude(vec3_t v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float vec3_magnitude_sq(vec3_t v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

void vec3_print(vec3_t v)
{
    printf("vec(%.3f, %.3f, %.3f)\n", v.x, v.y, v.z);
}

/*      Vector4      */

vec4_t vec4_new(float x, float y, float z)
{
    vec4_t vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    vec.w = 1.f;
    return vec;
}

vec4_t vec4_from_bgra(uint32_t c)
{
    float d = 1.f / 255.f;
    float b = (float)((c >> 24) & 0xFF);
    float g = (float)((c >> 16) & 0xFF);
    float r = (float)((c >>  8) & 0xFF);

    return vec4_new(b * d, g * d, r * d);;
}

uint32_t vec4_to_bgra(vec4_t c)
{
    uint32_t b = ((uint32_t)(f_min(f_max(c.x, 0.f), 1.f) * 255)) << 24;
    uint32_t g = ((uint32_t)(f_min(f_max(c.y, 0.f), 1.f) * 255)) << 16;
    uint32_t r = ((uint32_t)(f_min(f_max(c.z, 0.f), 1.f) * 255)) <<  8;
    // uint32_t a = ((uint32_t)(f_min(f_max(c.w, 0.f), 1.f) * 255)) <<  0;

    return b + g + r;
}

vec4_t vec4_from_scalar(float s)
{
    return vec4_new(s, s, s);
}

vec4_t vec4_add(vec4_t v1, vec4_t v2)
{
    return vec4_new(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec4_t vec4_sub(vec4_t v1, vec4_t v2)
{
    return vec4_new(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec4_t vec4_cross(vec4_t v1, vec4_t v2)
{
    float x = v1.y * v2.z - v1.z * v2.y;
    float y = v1.z * v2.x - v1.x * v2.z;
    float z = v1.x * v2.y - v1.y * v2.x;
    return vec4_new(x, y, z);
}

vec4_t vec4_scale(vec4_t v, float scale)
{
    vec4_t res = vec4_new(v.x * scale, v.y * scale, v.z * scale);
    return res;
}

vec4_t vec4_scale_with_w(vec4_t v, float scale)
{
    vec4_t res = {  .x = v.x * scale,
                    .y = v.y * scale,
                    .z = v.z * scale,
                    .w = v.w * scale };
    return res;
}

vec4_t vec4_pow(vec4_t v, float pow)
{
    v.x = f_pow(v.x, pow);
    v.y = f_pow(v.y, pow);
    v.z = f_pow(v.z, pow);

    return v;
}

vec4_t vec4_abs(vec4_t v)
{
    vec4_t res;

    res.x = f_abs(v.x);
    res.y = f_abs(v.y);
    res.z = f_abs(v.z);

    return res;
}

vec4_t vec4_negate(vec4_t v)
{
    return vec4_new(-v.x, -v.y, -v.z);
}

vec4_t vec4_normalize(vec4_t v)
{
    float magnitude = vec4_magnitude(v);
    return vec4_scale(v, 1.f/magnitude);
}

vec4_t vec4_hadamard(vec4_t v1, vec4_t v2)
{
    return vec4_new(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

vec4_t vec4_clamp(vec4_t v, float min, float max)
{
    float x = f_max(f_min(v.x, max), min);
    float y = f_max(f_min(v.y, max), min);
    float z = f_max(f_min(v.z, max), min);
    return vec4_new(x, y, z);
}

float vec4_dot(vec4_t v1, vec4_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vec4_magnitude(vec4_t v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float vec4_magnitude_sq(vec4_t v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

void vec4_print(vec4_t v, const char* name)
{
    printf("vec(%.3f, %.3f, %.3f, %.3f) \t- %s\n", v.x, v.y, v.z, v.w, name);
}

/*
 * Matrix
 */

mat_t mat_new(float a, float b, float c, float d,
              float e, float f, float g, float h,
              float i, float j, float k, float l,
              float m, float n, float o, float p)
{
    mat_t mat;
    mat.data[0][0] = a;
    mat.data[0][1] = b;
    mat.data[0][2] = c;
    mat.data[0][3] = d;

    mat.data[1][0] = e;
    mat.data[1][1] = f;
    mat.data[1][2] = g;
    mat.data[1][3] = h;

    mat.data[2][0] = i;
    mat.data[2][1] = j;
    mat.data[2][2] = k;
    mat.data[2][3] = l;

    mat.data[3][0] = m;
    mat.data[3][1] = n;
    mat.data[3][2] = o;
    mat.data[3][3] = p;
    return mat;
}

mat_t mat_new_identity()
{
    mat_t mat;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            float val = 0.f;
            if (i == j)
                val = 1.f;
            mat.data[i][j] = val;
        }
    }
    return mat;
}

mat_t mat_from_vec4(vec4_t v1, vec4_t v2, vec4_t v3)
{
    mat_t m;

    m.data[0][0] = v1.x;
    m.data[0][1] = v2.x;
    m.data[0][2] = v3.x;
    m.data[0][3] = 0.f;

    m.data[1][0] = v1.y;
    m.data[1][1] = v2.y;
    m.data[1][2] = v3.y;
    m.data[1][3] = 0.f;

    m.data[2][0] = v1.z;
    m.data[2][1] = v2.z;
    m.data[2][2] = v3.z;
    m.data[2][3] = 0.f;

    m.data[3][0] = 0.f;
    m.data[3][1] = 0.f;
    m.data[3][2] = 0.f;
    m.data[3][3] = 1.f;

    return m;
}

mat_t mat_translate(vec4_t v)
{
    mat_t m = mat_new_identity();
    m.data[0][3] = v.x;
    m.data[1][3] = v.y;
    m.data[2][3] = v.z;

    return m;
}

mat_t mat_add(mat_t m1, mat_t m2)
{
    mat_t mat;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            mat.data[i][j] = m1.data[i][j] + m2.data[i][j];
        }
    }
    return mat;
}

mat_t mat_sub(mat_t m1, mat_t m2)
{
    mat_t mat;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            mat.data[i][j] = m1.data[i][j] - m2.data[i][j];
        }
    }
    return mat;
}

mat_t mat_mul_mat(mat_t m1, mat_t m2)
{
    mat_t mat;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            float sum = 0.f;
            for (uint32_t x = 0; x < 4; x++)
            {
                sum += m1.data[i][x] * m2.data[x][j];
            }
            mat.data[i][j] = sum;
        }
    }
    return mat;
}

vec4_t mat_mul_vec(mat_t m, vec4_t v)
{
    vec4_t vec;
    vec.x = m.data[0][0] * v.x + m.data[0][1] * v.y + m.data[0][2] * v.z + m.data[0][3] * v.w;
    vec.y = m.data[1][0] * v.x + m.data[1][1] * v.y + m.data[1][2] * v.z + m.data[1][3] * v.w;
    vec.z = m.data[2][0] * v.x + m.data[2][1] * v.y + m.data[2][2] * v.z + m.data[2][3] * v.w;
    vec.w = m.data[3][0] * v.x + m.data[3][1] * v.y + m.data[3][2] * v.z + m.data[3][3] * v.w;
    return vec;
}

mat_t mat_inverse(mat_t m)
{
    /* https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix */
    
    mat_t mat;
    
    float a2323 = m.data[2][2] * m.data[3][3] - m.data[2][3] * m.data[3][2];
    float a1323 = m.data[2][1] * m.data[3][3] - m.data[2][3] * m.data[3][1];
    float a1223 = m.data[2][1] * m.data[3][2] - m.data[2][2] * m.data[3][1];
    float a0323 = m.data[2][0] * m.data[3][3] - m.data[2][3] * m.data[3][0];
    float a0223 = m.data[2][0] * m.data[3][2] - m.data[2][2] * m.data[3][0];
    float a0123 = m.data[2][0] * m.data[3][1] - m.data[2][1] * m.data[3][0];
    float a2313 = m.data[1][2] * m.data[3][3] - m.data[1][3] * m.data[3][2];
    float a1313 = m.data[1][1] * m.data[3][3] - m.data[1][3] * m.data[3][1];
    float a1213 = m.data[1][1] * m.data[3][2] - m.data[1][2] * m.data[3][1];
    float a2312 = m.data[1][2] * m.data[2][3] - m.data[1][3] * m.data[2][2];
    float a1312 = m.data[1][1] * m.data[2][3] - m.data[1][3] * m.data[2][1];
    float a1212 = m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1];
    float a0313 = m.data[1][0] * m.data[3][3] - m.data[1][3] * m.data[3][0];
    float a0213 = m.data[1][0] * m.data[3][2] - m.data[1][2] * m.data[3][0];
    float a0312 = m.data[1][0] * m.data[2][3] - m.data[1][3] * m.data[2][0];
    float a0212 = m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0];
    float a0113 = m.data[1][0] * m.data[3][1] - m.data[1][1] * m.data[3][0];
    float a0112 = m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0];

    float det = m.data[0][0] * ( m.data[1][1] * a2323 - m.data[1][2] * a1323 + m.data[1][3] * a1223 ) 
                - m.data[0][1] * ( m.data[1][0] * a2323 - m.data[1][2] * a0323 + m.data[1][3] * a0223 ) 
                + m.data[0][2] * ( m.data[1][0] * a1323 - m.data[1][1] * a0323 + m.data[1][3] * a0123 ) 
                - m.data[0][3] * ( m.data[1][0] * a1223 - m.data[1][1] * a0223 + m.data[1][2] * a0123 ) ;
    det = 1.f / det;

    mat.data[0][0] = det *   ( m.data[1][1] * a2323 - m.data[1][2] * a1323 + m.data[1][3] * a1223 );
    mat.data[0][1] = det * - ( m.data[0][1] * a2323 - m.data[0][2] * a1323 + m.data[0][3] * a1223 );
    mat.data[0][2] = det *   ( m.data[0][1] * a2313 - m.data[0][2] * a1313 + m.data[0][3] * a1213 );
    mat.data[0][3] = det * - ( m.data[0][1] * a2312 - m.data[0][2] * a1312 + m.data[0][3] * a1212 );
    mat.data[1][0] = det * - ( m.data[1][0] * a2323 - m.data[1][2] * a0323 + m.data[1][3] * a0223 );
    mat.data[1][1] = det *   ( m.data[0][0] * a2323 - m.data[0][2] * a0323 + m.data[0][3] * a0223 );
    mat.data[1][2] = det * - ( m.data[0][0] * a2313 - m.data[0][2] * a0313 + m.data[0][3] * a0213 );
    mat.data[1][3] = det *   ( m.data[0][0] * a2312 - m.data[0][2] * a0312 + m.data[0][3] * a0212 );
    mat.data[2][0] = det *   ( m.data[1][0] * a1323 - m.data[1][1] * a0323 + m.data[1][3] * a0123 );
    mat.data[2][1] = det * - ( m.data[0][0] * a1323 - m.data[0][1] * a0323 + m.data[0][3] * a0123 );
    mat.data[2][2] = det *   ( m.data[0][0] * a1313 - m.data[0][1] * a0313 + m.data[0][3] * a0113 );
    mat.data[2][3] = det * - ( m.data[0][0] * a1312 - m.data[0][1] * a0312 + m.data[0][3] * a0112 );
    mat.data[3][0] = det * - ( m.data[1][0] * a1223 - m.data[1][1] * a0223 + m.data[1][2] * a0123 );
    mat.data[3][1] = det *   ( m.data[0][0] * a1223 - m.data[0][1] * a0223 + m.data[0][2] * a0123 );
    mat.data[3][2] = det * - ( m.data[0][0] * a1213 - m.data[0][1] * a0213 + m.data[0][2] * a0113 );
    mat.data[3][3] = det *   ( m.data[0][0] * a1212 - m.data[0][1] * a0212 + m.data[0][2] * a0112 );

    return mat;
}

mat_t mat_transpose(mat_t m)
{
    mat_t mat;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            mat.data[i][j] = m.data[j][i];
        }
    }
    return mat;
}

void mat_print(mat_t m)
{
    printf("mat\n");
    printf("|%.3f, %.3f, %.3f, %.3f|\n", m.data[0][0], m.data[0][1], m.data[0][2], m.data[0][3]);
    printf("|%.3f, %.3f, %.3f, %.3f|\n", m.data[1][0], m.data[1][1], m.data[1][2], m.data[1][3]);
    printf("|%.3f, %.3f, %.3f, %.3f|\n", m.data[2][0], m.data[2][1], m.data[2][2], m.data[2][3]);
    printf("|%.3f, %.3f, %.3f, %.3f|\n", m.data[3][0], m.data[3][1], m.data[3][2], m.data[3][3]);
}

mat_t x_axis_rotation(float rad)
{
    float sin_val   = sinf(rad);
    float cos_val   = cosf(rad);
    
    mat_t mat       = mat_new_identity();

    mat.data[1][1]  = cos_val;
    mat.data[1][2]  = -sin_val;

    mat.data[2][1]  = sin_val;
    mat.data[2][2]  = cos_val;

    return mat;
}

mat_t y_axis_rotation(float rad)
{
    float sin_val   = sinf(rad);
    float cos_val   = cosf(rad);
    
    mat_t mat       = mat_new_identity();

    mat.data[0][0]  = cos_val;
    mat.data[0][2]  = sin_val;

    mat.data[2][0]  = -sin_val;
    mat.data[2][2]  = cos_val;

    return mat;
}

mat_t z_axis_rotation(float rad)
{
    float sin_val   = sinf(rad);
    float cos_val   = cosf(rad);

    mat_t mat       = mat_new_identity();

    mat.data[0][0]  = cos_val;
    mat.data[0][1]  = -sin_val;

    mat.data[1][0]  = sin_val;
    mat.data[1][1]  = cos_val;

    return mat;
}

float deg_to_rad(float deg)
{
    return deg * F_PI_OVER_180;
}

float rad_to_deg(float rad)
{
    return rad * F_180_OVER_PI;
}

/********************/
/*    Utilities     */
/********************/

float f_abs(float a)
{
    return (float)fabs(a);
}

float f_min(float a, float b)
{
    return a < b ? a : b;
}

float f_max(float a, float b)
{
    return a > b ? a : b;
}

float f_clamp(float v, float min, float max)
{
    return f_min(max, f_max(min, v));
}

float f_wrap(float v, float min, float max)
{
    float diff = max - min;

    while (v > max)
    {
        v = v - diff;
    }

    while (v < min)
    {
        v = v + diff;
    }

    return v;
}

float f_sin(float v)
{
    return sinf(v);
}

float f_cos(float v)
{
    return cosf(v);
}

float f_floor(float v)
{
    return floorf(v);
}

float f_ceil(float v)
{
    return ceilf(v);
}

float f_round(float v)
{
    return roundf(v);
}

float f_pow(float v, float pow)
{
    return powf(v, pow);
}

int32_t i_max(int32_t a, int32_t b)
{
    return a > b ? a : b;
}

int32_t i_min(int32_t a, int32_t b)
{
    return a < b ? a : b;
}

uint32_t u_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}

uint32_t u_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}
