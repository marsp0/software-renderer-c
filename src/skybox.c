#include "skybox.h"

#include <stdlib.h>
#include <stdio.h>

#include "parsers/png.h"
#include "constants.h"

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

/********************/
/* public functions */
/********************/

skybox_t* skybox_new()
{
    skybox_t* skybox = malloc(sizeof(skybox_t));

    skybox->vertices_size = 24;
    skybox->vertices = malloc(sizeof(vec4_t) * skybox->vertices_size);
    skybox->vertices[0] = vec4_new(-1.f,   1.f,   1.f);
    skybox->vertices[1] = vec4_new(-1.f,   1.f,   -1.f);
    skybox->vertices[2] = vec4_new(-1.f,   -1.f,   1.f);
    skybox->vertices[3] = vec4_new(-1.f,   -1.f,   -1.f);

    skybox->vertices[4] = vec4_new(1.f,   1.f,   1.f);
    skybox->vertices[5] = vec4_new(-1.f,   1.f,   1.f);
    skybox->vertices[6] = vec4_new(1.f,   -1.f,   1.f);
    skybox->vertices[7] = vec4_new(-1.f,   -1.f,   1.f);
    
    skybox->vertices[8] = vec4_new(1.f,   1.f,   -1.f);
    skybox->vertices[9] = vec4_new(1.f,   1.f,   1.f);
    skybox->vertices[10] = vec4_new(1.f,   -1.f,   -1.f);
    skybox->vertices[11] = vec4_new(1.f,   -1.f,   1.f);
    
    skybox->vertices[12] = vec4_new(-1.f,   1.f,   -1.f);
    skybox->vertices[13] = vec4_new(1.f,   1.f,   -1.f);
    skybox->vertices[14] = vec4_new(-1.f,   -1.f,   -1.f);
    skybox->vertices[15] = vec4_new(1.f,   -1.f,   -1.f);

    skybox->vertices[16] = vec4_new(1.f,   1.f,   1.f);
    skybox->vertices[17] = vec4_new(1.f,   1.f,   -1.f);
    skybox->vertices[18] = vec4_new(-1.f,   1.f,   1.f);
    skybox->vertices[19] = vec4_new(-1.f,   1.f,   -1.f);
    
    skybox->vertices[20] = vec4_new(-1.f,   -1.f,   1.f);
    skybox->vertices[21] = vec4_new(-1.f,   -1.f,   -1.f);
    skybox->vertices[22] = vec4_new(1.f,   -1.f,   1.f);
    skybox->vertices[23] = vec4_new(1.f,   -1.f,   -1.f);


    skybox->indices_size = 36;
    skybox->indices = malloc(sizeof(uint32_t) * skybox->indices_size);
    skybox->indices[0] = 0;
    skybox->indices[1] = 2;
    skybox->indices[2] = 1;
    skybox->indices[3] = 2;
    skybox->indices[4] = 3;
    skybox->indices[5] = 1;
    
    skybox->indices[6] = 4;
    skybox->indices[7] = 6;
    skybox->indices[8] = 5;
    skybox->indices[9] = 6;
    skybox->indices[10] = 7;
    skybox->indices[11] = 5;
    
    skybox->indices[12] = 8;
    skybox->indices[13] = 10;
    skybox->indices[14] = 9;
    skybox->indices[15] = 10;
    skybox->indices[16] = 11;
    skybox->indices[17] = 9;
    
    skybox->indices[18] = 12;
    skybox->indices[19] = 14;
    skybox->indices[20] = 13;
    skybox->indices[21] = 14;
    skybox->indices[22] = 15;
    skybox->indices[23] = 13;
    
    skybox->indices[24] = 16;
    skybox->indices[25] = 18;
    skybox->indices[26] = 17;
    skybox->indices[27] = 18;
    skybox->indices[28] = 19;
    skybox->indices[29] = 17;
    
    skybox->indices[30] = 20;
    skybox->indices[31] = 22;
    skybox->indices[32] = 21;
    skybox->indices[33] = 22;
    skybox->indices[34] = 23;
    skybox->indices[35] = 21;

    skybox->texcoords_size = 24;
    skybox->texcoords = malloc(sizeof(vec2_t) * skybox->texcoords_size);
    skybox->texcoords[0] = vec2_new(0.f, 0.f);
    skybox->texcoords[1] = vec2_new(1.f, 0.f);
    skybox->texcoords[2] = vec2_new(0.f, 1.f);
    skybox->texcoords[3] = vec2_new(1.f, 1.f);

    skybox->texcoords[4] = vec2_new(0.f, 0.f);
    skybox->texcoords[5] = vec2_new(1.f, 0.f);
    skybox->texcoords[6] = vec2_new(0.f, 1.f);
    skybox->texcoords[7] = vec2_new(1.f, 1.f);

    skybox->texcoords[8] = vec2_new(0.f, 0.f);
    skybox->texcoords[9] = vec2_new(1.f, 0.f);
    skybox->texcoords[10] = vec2_new(0.f, 1.f);
    skybox->texcoords[11] = vec2_new(1.f, 1.f);

    skybox->texcoords[12] = vec2_new(0.f, 0.f);
    skybox->texcoords[13] = vec2_new(1.f, 0.f);
    skybox->texcoords[14] = vec2_new(0.f, 1.f);
    skybox->texcoords[15] = vec2_new(1.f, 1.f);

    skybox->texcoords[16] = vec2_new(0.2, 0.2);
    skybox->texcoords[17] = vec2_new(1.f, 1.f);
    skybox->texcoords[18] = vec2_new(1.f, 1.f);
    skybox->texcoords[19] = vec2_new(1.f, 1.f);

    skybox->texcoords[20] = vec2_new(1.f, 1.f);
    skybox->texcoords[21] = vec2_new(1.f, 1.f);
    skybox->texcoords[22] = vec2_new(1.f, 1.f);
    skybox->texcoords[23] = vec2_new(1.f, 1.f);

    skybox->x_positive = parse_png_file(SKYBOX_X_POS_FILE);
    skybox->x_negative = parse_png_file(SKYBOX_X_NEG_FILE);
    skybox->y_positive = parse_png_file(SKYBOX_Y_POS_FILE);     // encoded with Huffman type 1 which we do not support
    skybox->y_negative = parse_png_file(SKYBOX_Y_NEG_FILE);
    skybox->z_positive = parse_png_file(SKYBOX_Z_POS_FILE);
    skybox->z_negative = parse_png_file(SKYBOX_Z_NEG_FILE);

    return skybox;
}

void skybox_free(skybox_t* skybox)
{
    free(skybox);
}
