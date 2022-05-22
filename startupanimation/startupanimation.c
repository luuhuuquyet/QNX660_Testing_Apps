#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main ( void )
{
    int i = 1;
    char cmd[255];
    system ( "display_image -zorder=1 -noinit=1 -display=1 -size=1200x1920 -file=/nfs/images/Splash_0.png &" );
    for ( i = 1; i <= 8; i++ ) {
      sprintf ( cmd, "display_image -zorder=1 -noinit=1 -display=1 -pos=0,0 -size=1200x450 -file=/nfs/images/Splash_%d.png &", i );
      system ( cmd );
      usleep ( 70000 );
    }

    system ( "slay -f display_image" );
    system ( "display_image -zorder=1 -noinit=1 -display=1 -size=990x990 -file=/nfs/images/Splash_9.png &" );

    return 0;
}
