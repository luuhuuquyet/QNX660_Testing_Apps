/*====================================================================================
* Title:  Sample application which setting color to screen  
* Author: Quyet Luu
* Date:   May 24, 2022
*=====================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <screen/screen.h>

struct color_code {
    char *name;
    unsigned int value;
};

struct color_code list[] = {
    { .name = "white", .value = 0xffffffff},
    { .name = "black", .value = 0xff000000},
    { .name = "red",   .value = 0xffff0000},
    { .name = "green", .value = 0xff008000},
    { .name = "blue",  .value = 0xff0000ff},
    { .name = "yellow",.value = 0xffffff00},
    { .name = "silver",.value = 0xffc0c0c0},
    { .name = "gray",  .value = 0xff808080},
    { .name = "maroon",.value = 0xff800000},
    { .name = "purple",.value = 0xff800080},
    { .name = "fuchsia",.value = 0xffff00ff},
    { .name = "lime",.value = 0xff00ff00},
    { .name = "olive",.value = 0xff808000},
    { .name = "navy",.value = 0xff000080},
    { .name = "teal",.value = 0xff008080},
    { .name = "aqua",.value = 0xff00FFFF},
};

unsigned int check_color(char *color){
    int i = 0;
    int size = sizeof(list) / sizeof(list[0]);
    for ( i = 0; i < size; i++){
        if (!strncmp((char *)color, (const char *)list[i].name, strlen(color))){
            return list[i].value;
        }
    }
    return 0;
}

void print_usage(){
    printf("set_color_screen [option] \n");
    printf("[option]: \n");
    printf("    help \n");
    printf("    auto \n");
    printf("    white \n");
    printf("    black \n");
    printf("    red \n");
    printf("    green \n");
    printf("    blue \n");
    printf("    yellow \n");
    printf("    silver \n");
    printf("    gray \n");
    printf("    maroon \n");
    printf("    purple \n");
    printf("    fuchsia \n");
    printf("    lime \n");
    printf("    olive \n");
    printf("    navy \n");
    printf("    teal \n");
    printf("    aqua \n");
}



int main(int argc, char **argv){
    int pos[2], size[2];
    char color_name[10]="";
    color_name[0] = NULL;
    int set_auto = 0;
    int help = 0;
    char *tmp1;
    int vis = 0;
    int zorder = 0;
    int count = 0;
    int dims[2] = { 0, 0 };
    int rect[4] = { 0, 0, 1, 1 };
    screen_window_t screen_win = NULL;
    screen_buffer_t screen_buf;
    screen_context_t screen_ctx;
    int set_color = 0xffffffff; // default is White


    while(--argc > 0) {
        tmp1 = argv[1];
        ++argv;
        if ( !strncmp((const char *)tmp1, "auto", 4)){
            set_auto = 1;
        } else if (!strncmp((const char *)tmp1, "help", 4)){
            help = 1;
        } else {
            int i = 0;
            while (*(tmp1+i) != NULL){
                color_name[i] = *(tmp1+i);
                i++;
            }
            color_name[i]='\0';
        }   
    }
    if (help){
        print_usage();
        return 0;
    }
    if (!set_auto  && color_name[0] != NULL){
        set_color = check_color((char *)color_name);
        if (!set_color){
            printf("Invalide color! \n");
            print_usage();
            return -1;
        }
    }

    screen_create_context(&screen_ctx, 0);
    screen_get_context_property_iv(screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT, &count);
    screen_display_t *screen_disps = calloc(count, sizeof(screen_display_t));
    screen_get_context_property_pv(screen_ctx, SCREEN_PROPERTY_DISPLAYS, (void **)screen_disps);
    screen_display_t screen_disp = screen_disps[0];
    free(screen_disps);
    screen_get_display_property_iv(screen_disp, SCREEN_PROPERTY_SIZE, dims);
    screen_create_window(&screen_win, screen_ctx);
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_COLOR, &set_color);
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SOURCE_SIZE, dims);
    screen_create_window_buffers(screen_win, 1);
    screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_buf);

    pos[0] = pos[1] = 0;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_POSITION, pos);

    size[0] = dims[0];
    size[1] = dims[1];
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SIZE, size);

    zorder = 1;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ZORDER, &zorder);
    vis = 1;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_VISIBLE, &vis);
    screen_post_window(screen_win, screen_buf, 1, rect, 0);

    if (set_auto){
        printf ("Running auto mode ....");
        int index = 0;
        int size = sizeof(list) / sizeof(list[0]);
        for(index = 0; index < size; index++){
            screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_COLOR, (int *)list[index].value);
            screen_flush_context(screen_ctx, SCREEN_WAIT_IDLE);
            sleep ( 2 );

        }        
    } else {
        while (1) {    
        }        
    }

    screen_destroy_context(screen_ctx);

    return EXIT_SUCCESS; 
}



