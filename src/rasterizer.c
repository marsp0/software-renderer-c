#include "rasterizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "shader.h"

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

static uint32_t (*fragment_shader)(float, float, float, float)  = NULL;
static bool depth_test                                          = true;

/********************/
/* static functions */
/********************/

static int32_t edge_check(int32_t x0, int32_t y0, 
                          int32_t x1, int32_t y1, 
                          int32_t x2, int32_t y2)
{
    return (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
}

/********************/
/* public functions */
/********************/

void rasterizer_set_fragment_shader(uint32_t (*shader)(float, float, float, float))
{
    fragment_shader = shader;
}

void rasterizer_set_depth_test(bool val)
{
    depth_test = val;
}

void rasterizer_draw_line(vec4_t v0,
                          vec4_t v1,
                          uint32_t color, 
                          framebuffer_t* framebuffer)
{
    int32_t x0 = (int32_t)v0.x;
    int32_t y0 = (int32_t)v0.y;
    int32_t x1 = (int32_t)v1.x;
    int32_t y1 = (int32_t)v1.y;
    int32_t temp = x0;
    bool steep = false;

    // draw by iterating over the longer side
    if (abs(x1 - x0) < abs(y1 - y0))
    {
        temp = x0;
        x0   = y0;
        y0   = temp;

        temp = x1;
        x1   = y1;
        y1   = temp;

        steep = true;
    }

    // draw from right to left
    if (x0 > x1)
    {
        temp = x0;
        x0   = x1;
        x1   = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    int32_t dx      = x1 - x0;
    int32_t dy      = y1 - y0;
    int32_t px      = 0.f;
    int32_t py      = 0.f;
    int32_t de      = abs(dy) << 1;
    int32_t error   = 0;
    int32_t width   = (int32_t)framebuffer->width;
    int32_t height  = (int32_t)framebuffer->height;

    for (int32_t x = x0, y = y0; x <= x1; x++)
    {
        px = x;
        py = y;

        if (steep)
        {
            px = y;
            py = x;
        }

        if (px >= 0.f && px < width && py >= 0.f && py < height)
        {
            framebuffer_set(framebuffer, (uint32_t)px, (uint32_t)py, color);
        }

        error += de;
        if (error >= dx)
        {
            y += (y1 > y0) ? 1 : -1;
            error -= (dx << 1);
        }
    }

}

void rasterizer_draw_triangle(vec4_t v0,
                              vec4_t v1,
                              vec4_t v2,
                              framebuffer_t* framebuffer,
                              depthbuffer_t* depthbuffer)
{
    // workaround until clipping is implemented
    if (v0.z <= 0.f || v0.z > 1.f ||
        v1.z <= 0.f || v1.z > 1.f ||
        v2.z <= 0.f || v2.z > 1.f)
    {
        // return;
    }

    int32_t x0 = (int32_t)v0.x;
    int32_t x1 = (int32_t)v1.x;
    int32_t x2 = (int32_t)v2.x;

    int32_t y0 = (int32_t)v0.y;
    int32_t y1 = (int32_t)v1.y;
    int32_t y2 = (int32_t)v2.y;

    // find min/max within buffer boundaries
    int32_t minx = (int32_t)fmax(fmin(fmin(x0, x1), x2), 0.f);
    int32_t miny = (int32_t)fmax(fmin(fmin(y0, y1), y2), 0.f);
    int32_t maxx = (int32_t)fmin(fmax(fmax(x0, x1), x2), framebuffer->width - 1);
    int32_t maxy = (int32_t)fmin(fmax(fmax(y0, y1), y2), framebuffer->height - 1);

    // area of parallelogram
    float area = (float)edge_check(x0, y0, x1, y1, x2, y2);

    if (area == 0.f)
    {
        return;
    }


    float inv_area  = 1.f / area;

    for (int32_t y = miny; y <= maxy; y++)
    {
        for (int32_t x = minx; x <= maxx; x++)
        {
            float w0 = (float)edge_check(x1, y1, x2, y2, x, y);
            float w1 = (float)edge_check(x2, y2, x0, y0, x, y);
            float w2 = (float)edge_check(x0, y0, x1, y1, x, y);

            if (w0 < 0.f || w1 < 0.f || w2 < 0.f)
            {
                continue;
            }

            // normalized barycentric coordinates
            w0 *= inv_area;
            w1 *= inv_area;
            w2 *= inv_area;

            // perspective correct interpolation of z
            float depth = w0 * v0.z + w1 * v1.z + w2 * v2.z;

            if (depth_test && depth < depthbuffer_get(depthbuffer, (uint32_t)x, (uint32_t)y))
            {
                continue;
            }

            uint32_t color = fragment_shader(w0, w1, w2);

            depthbuffer_set(depthbuffer, (uint32_t)x, (uint32_t)y, depth);
            framebuffer_set(framebuffer, (uint32_t)x, (uint32_t)y, color);
        }
    }
}