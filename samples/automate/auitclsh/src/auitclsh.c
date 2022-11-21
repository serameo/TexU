
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

auitclsh_t g_auitclsh;

int main(int argc, char *argv[])
{
    g_auitclsh.msgkey = ftok("autoui", 65); /*the name is the same as TexU server*/
    g_auitclsh.msgid  = msgget(g_auitclsh.msgkey, 0666 | IPC_CREAT);
    
    /*tcl*/
    Tcl_Main( argc, argv, auitclsh_init );
    return 0;
}

