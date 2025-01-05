#include "texture.h"

#include <stdlib.h>
#include <stdio.h>

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

/********************/
/* static functions */
/********************/

static vec4_t sample(texture_t* texture, uint32_t x, uint32_t y)
{
    float w             = (float)texture->width;
    float h             = (float)texture->height;
    x                   = u_min(w, u_max(0, x));
    y                   = u_min(h, u_max(0, y));
    uint32_t stride     = texture->stride;
    unsigned char* data = texture->data;

    uint32_t index      = (x + (uint32_t)w * y) * stride;
    float d             = 1.f / 255.f;
    float b             = (float)(data[index + 2] & 0xFF);
    float g             = (float)(data[index + 1] & 0xFF);
    float r             = (float)(data[index + 0] & 0xFF);

    return vec4_new(b * d, g * d, r * d);
}

/********************/
/* public functions */
/********************/

texture_t* texture_new(uint32_t width, uint32_t height, uint32_t stride)
{
	texture_t* texture = malloc(sizeof(texture_t));
	texture->width     = width;
	texture->height    = height;
	texture->stride    = stride;
	texture->data      = malloc(width * height * stride);
	return texture;
}

vec4_t texture_sample(texture_t* texture, float u, float v)
{
    // this function converts rgba from image to bgra

    float w                 = (float)texture->width;
    float h                 = (float)texture->height;
    vec4_t result           = vec4_new(1.f, 0.f, 1.f);
    texture_filter_e filter = get_texture_filter();

    if (filter == POINT_SAMPLE)
    {
        uint32_t x = (uint32_t)f_floor(u * w);
        uint32_t y = (uint32_t)f_floor(v * h);
        result = sample(texture, x, y);
    }
    else if (filter == BILINEAR_SAMPLE)
    {
        float x = u * w;
        float y = v * h;
        float x1 = f_floor(u * w);
        float x2 = x1 + 1.f;
        float y1 = f_floor(v * h);
        float y2 = y1 + 1.f;

        vec4_t f_x1y1 = sample(texture, (uint32_t)x1, (uint32_t)y1);
        vec4_t f_x1y2 = sample(texture, (uint32_t)x1, (uint32_t)y2);
        vec4_t f_x2y1 = sample(texture, (uint32_t)x2, (uint32_t)y1);
        vec4_t f_x2y2 = sample(texture, (uint32_t)x2, (uint32_t)y2);

        vec4_t f_xy1_1  = vec4_scale(f_x1y1, (x2 - x) / (x2 - x1));
        vec4_t f_xy1_2  = vec4_scale(f_x2y1, (x - x1) / (x2 - x1));
        vec4_t f_xy1    = vec4_add(f_xy1_1, f_xy1_2);

        vec4_t f_xy2_1  = vec4_scale(f_x1y2, (x2 - x) / (x2 - x1));
        vec4_t f_xy2_2  = vec4_scale(f_x2y2, (x - x1) / (x2 - x1));
        vec4_t f_xy2    = vec4_add(f_xy2_1, f_xy2_2);

        vec4_t f_xy_1   = vec4_scale(f_xy1, (y2 - y) / (y2 - y1) );
        vec4_t f_xy_2   = vec4_scale(f_xy2, (y - y1) / (y2 - y1) );

        result = vec4_add(f_xy_1, f_xy_2);
    }

    return result;
}

void texture_free(texture_t* texture)
{
	free(texture->data);
	free(texture);
}