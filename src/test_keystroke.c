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
/*command id*/
#define ID_QUIT             100
#define ID_TOGGLE           101

/*class*/
#define MyWndClass          "MyWndClass"

void
MyWndProc_OnPaint(texu_wnd* wnd, texu_cio* cio, texu_rect* rect)
{
}

texu_longptr
MyWndProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    static texu_i32 counter = 0;
    texu_cio* cio = texu_wnd_get_cio(wnd);
    texu_status rc = TEXU_OK;
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_wnd* child;
    texu_char keyname[TEXU_MAX_WNDTEXT + 1];
    texu_wnd* panel;
    static texu_i32  toggle = 0;
    texu_i32 repaint = 0;

    switch (msg)
    {
        case TEXU_WM_KEYDOWN:
        {
            rc = TexuDefWndProc(wnd, msg, param1, param2);
            if (0 == rc)
            {
                /*param1 = key*/
                /*param2 = alt or ctrl flags*/
                panel = TexuGetWindowItem(wnd, IDC_PANEL1);
                /*toggle display panel*/
                repaint = 0;
                
                switch (param1)
                {
                    case TVK_F4:
                    {
                        texu_wnd_visible(panel, toggle);
                        toggle = !toggle;
                        break;
                    }
                    case TVK_LEFT:
                    {
                        texu_rect rect;
                        texu_wnd_get_rect(panel, &rect);
                        --rect.x;
                        texu_wnd_send_msg(panel, TEXU_WM_MOVE, (texu_lparam)&rect, TEXU_FALSE);
                        repaint++;
                        break;
                    }
                    case TVK_RIGHT:
                    {
                        texu_rect rect;
                        texu_wnd_get_rect(panel, &rect);
                        ++rect.x;
                        texu_wnd_send_msg(panel, TEXU_WM_MOVE, (texu_lparam)&rect, TEXU_FALSE);
                        repaint++;
                        break;
                    }
                    case TVK_UP:
                    {
                        texu_rect rect;
                        texu_wnd_get_rect(panel, &rect);
                        --rect.y;
                        texu_wnd_send_msg(panel, TEXU_WM_MOVE, (texu_lparam)&rect, TEXU_FALSE);
                        repaint++;
                        break;
                    }
                    case TVK_DOWN:
                    {
                        texu_rect rect;
                        texu_wnd_get_rect(panel, &rect);
                        ++rect.y;
                        texu_wnd_send_msg(panel, TEXU_WM_MOVE, (texu_lparam)&rect, TEXU_FALSE);
                        repaint++;
                        break;
                    }
                }
#if 1
                texu_cio_get_keyname(cio, param1, keyname, TEXU_MAX_WNDTEXT);
                sprintf(text, "key code: [%d], key name: [%s]", param1, keyname);
                child = TexuGetWindowItem(panel, IDC_KEYINFO);
                TexuSetWindowText(child, text);
#endif
                if (repaint)
                {
                    /*TexuInvalidateWindow(wnd);*/
                }
            }
            break;
        }
        case TEXU_WM_COMMAND:
        {
            if (ID_QUIT == param1)
            {
                TexuExit();
            }
            if (ID_TOGGLE == param1)
            {
                panel = TexuGetWindowItem(wnd, IDC_PANEL1);
                texu_wnd_visible(panel, toggle);
                toggle = !toggle;
            }
            return 0;
        }
/*        case TEXU_WM_PAINT:
        {
            if (!texu_wnd_can_paint(wnd))
            {
                return 0;
            }
            MyWndProc_OnPaint(wnd, (texu_cio*)param1, (texu_rect*)param2);
            return 0;
        }*/
        case TEXU_WM_CREATE:
        {
            panel = TexuCreateWindow(
                        "Frame 1",
                        TEXU_PANEL_CLASS,
                        TEXU_PNS_TITLE, /* style*/
                        0,              /* exstyle*/
                        20,              /* y */
                        80,              /* x */
                        10,             /* height */
                        60,             /* width */
                        wnd,            /* parent */
                        IDC_PANEL1,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(panel, TEXU_CIO_COLOR_CYAN_WHITE, TEXU_CIO_COLOR_CYAN_WHITE);
            TexuSendMessage(panel, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_BLUE_CYAN, TEXU_CIO_COLOR_BLUE_CYAN);
            TexuSendMessage(panel, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_LEFT, 0);
            TexuSetWindowText(panel, "This is a panel");
#if 1
            child = TexuCreateWindow(
                        "Press F4 to toggle on/off panel",
                        TEXU_LABEL_CLASS,
                        TEXU_WS_CENTER, /* style*/
                        0,              /* exstyle*/
                        9,              /* y */
                        0,              /* x */
                        1,              /* height */
                        40,             /* width */
                        panel,            /* parent */
                        IDC_LABEL1,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_GREEN, TEXU_CIO_COLOR_WHITE_GREEN);
            child = TexuCreateWindow(
                        "Press F3 to Quit",
                        TEXU_LABEL_CLASS,
                        TEXU_WS_CENTER, /* style*/
                        0,              /* exstyle*/
                        1,             /* y */
                        0,             /* x */
                        1,              /* height */
                        32,             /* width */
                        panel,            /* parent */
                        IDC_LABEL2,     /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
#endif

#if 1
            child = TexuCreateWindow(
                        "Press any key to find the key code",
                        TEXU_LABEL_CLASS,
                        TEXU_WS_CENTER, /* style*/
                        0,              /* exstyle*/
                        2,              /* y */
                        0,              /* x */
                        1,              /* height */
                        64,             /* width */
                        panel,            /* parent */
                        IDC_KEYINFO,    /* id */
                        0               /* user data */
                        );
            TexuSetColor(child, TEXU_CIO_COLOR_GREEN_YELLOW, TEXU_CIO_COLOR_YELLOW_GREEN);
#endif
            TexuAddHotKey(wnd, TVK_F3, ID_QUIT, 0);
/*            TexuAddHotKey(wnd, TVK_F4, ID_TOGGLE, 0);*/
            return TEXU_OK;
        }
        case TEXU_WM_IDLE:
        {
#if 0
            counter++;
            sprintf(text, "counter: %d", counter);
            texu_cio_draw_text(cio, 
                0,  /*y*/ 
                0,  /*x*/
                text, 
                TEXU_CIO_COLOR_WHITE_BLUE, /*white on blue*/
                0, /*bgcolor: Not used for vms*/ 
                0, /*clsname: Not used for vms*/ 
                0  /*id:      Not used for vms*/
                );
#endif
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
