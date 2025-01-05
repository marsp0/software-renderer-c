#pragma once

#include <stdint.h>

#include "math.h"
#include "settings.h"

typedef struct
{
	uint32_t width;
	uint32_t height;
	uint32_t stride;
	unsigned char* data; /* R G B (A) */
} texture_t;

texture_t*  texture_new(uint32_t width, uint32_t height, uint32_t stride);
vec4_t      texture_sample(texture_t* texture, float u, float v, texture_filter_e method);
void        texture_free(texture_t* texture);