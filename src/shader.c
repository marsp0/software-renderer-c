#include "shader.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "texture.h"
#include "constants.h"

/********************
 *  Notes
 *
 * - disney pbr paper   - https://media.disneyanimation.com/uploads/production/publication_asset/48/asset/s2012_pbs_disney_brdf_notes_v3.pdf
 * - unreal pbr paper   - https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
 * - LearnOpengl PBR    - https://learnopengl.com/PBR/Theory
 * - Specular BRDF ref  - http://graphicrants.blogspot.nl/2013/08/specular-brdf-reference.html
 * - Tangent space      - https://learnopengl.com/Advanced-Lighting/Normal-Mapping
 * - Skybox algorithm   - https://registry.khronos.org/OpenGL/specs/gl/glspec46.core.pdf (Section 8.13)
 * - attr interpolation - https://registry.khronos.org/OpenGL/specs/gl/glspec46.core.pdf (Section 14.6)
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

static const float gamma_val        = 2.2f;
static const float one_over_gamma   = 1.f / gamma_val;
static camera_t* camera             = NULL;
static vec4_t cam_w;

// pbr textures
static texture_t* albedo_texture    = NULL;
static texture_t* metallic_texture  = NULL;
static texture_t* normal_texture    = NULL;

// skybox textures
static texture_t* x_positive        = NULL;
static texture_t* x_negative        = NULL;
static texture_t* y_positive        = NULL;
static texture_t* y_negative        = NULL;
static texture_t* z_positive        = NULL;
static texture_t* z_negative        = NULL;

// shared attributes
static vec4_t v0_w;
static vec4_t v1_w;
static vec4_t v2_w;
static vec2_t t0;
static vec2_t t1;
static vec2_t t2;
static vec4_t n0;
static vec4_t n1;
static vec4_t n2;

/********************/
/* static functions */
/********************/

static vec4_t vec4_mix(vec4_t v0, vec4_t v1, float mix)
{
    float x = v0.x * (1 - mix) + v1.x * mix;
    float y = v0.y * (1 - mix) + v1.y * mix;
    float z = v0.z * (1 - mix) + v1.z * mix;

    return vec4_new(x, y, z);
}

// static vec4_t sample_normal(texture_t* tex, float u, float v)
// {
//     // fixme: this second sample method is needed because of the whole BGRA thing.
//     //        Once a clear boundry is introduced and RGBA has been standardized in the renderer then this can be removed
//     uint32_t u_idx = (uint32_t)(u * (float)tex->width) - 1;
//     uint32_t v_idx = (uint32_t)(v * (float)tex->height) - 1;

//     vec4_t temp = vec4_from_bgra(texture_get(tex, u_idx, v_idx));

//     return vec4_new(temp.z * 2.f - 1.f,
//                     temp.y * 2.f - 1.f,
//                     temp.x * 2.f - 1.f);
// }


static float schlick_ggx(float dot, float k)
{
    return dot / (dot * (1 - k) + k);
}


// Distribution function describing how much microfacet normals are aligned to the half vector
static float normal_dist(float n_dot_h, float r)
{
    // Trowbridge-Reitz GGX
    float r_sq_sq   = r * r * r * r;
    float b         = n_dot_h * n_dot_h * (r_sq_sq - 1.f) + 1.f;
    float denom     = F_PI * b * b;
    float result    = r_sq_sq / denom;

    return result;
}


// Geometry function describing self shadowing
static float self_shadow(float n_dot_v, float n_dot_l, float r)
{
    // Smith + Schlick-GGX
    // float k_direct  = (r + 1) * (r + 1) / 8;
    float k_direct  = (r * r) / 2.f;
    // float k_direct  = (r * r) / 8.f;
    // float k_ibl     = a * a / 2
    float ggx_1     = schlick_ggx(n_dot_v, k_direct);
    float ggx_2     = schlick_ggx(n_dot_l, k_direct);
    float result    = ggx_1 * ggx_2;

    return result;
}


// Fresnel factor describing ratio of reflected vs refracted light
static vec4_t fresnel(float h_dot_v, vec4_t a, float m)
{
    vec4_t one      = vec4_new(1.f, 1.f, 1.f);

    // Fresnel-Schlick approx
    vec4_t f0       = vec4_from_scalar(0.04f);
    f0              = vec4_mix(f0, a, m);
    vec4_t diff     = vec4_sub(one, f0);
    float exp       = (float)pow(f_clamp(1.f - h_dot_v, 0.f, 1.f), 5.f);
    diff            = vec4_scale(diff, exp);
    vec4_t result   = vec4_add(f0, diff);

    return result;
}


/********************/
/* public functions */
/********************/

void shader_set_uniforms(camera_t* cam,
                         texture_t* albedo_tex,
                         texture_t* metallic_tex,
                         texture_t* normal_tex,
                         vec4_t v0, 
                         vec4_t v1, 
                         vec4_t v2,
                         vec2_t tex_coord0,
                         vec2_t tex_coord1,
                         vec2_t tex_coord2,
                         vec4_t normal_vec0,
                         vec4_t normal_vec1,
                         vec4_t normal_vec2)
{
    camera              = cam;
    cam_w               = camera->position_w;

    albedo_texture      = albedo_tex;
    metallic_texture    = metallic_tex;
    normal_texture      = normal_tex;

    mat_t M             = mat_new_identity();
    v0_w                = mat_mul_vec(M, v0);
    v1_w                = mat_mul_vec(M, v1);
    v2_w                = mat_mul_vec(M, v2);

    t0                  = tex_coord0;
    t1                  = tex_coord1;
    t2                  = tex_coord2;

    n0                  = normal_vec0;
    n1                  = normal_vec1;
    n2                  = normal_vec2;
}


void shader_set_uniforms_skybox(camera_t* cam,
                                texture_t* x_pos,
                                texture_t* x_neg,
                                texture_t* y_pos,
                                texture_t* y_neg,
                                texture_t* z_pos,
                                texture_t* z_neg,
                                vec4_t v0,
                                vec4_t v1,
                                vec4_t v2,
                                vec2_t tex_coord0,
                                vec2_t tex_coord1,
                                vec2_t tex_coord2)
{
    camera = cam;
    x_positive = x_pos;
    x_negative = x_neg;
    y_positive = y_pos;
    y_negative = y_neg;
    z_positive = z_pos;
    z_negative = z_neg;

    t0                  = tex_coord0;
    t1                  = tex_coord1;
    t2                  = tex_coord2;

    mat_t M             = mat_new_identity();
    v0_w                = mat_mul_vec(M, v0);
    v1_w                = mat_mul_vec(M, v1);
    v2_w                = mat_mul_vec(M, v2);
}


vec4_t shader_vertex(vec4_t v)
{
    mat_t M         = mat_new_identity();
    mat_t V         = camera_view_mat(camera);
    mat_t P         = camera_proj_mat(camera);
    mat_t VM        = mat_mul_mat(V, M);
    mat_t PVM       = mat_mul_mat(P, VM);
    
    return mat_mul_vec(PVM, v);
}


vec4_t shader_vertex_skybox(vec4_t v)
{
    mat_t M         = mat_new_identity();
    mat_t V         = camera_view_mat(camera);
    mat_t P         = camera_proj_mat(camera);
    mat_t VM        = mat_mul_mat(V, M);
    mat_t PVM       = mat_mul_mat(P, VM);
    // PVM.data[0][3] = 0.f;
    // PVM.data[1][3] = 0.f;
    // PVM.data[2][3] = 0.f;
    // PVM.data[3][3] = NEAR_PLANE;
    
    return mat_mul_vec(PVM, v);
}


uint32_t shader_fragment(float w, float w0, float w1, float w2)
{
    vec4_t one          = vec4_from_scalar(1.f);

    float s             = f_min((t0.x * w0 + t1.x * w1 + t2.x * w2) / w, 1.f);
    float t             = f_min((t0.y * w0 + t1.y * w1 + t2.y * w2) / w, 1.f);

    vec4_t albedo       = vec4_pow(texture_sample(albedo_texture, s, t, get_texture_filter()), gamma_val);
    vec4_t metallic     = texture_sample(metallic_texture, s, t, get_texture_filter());
    float rough         = metallic.y;                                       // green channel
    float metal         = metallic.x;                                       // blue channel
    // vec_t o             = vec_from_bgra(sample(tri->occlusion, s, t));

    // vec4_t n_t          = vec4_normalize(sample_normal(normal_texture, s, t));
    vec4_t n_w          = vec4_scale(n0, w0);
    n_w                 = vec4_add(n_w, vec4_scale(n1, w1));
    n_w                 = vec4_add(n_w, vec4_scale(n2, w2));
    n_w                 = vec4_normalize(n_w);
    
    // float du1       = t1.x - t0.x;
    // float du2       = t2.x - t0.x;
    // float dv1       = t1.y - t0.y;
    // float dv2       = t2.y - t0.y;
    // vec4_t e1       = vec4_sub(v1_w, v0_w);
    // vec4_t e2       = vec4_sub(v2_w, v0_w);

    // float dnm       = du1 * dv2 - du2 * dv1;
    // float tx        = (e1.x * dv2 - e2.x * dv1) / dnm;
    // float ty        = (e1.y * dv2 - e2.y * dv1) / dnm;
    // float tz        = (e1.z * dv2 - e2.z * dv1) / dnm;
    // vec4_t t_w      = vec4_normalize(vec4_new(tx, ty, tz));
    // vec4_t b_w      = vec4_normalize(vec4_cross(n_w, t_w));
    // t_w             = vec4_normalize(vec4_cross(b_w, n_w));
    // tbn_t_w         = mat_from_vec4(t_w, b_w, n_w);

    // interpolate per vertex vars
    vec4_t pos_w;
    pos_w               = vec4_scale(v0_w, w0 / w);
    pos_w               = vec4_add(pos_w, vec4_scale(v1_w, w1 / w));
    pos_w               = vec4_add(pos_w, vec4_scale(v2_w, w2 / w));

    vec4_t view_w       = vec4_normalize(vec4_sub(cam_w, pos_w));
    vec4_t light_w      = vec4_normalize(one);
    vec4_t halfway_w    = vec4_normalize(vec4_add(view_w, light_w));

    float n_dot_h       = f_max(vec4_dot(n_w, halfway_w),       0.f);
    float n_dot_v       = f_max(vec4_dot(n_w, view_w),          0.f);
    float n_dot_l       = f_max(vec4_dot(n_w, light_w),         0.f);
    float h_dot_v       = f_max(vec4_dot(halfway_w, view_w),    0.f);

    // specular
    //      F * D * G
    // ---------------------
    // 4 * n_dot_l * n_dot_v

    float d             = normal_dist(n_dot_h, rough);
    float g             = self_shadow(n_dot_v, n_dot_l, rough);
    vec4_t f            = fresnel(h_dot_v, albedo, metal);
    float dg            = ( d * g ) / ( 4 * n_dot_l * n_dot_v + 0.001f );
    vec4_t specular     = vec4_scale(f, dg);

    // diffuse
    //   kd * c
    //  --------
    //     pi

    vec4_t kd           = vec4_sub(one, f);
    kd                  = vec4_scale(kd, 1.f - metal);
    kd                  = vec4_scale(kd, 1.f/F_PI);
    vec4_t diffuse      = vec4_hadamard(albedo, kd);

    vec4_t col          = vec4_add(diffuse, specular);
    col                 = vec4_scale(col, 1.f);
    col                 = vec4_scale(col, n_dot_l);

    // ambient + gamma correction

    vec4_t ambient      = vec4_scale(albedo, 0.05f);
    vec4_t final        = vec4_pow(vec4_add(col, ambient), one_over_gamma);

    return vec4_to_bgra(final);
}


uint32_t shader_fragment_skybox(float w, float w0, float w1, float w2)
{

    float sc;
    float tc;
    float ma;
    vec4_t pos_w;
    texture_t* texture;

    pos_w           = vec4_scale(v0_w, w0 / w);
    pos_w           = vec4_add(pos_w, vec4_scale(v1_w, w1 / w));
    pos_w           = vec4_add(pos_w, vec4_scale(v2_w, w2 / w));
    pos_w           = vec4_normalize(pos_w);

    pos_w           = vec4_scale(v0_w, w0);
    pos_w           = vec4_add(pos_w, vec4_scale(v1_w, w1));
    pos_w           = vec4_add(pos_w, vec4_scale(v2_w, w2));
    pos_w           = vec4_normalize(pos_w);

    // determine major axis
    vec4_t abs_pos = vec4_abs(pos_w);

    if (abs_pos.x > abs_pos.y && abs_pos.x > abs_pos.z)
    {
        // x major
        sc          = -pos_w.z;
        tc          = -pos_w.y;
        ma          = pos_w.x;
        texture     = x_positive;

        if (pos_w.x < 0.f)
        {
            sc      = pos_w.z;
            texture = x_negative;
        }
    }
    else if (abs_pos.y > abs_pos.x && abs_pos.y > abs_pos.z)
    {
        // y major
        sc          = pos_w.x;
        tc          = pos_w.z;
        ma          = pos_w.y;
        texture     = y_positive;

        if (pos_w.y < 0.f)
        {
            tc      = -pos_w.z;
            texture = y_negative;
        }
    }
    else
    {
        // z major
        sc          = pos_w.x;
        tc          = -pos_w.y;
        ma          = pos_w.z;
        texture     = z_positive;

        if (pos_w.z < 0.f)
        {
            sc      = -pos_w.x;
            texture = z_negative;
        }
    }

    float s = 0.5 * (sc / f_abs(ma) + 1.f);
    float t = 0.5 * (tc / f_abs(ma) + 1.f);
    
    s             = (t0.x * w0 + t1.x * w1 + t2.x * w2) / w;
    t             = (t0.y * w0 + t1.y * w1 + t2.y * w2) / w;

    // vec4_t red = vec4_new(0.f, 0.f, 1.f);
    // vec4_t green = vec4_new(0.f, 1.f, 0.f);
    // vec4_t blue = vec4_new(1.f, 0.f, 0.f);
    // float x = (red.x * w2 + green.x * w1 + blue.x * w0) / w;
    // float y = (red.y * w2 + green.y * w1 + blue.y * w0) / w;
    // float z = (red.z * w2 + green.z * w1 + blue.z * w0) / w;
    // return vec4_to_bgra(vec4_new(x, y,z));

    return vec4_to_bgra(texture_sample(texture, s, t, POINT_SAMPLE));
}
