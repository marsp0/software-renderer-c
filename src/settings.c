#include "settings.h"

#include <stdint.h>

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

static texture_filter_e texture_filter = BILINEAR_SAMPLE;

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

texture_filter_e get_texture_filter()
{
    return texture_filter;
}

void change_texture_filter()
{
    int32_t size = (int32_t)TEXTURE_FILTER_SIZE;
    int32_t filter = (int32_t)texture_filter;

    filter++;
    
    if (filter == size)
    {
        filter = (int32_t)POINT_SAMPLE;
    }
    
    texture_filter = (texture_filter_e)filter;
}
