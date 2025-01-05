#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

#include "parsers/glb.h"

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

scene_t* scene_new(const char* file_path)
{
    scene_t* scene = parse_scene(file_path);

    scene->skybox = skybox_new();

    return scene;
}

void scene_update(scene_t* scene, input_t input)
{
    camera_update(scene->camera, input);
}

void scene_free(scene_t* scene)
{
    mesh_free(scene->mesh);
    camera_free(scene->camera);
    skybox_free(scene->skybox);
    free(scene);
}