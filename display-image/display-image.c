#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <jpegint.h>
#include <time.h>
#include <screen/screen.h>
#include <png.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <dirent.h>
#include <img/img.h>


screen_context_t screen_ctx;
screen_window_t screen_win;
const char* img_path = "/nfs/QuyetLuu.jpg"; /* Relative path to image asset */
int viewport_size[2] = { 0, 0 };


static int decode_setup(uintptr_t data, img_t *img, unsigned flags)
{
    screen_window_t screen_win = (screen_window_t)data;
    screen_buffer_t screen_buf;
    int size[2];

    size[0] = img->w;
    size[1] = img->h;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, size);
    screen_create_window_buffers(screen_win, 1);

    screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_buf);
    screen_get_buffer_property_pv(screen_buf, SCREEN_PROPERTY_POINTER, (void **)&img->access.direct.data);
    screen_get_buffer_property_iv(screen_buf, SCREEN_PROPERTY_STRIDE, (int *)&img->access.direct.stride);

    img->flags |= IMG_DIRECT;
    return IMG_ERR_OK;
}

static void decode_abort(uintptr_t data, img_t *img)
{
    screen_window_t screen_win = (screen_window_t)data;
    screen_destroy_window_buffers(screen_win);
}

int load_image(screen_window_t screen_win, const char *path)
{
    img_decode_callouts_t callouts;
    img_lib_t ilib = NULL;
    img_t img;
    int rc;

    rc = img_lib_attach(&ilib);
    if (rc != IMG_ERR_OK) {
        printf("Failed to load lib \n");
        return -1;
    }

    memset(&img, 0, sizeof(img));
    img.flags |= IMG_FORMAT;
    img.format = IMG_FMT_PKLE_XRGB8888;

    memset(&callouts, 0, sizeof(callouts));
    callouts.setup_f = decode_setup;
    callouts.abort_f = decode_abort;
    callouts.data = (uintptr_t)screen_win;

    rc = img_load_file(ilib, path, &callouts, &img);
    img_lib_detach(ilib);

    return rc == IMG_ERR_OK ? 0 : -1;
}



int main(int argc, char **argv)
{
    const int usage = SCREEN_USAGE_WRITE;

    screen_buffer_t screen_buf = NULL;
    int rect[4] = { 0, 0, 0, 0 };

    /* Setup the window */
    screen_create_context(&screen_ctx, 0);
    screen_create_window(&screen_win, screen_ctx);
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage);

    load_image(screen_win, img_path);

    screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_buf);
    screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);
    viewport_size[0] = rect[2];
    viewport_size[1] = rect[3];
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SOURCE_SIZE , viewport_size);

    screen_post_window(screen_win, screen_buf, 1, rect, 0);


    while(1){

    }
}