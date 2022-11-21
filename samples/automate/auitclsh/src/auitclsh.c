
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl.h>
#include <texu.h>

#include "auitclsh.h"

int main(int argc, char *argv[])
{   
    /*tcl*/
    Tcl_Main( argc, argv, auitclsh_init );
    return 0;
}

