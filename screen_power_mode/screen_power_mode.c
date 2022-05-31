
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <screen/screen.h>

void print_usage(char **argv){
    printf("%s [options]\n", argv);
    printf("   [options]: \n");
    printf("      -mode    :       on/off \n");
    printf("      -display :       display number \n");
        
    printf("Examples:\n");
    printf("    screen_power_mode -mode=off -display=1\n");
}

int main(int argc, char **argv)
{

    int i, rc, display=1, count=0, help = 0;
    int power_mode = SCREEN_POWER_MODE_OFF, power_mode_r = SCREEN_POWER_MODE_OFF;
    const char *tok;
    screen_context_t screen_ctx;
    if (argc < 2){
        print_usage(argv[0]);
        return 0;
    }
    for (i = 1; i < argc; i++) {
        if (!strncmp(argv[i], "-mode=", strlen("-mode="))) {
            if (!strcasecmp(argv[i]+strlen("-mode="), "off")) {
                power_mode =  SCREEN_POWER_MODE_OFF;
            } else if (!strcasecmp(argv[i]+strlen("-mode="), "on")) {
                power_mode =  SCREEN_POWER_MODE_ON;
            } else {
                fprintf(stderr, "invalid format: %s\n", argv[i] + strlen("-mode="));
                return -1;
            }
        }else if (strncmp(argv[i], "-display=", strlen("-display=")) == 0) {
            tok = argv[i] + strlen("-display=");
            display = atoi(tok);
        } else if (strncmp(argv[i], "-help", strlen("-help")) == 0){
            help = 1;
         } else  {
            fprintf(stderr, "invalid command line option: %s\n", argv[i]);
            return -1;
        }
    }
    if (help){

        print_usage(argv[0]);
        return 0;
    } 
    rc = screen_create_context(&screen_ctx, SCREEN_POWER_MANAGER_CONTEXT);
    if (rc) {
        perror("screen_create_context");
        goto fail;
    }

    rc = screen_get_context_property_iv(screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT, &count);
    if (rc) {
        perror("screen_get_context_property_iv");
        goto fail;
    }

    screen_display_t *screen_disps = calloc(count, sizeof(screen_display_t));
    if (screen_disps == NULL) {
        fprintf(stderr, "could not allocate memory for display list\n");
        goto fail;
    }


    rc = screen_get_context_property_pv(screen_ctx, SCREEN_PROPERTY_DISPLAYS, (void **)screen_disps);
    if (rc) {
        perror("screen_get_context_property_pv");
        goto fail;
    }

    for (i = 0; i < count; ++i) {
        int actual_id = 0;  // invalid
        (void)screen_get_display_property_iv(screen_disps[i],
                SCREEN_PROPERTY_ID, &actual_id);
        if (display == actual_id) {
            break;
        }
    }
    screen_display_t screen_disp = screen_disps[i];
    free(screen_disps);
    
    rc = screen_set_display_property_iv(screen_disp, SCREEN_PROPERTY_POWER_MODE, &power_mode);
    if (rc) {
        perror("screen_set_display_property_iv");
        goto fail;
    }

    rc = screen_get_display_property_iv(screen_disp, SCREEN_PROPERTY_POWER_MODE, &power_mode_r);
    if (rc) {
        perror("screen_get_display_property_iv");
        goto fail;
    }

    if (power_mode_r == power_mode) {
        fprintf(stderr, "Set power mode successfull\n");
    }else {
        fprintf(stderr, "Set power mode unsuccessfull\n");
    }

fail:
    screen_destroy_context(screen_ctx);
    return 0;
}
