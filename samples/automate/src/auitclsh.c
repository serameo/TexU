
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

#include "tcl.h"
#include "texu.h"
#include "auitclsh.h"
/*global variables*/
auitclsh_t g_auitclsh;

int main(int argc, char *argv[])
{
    texu_status rc  = TEXU_OK;
    texu_char   *filename = "./auitclsh.cnf";
    FILE        *fp     = fopen(filename, "r");
    texu_char   *key    = 0;
    texu_char   *val    = 0;
    texu_i64    i64     = 0;
    texu_f64    f64     = 0.0;
    texu_char   pathname[256] = "autoui";
    texu_xcnf   *xcnf   = 0;

    if (fp)
    {
        xcnf = texu_xcnf_new();
        rc = texu_xcnf_load(xcnf, fp);
        g_auitclsh.xcnf = xcnf;
        
        key = "AUITCLSH.TEXU.SERVER";
        val = texu_xcnf_get_string(xcnf, key, pathname);
        strcpy(pathname, val);
        fclose(fp);
    }
    else
    {
        g_auitclsh.xcnf = 0;
    }
    
    g_auitclsh.msgkey = ftok(pathname, 65); /*the name is the same as TexU server*/
    if (g_auitclsh.msgkey < 0)
    {
        printf("cannot connect %s\n", argv[1]);
        return -2;
    }
    g_auitclsh.msgid  = msgget(g_auitclsh.msgkey, 0666 | IPC_CREAT);

    /*tcl*/
    Tcl_Main( argc, argv, auitclsh_init );
    return 0;
}

