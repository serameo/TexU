/*
#
# File Name: texufrm.c
# Author: Seree Rakwong
# Date: 24-NOV-2022
#
# History
# Date              Author          Description
# ---------------------------------------------
# 24-NOV-2022       MEO             Create a frame window to handle TEXU_WM_CHAR
#
*/

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define TEXU_THREAD_SAFE    1

#include "texulib.h"
#include "texutils.h"
#include "texui.h"
#include "texumenu.h"
#include "texust.h"
#include "cjson.h"

#ifdef __cplusplus
extern "C"
{
#endif
    texu_longptr
    TexuDefWndProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2);

    /*
    #-------------------------------------------------------------------------------
    # TexU frame window procedure
    #
             1         2         3         4         5         6         7         8
    12345678901234567890123456789012345678901234567890123456789012345678901234567890
    */

    texu_i32 _TexuFrameWndProc_OnChar(texu_wnd *, texu_i32 ch, texu_i32 alt);

    

    texu_i32
    _TexuFrameWndProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
    {
        texu_i32 rc = 0;
        texu_wnd *activewnd = texu_wnd_get_activechild(wnd);
        texu_wnd *nextwnd = 0;
        texu_wnd *parent = texu_wnd_get_parent(wnd);
        texu_wnd *desktop = (wnd ? texu_env_get_desktop(wnd->env) : 0);
        texu_i32 y = 0;
        texu_i32 x = 0;
        texu_i32 width = 0;
        /*texu_cio *cio = 0;*/
        texu_wnd_keycmd *keycmd = 0;
        texu_menu *menu = 0;

        keycmd = texu_wnd_find_keycmd(wnd, ch, alt);
        if (keycmd && parent == desktop)
        {
            /* if there are any hotkey registered */
            return texu_wnd_send_msg(wnd, TEXU_WM_COMMAND, (texu_lparam)keycmd->cmd, alt);
        }
        if (parent == desktop)
        {
            if (ch != -1 && (alt & TEXU_KEYPRESSED_ALT))
            {
                menu = wnd->menu;
                if (menu)
                {
                    rc = texu_wnd_send_msg(wnd, TEXU_WM_ENTERMENU, ch, alt);
                    return 0;
                }
            }
            if (activewnd && activewnd != wnd)
            {
                if (ch == TEXU_KEY_NEXTWND)
                {
                    nextwnd = texu_wnd_get_next_activechild(wnd, activewnd);
                }
                else if (ch == TEXU_KEY_PREVWND)
                {
                    nextwnd = texu_wnd_get_prev_activechild(wnd, activewnd);
                }

                /* kill and set the new active window */
                if (nextwnd)
                {
                    rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_lparam)nextwnd, 0);
                    if (rc != TEXU_OK)
                    {
                        return -1;
                    }
                    rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)activewnd, 0);

                    /* the new active window */
                    wnd->activechild = nextwnd;
                    return 1;
                }
                else
                {
#if 1
                    /* rounded robin active child */
                    if ((ch == TEXU_KEY_NEXTWND) || (ch == TEXU_KEY_PREVWND))
                    {
                        texu_wnd *lastwnd = texu_wnd_get_last_activechild(wnd);
                        texu_wnd *firstwnd = texu_wnd_get_first_activechild(wnd);
                        
                        if ((activewnd == lastwnd) && (ch == TEXU_KEY_NEXTWND))
                        {
                            /*move the next active window to the first active child*/
                            nextwnd = firstwnd;
                        }
                        else if ((activewnd == firstwnd) && (ch == TEXU_KEY_PREVWND))
                        {
                            /*move the next active window to the last active child*/
                            nextwnd = lastwnd;
                        }
                        rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_lparam)nextwnd, 0);
                        if (rc != TEXU_OK)
                        {
                            return -1;
                        }
                        rc = texu_wnd_send_msg((nextwnd ? nextwnd : wnd), TEXU_WM_SETFOCUS, (texu_lparam)activewnd, 0);

                        /* the new active window */
                        wnd->activechild = nextwnd;
                        return 1;
                    }
#endif
                    return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_lparam)ch, alt);
                }
            } /* child window is active */
            else
            {
                /* the current frame window is active */
                if (activewnd)
                {
                    activewnd = texu_wnd_get_activechild(activewnd);
                    return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_lparam)ch, alt);
                }
            }
        }
        else
        {
            if (activewnd)
            {
                return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_lparam)ch, alt);
            }
        }
        return 0;
    }

    texu_longptr
    TexuFrameWndProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
    {
        switch (msg)
        {
            case TEXU_WM_CHAR:
                return _TexuFrameWndProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
        }
        return TexuDefWndProc(wnd, msg, param1, param2);
    }

    /*
    #
    #
    */

#ifdef __cplusplus
}
#endif
