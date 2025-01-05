#pragma once

#include <stdint.h>

/********************/
/*     Defines      */
/********************/

#define F_PI            3.1415927410125f
#define F_PI_2          1.5707963705062f
#define F_PI_OVER_180   0.0174532925199f
#define F_180_OVER_PI   57.2957795131f

typedef float rad_t;

/********************/
/*      Vector      */
/********************/

typedef struct
{
    float x;
    float y;
} vec2_t;

typedef struct
{
    float x;
    float y;
    float z;
} vec3_t;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vec4_t;

vec2_t      vec2_new(float x, float y);
vec2_t      vec2_from_scalar(float s);
vec2_t      vec2_add(vec2_t v1, vec2_t v2);
vec2_t      vec2_sub(vec2_t v1, vec2_t v2);
vec2_t      vec2_scale(vec2_t v, float scale);
vec2_t      vec2_negate(vec2_t v);
vec2_t      vec2_normalize(vec2_t v);
vec2_t      vec2_hadamard(vec2_t v1, vec2_t v2);
vec2_t      vec2_clamp(vec2_t v, float min, float max);
float       vec2_dot(vec2_t v1, vec2_t v2);
float       vec2_magnitude(vec2_t v);
float       vec2_magnitude_sq(vec2_t v);
void        vec2_print(vec2_t v);

vec3_t      vec3_new(float x, float y, float z);
vec3_t      vec3_from_scalar(float s);
vec3_t      vec3_add(vec3_t v1, vec3_t v2);
vec3_t      vec3_sub(vec3_t v1, vec3_t v2);
vec3_t      vec3_cross(vec3_t v1, vec3_t v2);
vec3_t      vec3_scale(vec3_t v, float scale);
vec3_t      vec3_negate(vec3_t v);
vec3_t      vec3_normalize(vec3_t v);
vec3_t      vec3_hadamard(vec3_t v1, vec3_t v2);
vec3_t      vec3_clamp(vec3_t v, float min, float max);
float       vec3_dot(vec3_t v1, vec3_t v2);
float       vec3_magnitude(vec3_t v);
float       vec3_magnitude_sq(vec3_t v);
void        vec3_print(vec3_t v);

vec4_t      vec4_new(float x, float y, float z);
vec4_t      vec4_from_bgra(uint32_t c);
uint32_t    vec4_to_bgra(vec4_t c);
vec4_t      vec4_from_scalar(float s);
vec4_t      vec4_add(vec4_t v1, vec4_t v2);
vec4_t      vec4_sub(vec4_t v1, vec4_t v2);
vec4_t      vec4_cross(vec4_t v1, vec4_t v2);
vec4_t      vec4_scale(vec4_t v, float scale);
vec4_t      vec4_scale_with_w(vec4_t v, float scale);
vec4_t      vec4_pow(vec4_t v, float pow);
vec4_t      vec4_abs(vec4_t v);
vec4_t      vec4_negate(vec4_t v);
vec4_t      vec4_normalize(vec4_t v);
vec4_t      vec4_hadamard(vec4_t v1, vec4_t v2);
vec4_t      vec4_clamp(vec4_t v, float min, float max);
float       vec4_dot(vec4_t v1, vec4_t v2);
float       vec4_magnitude(vec4_t v);
float       vec4_magnitude_sq(vec4_t v);
void        vec4_print(vec4_t v, const char* name);


/********************/
/*       Matrix     */
/********************/

typedef struct
{
    float data[4][4];
} mat_t;

mat_t   mat_new(float a, float b, float c, float d,
                float e, float f, float g, float h,
                float i, float j, float k, float l,
                float m, float n, float o, float p);
mat_t   mat_new_identity();
mat_t   mat_from_vec4(vec4_t v1, vec4_t v2, vec4_t v3);
mat_t   mat_translate(vec4_t v);
mat_t   mat_add(mat_t m1, mat_t m2);
mat_t   mat_sub(mat_t m1, mat_t m2);
mat_t   mat_mul_mat(mat_t m1, mat_t m2);
vec4_t  mat_mul_vec(mat_t m, vec4_t v);
mat_t   mat_inverse(mat_t m);
mat_t   mat_transpose(mat_t m);
void    mat_print(mat_t m);


/********************/
/*    Quaternion    */
/********************/

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} quat_t;

quat_t quat_identity();
quat_t quat_new(float x, float y, float z, float w);


/********************/
/*  Rotation Utils  */
/********************/

mat_t x_axis_rotation(float rad);
mat_t y_axis_rotation(float rad);
mat_t z_axis_rotation(float rad);
float deg_to_rad(float deg);
float rad_to_deg(float rad);


/********************/
/*       Plane      */
/********************/

typedef struct
{
    vec4_t p;
    vec4_t n;
} plane_t;


/********************/
/*      Sphere      */
/********************/

typedef struct
{
    vec4_t c;
    float r;
} sphere_t;


/********************/
/*    Utilities     */
/********************/

float       f_abs(float a);
float       f_min(float a, float b);
float       f_max(float a, float b);
float       f_ceil(float v);
float       f_floor(float v);
float       f_round(float v);
float       f_clamp(float v, float min, float max);
float       f_wrap(float v, float min, float max);
float       f_sin(float v);
float       f_cos(float v);
float       f_pow(float v, float pow);
int32_t     i_max(int32_t a, int32_t b);
int32_t     i_min(int32_t a, int32_t b);
uint32_t    u_max(uint32_t a, uint32_t b);
uint32_t    u_min(uint32_t a, uint32_t b);
