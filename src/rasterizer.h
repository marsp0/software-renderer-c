#pragma once

#include "math.h"
#include "mesh.h"
#include "framebuffer.h"
#include "depthbuffer.h"

void rasterizer_set_fragment_shader(uint32_t (*shader)(float, float, float));
void rasterizer_draw_line(vec4_t p0,
                          vec4_t p1,
                          uint32_t color,
                          framebuffer_t* framebuffer);

void rasterizer_draw_triangle(vec4_t v0,
                              vec4_t v1,
                              vec4_t v2,
                              framebuffer_t* framebuffer,
                              depthbuffer_t* depthbuffer);