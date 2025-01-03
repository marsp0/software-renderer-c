#include "renderer.h"

#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "linux/input.h"
#include "time_utils.h"
#include "rasterizer.h"
#include "constants.h"
#include "shader.h"
#include "settings.h"

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

static scene_t* scene               = NULL;
static display_t* display           = NULL;
static framebuffer_t* front         = NULL;
static framebuffer_t* back          = NULL;
static framebuffer_t* current       = NULL;
static depthbuffer_t* depthbuffer   = NULL;
static bool wireframe               = false;

/********************/
/* static functions */
/********************/

static void renderer_update(input_t input)
{
    scene_update(scene, input);

    if (input.keys & KEY_2) { change_texture_filter(); }
}

// static void renderer_draw_utilities()
// {
//     float w         = (float)width;
//     float h         = (float)height;
//     camera_t* cam   = scene->camera;

//     vec4_t points[4]    = {vec4_new(0.f, 0.f, 0.f),
//                            vec4_new(1.f, 0.f, 0.f),
//                            vec4_new(0.f, 1.f, 0.f),
//                            vec4_new(0.f, 0.f, 1.f)};

//     uint32_t colors[4] = {0x00000000, 
//                           0x0000FF00, 
//                           0x00FF0000, 
//                           0xFF000000};

//     mat_t PV = mat_mul_mat(camera_proj_mat(cam),
//                            camera_view_mat(cam));

//     for (uint32_t i = 0; i < sizeof(points) / sizeof(vec4_t); i++)
//     {
//         points[i]   = mat_mul_vec(PV, points[i]);
//         points[i]   = vec4_scale(points[i], 1.f/points[i].w);
//         points[i].x = (points[i].x + 1.f) * 0.5f * w;
//         points[i].y = (points[i].y + 1.f) * 0.5f * h;
//     }

//     rasterizer_draw_line(points[0], points[1], colors[1], current);
//     rasterizer_draw_line(points[0], points[2], colors[2], current);
//     rasterizer_draw_line(points[0], points[3], colors[3], current);
// }

static void renderer_draw_mesh(mesh_t* mesh)
{

    uint32_t i0;
    uint32_t i1;
    uint32_t i2;

    vec4_t v0;
    vec4_t v1;
    vec4_t v2;

    vec2_t t0;
    vec2_t t1;
    vec2_t t2;

    vec4_t n0;
    vec4_t n1;
    vec4_t n2;

    float w_over_2          = (float)WINDOW_WIDTH * 0.5f;
    float h_over_2          = (float)WINDOW_HEIGHT * 0.5f;

    camera_t* cam           = scene->camera;

    uint32_t indices_size   = mesh->indices_size;
    uint32_t* indices       = mesh->indices;
    vec4_t* vertices        = mesh->vertices;
    vec2_t* texcoords       = mesh->texcoords;
    vec4_t* normals         = mesh->normals;

    rasterizer_set_fragment_shader(shader_fragment);

    for (uint32_t i = 0; i < indices_size; i += 3)
    {
        i0 = indices[i + 0];
        i1 = indices[i + 1];
        i2 = indices[i + 2];

        v0 = vertices[i0];
        v1 = vertices[i1];
        v2 = vertices[i2];

        t0 = texcoords[i0];
        t1 = texcoords[i1];
        t2 = texcoords[i2];

        n0 = normals[i0];
        n1 = normals[i1];
        n2 = normals[i2];

        // backface cull
        if (vec4_dot(n0, cam->forward) < 0.f)
        {
            continue;
        }

        // set shader utils
        shader_set_uniforms(cam,
                            mesh->albedo,
                            mesh->metallic,
                            mesh->normal,
                            v0, v1, v2,
                            t0, t1, t2,
                            n0, n1, n2);

        // run vertex shader
        v0 = shader_vertex(v0);
        v1 = shader_vertex(v1);
        v2 = shader_vertex(v2);

        // persp divide
        v0 = vec4_scale(v0, 1.f / v0.w);
        v1 = vec4_scale(v1, 1.f / v1.w);
        v2 = vec4_scale(v2, 1.f / v2.w);

        // viewport transform
        v0.x = (v0.x + 1.f) * w_over_2;
        v0.y = (v0.y + 1.f) * h_over_2;
        v1.x = (v1.x + 1.f) * w_over_2;
        v1.y = (v1.y + 1.f) * h_over_2;
        v2.x = (v2.x + 1.f) * w_over_2;
        v2.y = (v2.y + 1.f) * h_over_2;

        rasterizer_draw_triangle(v0, v1, v2, current, depthbuffer);
    }
}

static void renderer_draw()
{
    // renderer_draw_utilities();

    renderer_draw_mesh(scene->mesh);

    display_draw(display, current);
}

static void renderer_clear_buffers()
{
    framebuffer_clear(current);
    depthbuffer_clear(depthbuffer);
    display_clear(display);
}

/********************/
/* public functions */
/********************/

void renderer_init()
{
    display       = display_new();

    front         = framebuffer_new(WINDOW_WIDTH, WINDOW_HEIGHT);
    back          = framebuffer_new(WINDOW_WIDTH, WINDOW_HEIGHT);
    current       = front;
    depthbuffer   = depthbuffer_new(WINDOW_WIDTH, WINDOW_HEIGHT);
    wireframe     = false;
}

void renderer_load(const char* file_path)
{
    if (scene)
    {
        scene_free(scene);
    }
    
    scene = scene_new(file_path);
}

void renderer_run()
{
    bool quit = false;
    timestamp_t start;
    timestamp_t end;
    timestamp_t diff;
    timestamp_t frame_time = 16 * MILLISECOND;
    struct timespec sleep_timer = { 0, 0 };

    while (!quit)
    {
        start = time_now();

        input_t input = handle_input(display);

        renderer_update(input);

        renderer_draw();

        renderer_clear_buffers();

        current = current == front ? back : front;

        // maintain 60fps
        end = time_now();
        diff = end - start;

        if (frame_time > diff)
        {
            sleep_timer.tv_nsec = frame_time - diff;
            nanosleep(&sleep_timer, NULL);
        }

        printf("%ldus\n", diff / 1000);

        // quit
        quit = input.keys & QUIT;
    }
}

void renderer_free()
{
    if (scene)
    {
        scene_free(scene);
    }
    display_free(display);
    framebuffer_free(front);
    framebuffer_free(back);
    depthbuffer_free(depthbuffer);
}