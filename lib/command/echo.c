#include <type.h>
#include <const.h>
#include <sys.h>
#include <user.h>
#include <stdio.h>
#include <com.h>
#include <ipc.h>
#include <string.h>

int main( int argc , char *argv[] )
{
    if ( argc >= 2 && argv[1] != 0 )
    {
      printf( argv[1] );
    }
    return 0;
}
