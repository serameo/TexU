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
#define IDC_PANEL2          107
#define IDC_PANEL3          108
#define IDC_PANEL4          109
#define IDC_GROUP1          110
#define IDC_GROUP2          111
/*command id*/
#define ID_QUIT             100
#define ID_TOGGLE_PANEL     101
#define ID_TOGGLE_GROUP     102

/*class*/
#define MyWndClass          "MyWndClass"


texu_longptr
MyWndProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    texu_status rc = TEXU_OK;
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_wnd* child;
    texu_wnd* panel;
    texu_wnd* group;
    texu_wnd* status;
    static texu_i32 toggle = 0;
    static texu_i32 toggle_group = 0;
    switch (msg)
    {
        /*
        case TEXU_WM_KEYDOWN:
        {
            rc = TexuDefWndProc(wnd, msg, param1, param2);
            if (0 == rc)
            {
                if ('0' == param1)
                {
                    group = TexuGetWindowItem(wnd, IDC_GROUP1);
                    TexuShowWindow(group, toggle_group);
                    toggle_group = !toggle_group;
                }
                if ('1' == param1)
                {
                    group = TexuGetWindowItem(wnd, IDC_GROUP1);
                    panel = TexuGetWindowItem(group, IDC_PANEL1);
                    TexuShowWindow(panel, toggle);
                    toggle = !toggle;
                }
            }
        }
        */
        case TEXU_WM_COMMAND:
        {
            if (ID_QUIT == param1)
            {
                TexuExit();
            }
            else if (ID_TOGGLE_GROUP == param1)
            {
                group = TexuGetWindowItem(wnd, IDC_GROUP1);
                TexuShowWindow(group, toggle_group);
                toggle_group = !toggle_group;
            }
            else if (ID_TOGGLE_PANEL == param1)
            {
                group = TexuGetWindowItem(wnd, IDC_GROUP1);
                panel = TexuGetWindowItem(group, IDC_PANEL1);
                TexuShowWindow(panel, toggle);
                toggle = !toggle;
            }
            
            return 0;
        }
        case TEXU_WM_CREATE:
        {
            group = TexuCreateWindow(
                        "GROUP 1",
                        TEXU_PANEL_CLASS,
                        TEXU_PNS_TITLE|TEXU_PNS_BORDER|TEXU_PNS_STATUS, /* style*/
                        0,              /* exstyle*/
                        0,              /* y */
                        0,              /* x */
                        15,             /* height */
                        100,            /* width */
                        wnd,            /* parent */
                        IDC_GROUP1,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(group, TEXU_CIO_COLOR_CYAN_WHITE, TEXU_CIO_COLOR_CYAN_WHITE);
            TexuSendMessage(group, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_BLUE_CYAN, TEXU_CIO_COLOR_BLUE_CYAN);
            TexuSendMessage(group, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_CENTER, 0);
            
            status = (texu_wnd*)TexuSendMessage(group, TEXU_PNM_GETSTATUSBAR, 0, 0);
            TexuSetWindowText(status, "Press F5 to Quit/Press 0 to toggle on/off");
            TexuSendMessage(status, TEXU_SBM_SETCOLOR, 0, TEXU_CIO_COLOR_YELLOW_MAGENTA);
            TexuSendMessage(status, TEXU_SBM_SETBGCOLOR, 0, TEXU_CIO_COLOR_MAGENTA_YELLOW);
#if 0
            child = TexuCreateWindow(
                        "Press F5 to Quit",
                        TEXU_LABEL_CLASS,
                        TEXU_WS_CENTER, /* style*/
                        0,              /* exstyle*/
                        10,             /* y: last line of the panel */
                        1,              /* x */
                        1,              /* height */
                        98,             /* width */
                        group,          /* parent */
                        IDC_LABEL2,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
#endif
            panel = TexuCreateWindow(
                        "Panel 1",
                        TEXU_PANEL_CLASS,
                        TEXU_PNS_TITLE|TEXU_PNS_BORDER|TEXU_PNS_STATUS, /* style*/
                        0,              /* exstyle*/
                        1,              /* y */
                        1,              /* x */
                        9,             /* height */
                        50,             /* width */
                        group,          /* parent */
                        IDC_PANEL1,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(panel, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
            TexuSendMessage(panel, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_WHITE_GREEN, TEXU_CIO_COLOR_WHITE_GREEN);
            TexuSendMessage(panel, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_CENTER, 0);
            
            status = (texu_wnd*)TexuSendMessage(panel, TEXU_PNM_GETSTATUSBAR, 0, 0);
            TexuSetWindowText(status, "Press 1 to to toggle Panel 1 on/off");
            TexuSendMessage(status, TEXU_SBM_SETCOLOR, 0, TEXU_CIO_COLOR_BLUE_YELLOW);
            TexuSendMessage(status, TEXU_SBM_SETBGCOLOR, 0, TEXU_CIO_COLOR_YELLOW_BLUE);

#if 0
            child = TexuCreateWindow(
                        "Press 1 to to toggle Panel 1 on/off",
                        TEXU_LABEL_CLASS,
                        TEXU_WS_CENTER, /* style*/
                        0,              /* exstyle*/
                        7,              /* y: last line of the panel */
                        1,              /* x */
                        1,              /* height */
                        48,             /* width */
                        panel,          /* parent */
                        IDC_LABEL1,     /* id */
                        0               /* user data */
                        );
#endif
            group = TexuCreateWindow(
                        "GROUP 2",
                        TEXU_PANEL_CLASS,
                        TEXU_PNS_TITLE|TEXU_PNS_BORDER, /* style*/
                        0,              /* exstyle*/
                        15,             /* y */
                        0,              /* x */
                        10,             /* height */
                        100,            /* width */
                        wnd,            /* parent */
                        IDC_GROUP2,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(group, TEXU_CIO_COLOR_CYAN_WHITE, TEXU_CIO_COLOR_CYAN_WHITE);
            TexuSendMessage(group, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_BLUE_CYAN, TEXU_CIO_COLOR_BLUE_CYAN);
            TexuSendMessage(group, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_CENTER, 0);

            TexuAddHotKey(wnd, '0', ID_TOGGLE_GROUP, 0);
            TexuAddHotKey(wnd, '1', ID_TOGGLE_PANEL, 0);
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

