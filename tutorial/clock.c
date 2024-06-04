#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texu.h"
#include "texukeys.h"

/*control id*/
#define IDC_LABEL1          101
#define IDC_LABEL2          102
#define IDC_LABEL3          103
#define IDC_LABEL4          104
#define IDC_KEYINFO         105
#define IDC_PANEL1          106
#define IDC_CLOCK1          107
/*command id*/
#define ID_QUIT             100
#define ID_TOGGLE           101

/*class*/
#define MyWndClass          "MyWndClass"


texu_longptr
MyWndProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    texu_status rc = TEXU_OK;
    texu_wnd* clock;
    texu_wnd* child;
    texu_wnd* panel;
    texu_wnd* status;

    switch (msg)
    {
        case TEXU_WM_COMMAND:
        {
            if (ID_QUIT == param1)
            {
                TexuExit();
            }
            return 0;
        }
        case TEXU_WM_CREATE:
        {
            panel = TexuCreateWindow(
                        "Hello World",
                        TEXU_PANEL_CLASS,
                        TEXU_PNS_TITLE|TEXU_PNS_BORDER|TEXU_PNS_STATUS, /* style*/
                        0,              /* exstyle*/
                        10,              /* y */
                        20,              /* x */
                        10,             /* height */
                        60,             /* width */
                        wnd,            /* parent */
                        IDC_PANEL1,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(panel, TEXU_CIO_COLOR_CYAN_WHITE, TEXU_CIO_COLOR_CYAN_WHITE);
            TexuSendMessage(panel, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_BLUE_CYAN, TEXU_CIO_COLOR_BLUE_CYAN);
            TexuSendMessage(panel, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_CENTER, 0);
            TexuSetWindowText(panel, "Hello World");

            status = (texu_wnd*)TexuSendMessage(panel, TEXU_PNM_GETSTATUSBAR, 0, 0);
            TexuSetWindowText(status, "Press F5 to Quit/Press 0 to toggle on/off");
            TexuSendMessage(status, TEXU_SBM_SETCOLOR, 0, TEXU_CIO_COLOR_YELLOW_MAGENTA);
            TexuSendMessage(status, TEXU_SBM_SETBGCOLOR, 0, TEXU_CIO_COLOR_MAGENTA_YELLOW);

            child = TexuCreateWindow(
                        "Time",
                        TEXU_CLOCKCTRL_CLASS,
                        TEXU_WS_LEFT,   /* style*/
                        0,              /* exstyle*/
                        1,              /* y: last line of the panel */
                        1,              /* x */
                        1,              /* height */
                        58,             /* width */
                        panel,          /* parent */
                        IDC_CLOCK1,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
#if 0
            child = TexuCreateWindow(
                        "Press F5 to Quit",
                        TEXU_LABEL_CLASS,
                        TEXU_WS_CENTER, /* style*/
                        0,              /* exstyle*/
                        9,              /* y: last line of the panel */
                        0,              /* x */
                        1,              /* height */
                        60,             /* width */
                        panel,            /* parent */
                        IDC_LABEL2,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
#endif
            TexuAddHotKey(wnd, TVK_F5, ID_QUIT, 0);
            return TEXU_OK;
        }
        case TEXU_WM_IDLE:
        {
            panel = TexuGetWindowItem(wnd, IDC_PANEL1);
            clock = TexuGetWindowItem(panel, IDC_CLOCK1);
            TexuSendMessage(clock, TEXU_CLKM_UPDATE, 0, 0);
            return 0;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

int main()
{
    texu_status rc = 0;
    texu_wnd *wnd = 0;

    rc = TexuStartup(25, 100); /*25 lines x 100 characters per line*/
    if (rc != TEXU_OK)
    {
        printf("cannot initialized environment\n");
        return -1;
    }

    rc = TexuRegisterClass(
        MyWndClass,
        MyWndProc);

    wnd = TexuCreateWindow(
        "Test TexU App",
        MyWndClass,
        TEXU_WS_CLIPWINDOW, /* style*/
        0, /* exstyle*/
        0, /* y */
        0, /* x */
        TexuGetMaxY(),
        TexuGetMaxX(),
        0, /* parent */
        1, /* id */
        0  /* user data */
    );

    TexuShowWindow(wnd, TEXU_SW_SHOW);
    TexuInvalidateWindow(wnd);

    TexuRun();

    TexuShutdown();
    return 0;
}
