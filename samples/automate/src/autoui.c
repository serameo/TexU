/*
 *       Program  : AUTOUI.C
 *       System   : Linux
 *       Date     : 14-Nov-2022
 *       By       : Seree Rakwong
 *       Purpose  : Automate test dumb terminal.
 *       Reference: https://github.com/serameo/TexU
 *                  https://www.tcl-lang.org/
 *       Edit history:
 *
 *       Date      By             Reason
 *       ========  ====           ================================================
 *
 */



#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <sys/resource.h>
#include <signal.h>

/*
#define USE_TCL_AUTOMATION      1
*/

#include "texu.h"           /*simple implement Text UI*/
#include "autoui.h"         /*defines, macros, prototypes*/


int main(int argc, char* argv[])
{
    texu_status rc = TEXU_OK;
    texu_wnd *wnd = 0;
    
    /*TexuStartup(25, 80, argv[0]);*/
    TexuStartup(25, 80, "autoui");
   
    /*register class*/
    rc = TexuRegisterClass(
            LOGINWND_CLASS,
            Login_WndProc);


    /*create the new main window*/
    wnd = TexuCreateWindow(
            "Auto UI App",
            LOGINWND_CLASS,
            0, /* style*/
            0, /* exstyle*/
            0, /* y */
            0, /* x */
            TexuGetMaxY(),
            TexuGetMaxX(),
            0, /* parent */
            1, /* id */
            0  /* user data */
    );

    TexuShowWindow(wnd, TEXU_WS_SHOW);
    TexuInvalidateWindow(wnd);

    TexuRun();
    TexuShutdown();

    return 0;
}

