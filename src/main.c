#include <unistd.h>
#include <stdlib.h>

#include "renderer.h"
#include "rasterizer.h"
#include "time_utils.h"
#include "constants.h"

int32_t main()
{
    // initialize
    time_init();
    renderer_init();
    renderer_load(SCENE_FILE);

    // run
    renderer_run();

    // clean up
    renderer_free();

    return 0;
}
