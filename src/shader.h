#pragma once

#include "camera.h"

#include "texture.h"

void        shader_set_uniforms(camera_t* cam,
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
                                vec4_t normal_vec2);

void        shader_set_uniforms_skybox(camera_t* cam,
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
                                       vec2_t tex_coord2);

vec4_t      shader_vertex_skybox(vec4_t v);
vec4_t      shader_vertex(vec4_t v);
uint32_t    shader_fragment(float w, float w0, float w1, float w2);
uint32_t    shader_fragment_skybox(float w, float w0, float w1, float w2);
