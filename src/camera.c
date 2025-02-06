#include "camera.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"

/*
 * Notes
 * 
 * marsp0.github.io/articles/deriving-camera-transform.html
 * 
 *
 * Reverse Z buffer
 * https://developer.nvidia.com/content/depth-precision-visualized
 * http://www.geometry.caltech.edu/pubs/UD12.pdf
 * http://www.humus.name/Articles/Persson_CreatingVastGameWorlds.pdf
 * http://www.terathon.com/gdc07_lengyel.pdf
 * https://www.danielecarbone.com/reverse-depth-buffer-in-opengl/
 * https://stackoverflow.com/questions/63084469/when-and-how-does-opengl-calculate-f-depthdepth-value
 * https://stackoverflow.com/questions/63096579/how-does-opengl-come-to-the-formula-f-depth-and-and-is-this-the-window-viewport
 */

/********************/
/*      defines     */
/********************/

#define PI_OVER_180 0.017453292f
#define MAX_PITCH   1.55334f

/********************/
/* static variables */
/********************/

static const vec4_t world_up    = { 0.f, 1.f, 0.f, 1.f };
static const float d_a          = 3.f;
static const float d_z          = 0.05f;

/********************/
/* static functions */
/********************/

static void camera_generate_basis(camera_t* cam)
{
    // generate new basis vectors
    float phi       = cam->phi;
    float theta     = cam->theta;
    float radius    = cam->radius;
    float sin_phi   = f_sin(phi);
    float cos_phi   = f_cos(phi);
    float sin_theta = f_sin(theta);
    float cos_theta = f_cos(theta);
    float x         = radius * sin_phi * cos_theta;
    float y         = radius * cos_phi;
    float z         = radius * sin_phi * sin_theta;

    vec4_t pos_w    = vec4_add(cam->target_w, vec4_new(x, y, z));
    vec4_t forward  = vec4_normalize(vec4_new(x, y, z));
    vec4_t left     = vec4_normalize(vec4_cross(world_up, forward));
    vec4_t up       = vec4_normalize(vec4_cross(forward, left));

    cam->position_w = pos_w;
    cam->forward    = forward;
    cam->left       = left;
    cam->up         = up;
}

/********************/
/* public functions */
/********************/

camera_t* camera_new(vec4_t target,
                     float phi,
                     float theta,
                     float radius,
                     float fov_x,
                     float near,
                     float far,
                     float aspect_ratio)
{
    camera_t* camera    = malloc(sizeof(camera_t));

    camera->radius      = radius;
    camera->phi         = phi;
    camera->theta       = theta;
    camera->target_w    = target;
    camera->forward     = vec4_new(0.f, 0.f, -1.f);
    camera->left        = vec4_new(1.f, 0.f, 0.f);
    camera->up          = vec4_new(0.f, 1.f, 0.f);

    camera->fov_x       = fov_x;
    camera->asp_ratio   = aspect_ratio;
    camera->n_dist      = near;
    camera->f_dist      = far;
    camera->r_dist      = tanf(fov_x / 2.f) * near;
    camera->l_dist      = -camera->r_dist;
    camera->t_dist      = camera->r_dist / aspect_ratio;
    camera->b_dist      = -camera->t_dist;

    camera_generate_basis(camera);

    return camera;
}

void camera_update(camera_t* cam, input_t input)
{
    int32_t dx = input.curr_x - input.prev_x;
    int32_t dy = input.curr_y - input.prev_y;

    // update orientation
    if (input.keys & LEFT_CLICK)
    {
        float phi   = cam->phi;
        float theta = cam->theta;

        phi        -= deg_to_rad((float)dy * d_a);
        phi         = f_clamp(phi, 0.001f, F_PI - 0.001f);

        theta      += deg_to_rad((float)dx * d_a);
        theta       = f_wrap(theta, 0.f, 2 * F_PI);

        cam->phi    = phi;
        cam->theta  = theta;
    }

    // update radius
    float radius = cam->radius;

    if      (input.keys & SCROLL_UP)    { radius = radius - d_z; }
    else if (input.keys & SCROLL_DOWN)  { radius = radius + d_z; }

    cam->radius = f_max(0.02f, radius);

    // update target position
    if (input.keys & RIGHT_CLICK)
    {
        vec4_t target_w = cam->target_w;
        vec4_t n        = vec4_negate(cam->forward);

        vec4_t p1       = vec4_new((float)input.curr_x, -(float)input.curr_y, 1.f);
        vec4_t p2       = vec4_new((float)input.prev_x, -(float)input.prev_y, 1.f);

        mat_t P         = camera_proj_mat(cam);
        mat_t V         = camera_view_mat(cam);
        mat_t P_inv     = mat_inverse(P);
        mat_t V_inv     = mat_inverse(V);

        p1              = mat_mul_vec(P_inv, p1);
        p1              = mat_mul_vec(V_inv, p1);
        p1.w            = 1.f/p1.w;
        p1              = vec4_scale(p1, p1.w);
        vec4_t tar_p1   = vec4_sub(p1, target_w);
        vec4_t n_p1     = vec4_scale(n, vec4_dot(tar_p1, n));
        vec4_t q1       = vec4_sub(tar_p1, n_p1);

        p2              = mat_mul_vec(P_inv, p2);
        p2              = mat_mul_vec(V_inv, p2);
        p2.w            = 1.f/p2.w;
        p2              = vec4_scale(p2, p2.w);
        vec4_t tar_p2   = vec4_sub(p2, target_w);
        vec4_t n_p2     = vec4_scale(n, vec4_dot(tar_p2, n));
        vec4_t q2       = vec4_sub(tar_p2, n_p2);

        cam->target_w = vec4_add(target_w, vec4_scale(vec4_sub(q2, q1), 0.12f));
    }

    // handle reset
    if (input.keys & KEY_1)
    {
        cam->target_w   = vec4_from_scalar(0.f);
        cam->phi        = F_PI / 2.f;
        cam->theta      = 0.1f;
        cam->radius     = 0.5f;
    }

    camera_generate_basis(cam);
}

mat_t camera_view_mat(camera_t* cam)
{
    mat_t result        = mat_new_identity();

    vec4_t pos_negative = vec4_negate(cam->position_w);
    vec4_t forward      = cam->forward;
    vec4_t left         = cam->left;
    vec4_t up           = cam->up;

    // construct cam -> world transform + invert it
    result.data[0][0]   = left.x;
    result.data[0][1]   = left.y;
    result.data[0][2]   = left.z;
    result.data[0][3]   = vec4_dot(pos_negative, left);

    result.data[1][0]   = up.x;
    result.data[1][1]   = up.y;
    result.data[1][2]   = up.z;
    result.data[1][3]   = vec4_dot(pos_negative, up);

    result.data[2][0]   = forward.x;
    result.data[2][1]   = forward.y;
    result.data[2][2]   = forward.z;
    result.data[2][3]   = vec4_dot(pos_negative, forward);

    return result;
}

mat_t camera_proj_mat(camera_t* cam)
{
    mat_t result = mat_new_identity();

    float n_over_r  = cam->n_dist / cam->r_dist;
    float n_over_t  = cam->n_dist / cam->t_dist;
    float f_min_n   = cam->f_dist - cam->n_dist;
    float f_plus_n  = cam->f_dist + cam->n_dist;
    float fn        = cam->n_dist * cam->f_dist;

    result.data[0][0] = n_over_r;
    result.data[1][1] = n_over_t;
    result.data[2][2] = -f_plus_n / f_min_n;
    result.data[2][3] = -2.f * fn / f_min_n;
    result.data[3][2] = -1.f;
    result.data[3][3] = 0.f;

    return result;
}

void camera_free(camera_t* cam)
{
    free(cam);
}
