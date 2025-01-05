#pragma once

#include <stdint.h>

#include "math.h"
#include "texture.h"

typedef struct
{
    vec4_t*     vertices;
    uint32_t    vertices_size;

    uint32_t*   indices;
    uint32_t    indices_size;

    vec2_t*     texcoords;
    uint32_t    texcoords_size;

    texture_t*  x_positive;
    texture_t*  x_negative;
    texture_t*  y_positive;
    texture_t*  y_negative;
    texture_t*  z_positive;
    texture_t*  z_negative;
} skybox_t;

skybox_t*   skybox_new();
void        skybox_free(skybox_t* skybox);
