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
#define IDC_EDIT1           111
#define IDC_EDIT2           112
#define IDC_EDIT3           113
#define IDC_EDIT4           114
#define IDC_PANEL1          121
/*command id*/
#define ID_QUIT             100

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
    texu_i32 y = 2;

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
                        0,              /* y */
                        0,             /* x */
                        25,             /* height */
                        100,            /* width */
                        wnd,            /* parent */
                        IDC_PANEL1,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(panel, TEXU_CIO_COLOR_CYAN_WHITE, TEXU_CIO_COLOR_CYAN_WHITE);
            TexuSendMessage(panel, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_BLUE_CYAN, TEXU_CIO_COLOR_BLUE_CYAN);
            TexuSendMessage(panel, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_CENTER, 0);
            TexuSetWindowText(panel, "Sample Edit Controls");

            status = (texu_wnd*)TexuSendMessage(panel, TEXU_PNM_GETSTATUSBAR, 0, 0);
            TexuSetWindowText(status, "Press F5 to Quit/Press 0 to toggle on/off");
            TexuSendMessage(status, TEXU_SBM_SETCOLOR, 0, TEXU_CIO_COLOR_YELLOW_MAGENTA);
            TexuSendMessage(status, TEXU_SBM_SETBGCOLOR, 0, TEXU_CIO_COLOR_MAGENTA_YELLOW);

            child = TexuCreateWindow(
                        "Number:",
                        TEXU_LABEL_CLASS,
                        TEXU_WS_RIGHT,   /* style*/
                        0,              /* exstyle*/
                        y,              /* y: last line of the panel */
                        1,              /* x */
                        1,              /* height */
                        10,             /* width */
                        panel,          /* parent */
                        IDC_LABEL1,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_BLACK_WHITE, TEXU_CIO_COLOR_BLACK_WHITE);


            child = TexuCreateWindow(
                        "1234",
                        TEXU_EDIT_CLASS,
                        TEXU_WS_LEFT|TEXU_ES_NUMBER|TEXU_ES_AUTOHSCROLL,   /* style*/
                        0,              /* exstyle*/
                        y,              /* y: last line of the panel */
                        11,             /* x */
                        1,              /* height */
                        10,             /* width */
                        panel,          /* parent */
                        IDC_EDIT1,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

            /* next line*/
            ++y;
            child = TexuCreateWindow(
                        "Decimal:",
                        TEXU_LABEL_CLASS,
                        TEXU_WS_RIGHT,   /* style*/
                        0,              /* exstyle*/
                        y,              /* y: last line of the panel */
                        1,              /* x */
                        1,              /* height */
                        10,             /* width */
                        panel,          /* parent */
                        IDC_LABEL2,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_BLACK_WHITE, TEXU_CIO_COLOR_BLACK_WHITE);

            child = TexuCreateWindow(
                        "1234.567",
                        TEXU_EDIT_CLASS,
                        TEXU_WS_LEFT|TEXU_ES_DECIMAL|TEXU_ES_AUTOHSCROLL,   /* style*/
                        0,              /* exstyle*/
                        y,              /* y: last line of the panel */
                        11,              /* x */
                        1,              /* height */
                        10,             /* width */
                        panel,          /* parent */
                        IDC_EDIT2,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

            /* next line*/
            ++y;
            child = TexuCreateWindow(
                        "Text:",
                        TEXU_LABEL_CLASS,
                        TEXU_WS_RIGHT,   /* style*/
                        0,              /* exstyle*/
                        y,              /* y: last line of the panel */
                        1,              /* x */
                        1,              /* height */
                        10,             /* width */
                        panel,          /* parent */
                        IDC_LABEL3,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_BLACK_WHITE, TEXU_CIO_COLOR_BLACK_WHITE);

            child = TexuCreateWindow(
                        "Enter the text",
                        TEXU_EDIT_CLASS,
                        TEXU_WS_LEFT|TEXU_ES_AUTOHSCROLL,   /* style*/
                        0,              /* exstyle*/
                        y,              /* y: last line of the panel */
                        11,             /* x */
                        1,              /* height */
                        30,             /* width */
                        panel,          /* parent */
                        IDC_EDIT3,      /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

            TexuAddHotKey(wnd, TVK_F5, ID_QUIT, 0);
            return TEXU_OK;
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
