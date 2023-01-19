/*
#
# File Name: texu.h
# Author: Seree Rakwong
# Date: 06-MAY-2020
#
*/

#ifndef _TEXU_H_
#define _TEXU_H_

/*
# TexU version
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#define TEXU_MAJOR_VERSION             0x100
#define TEXU_MINOR_VERSION             0x0
#define TEXU_VERSION                   (TEXU_MAJOR_VERSION | TEXU_MINOR_VERSION)

/*
# TexU definitions and macros
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#include "texumacros.h"

/*
# TexU types and structures
#
*/
#include "texutypes.h"

/*
# TexU libraries
#
*/
#include "texulib.h"
#include "texui.h"
#include "texutils.h"
#include "texucio.h"
#include "texust.h"
#include "texumenu.h"
#include "texuconst.h"
#include "texumacros.h"
#include "texutypes.h"
#include "texuix.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/*
# TexU interface functions
#
*/
#ifdef USE_TCL_AUTOMATION
texu_status         TexuStartup(texu_i32 lines, texu_i32 cols, const char* pathname);
#else
#if (defined WIN32 && defined _WINDOWS)
texu_status
TexuStartup(HINSTANCE hinst, texu_i32 lines, texu_i32 cols);
#else
texu_status         TexuStartup(texu_i32 lines, texu_i32 cols);
#endif
#endif /*USE_TCL_AUTOMATION*/

texu_status         TexuShutdown();
texu_status         TexuRun();
#if (defined WIN32 && defined _WINDOWS)
texu_status         TexuCreateMainEnvWnd(DWORD dwExStyle, DWORD dwStyle,
                                     int x, int y, int cx, int cy,
                                     LPVOID lpData);

texu_status         TexuCreateChildEnvWnd(DWORD dwExStyle, DWORD dwStyle,
                                          int x, int y, int cx, int cy,
                                          HWND hWndParent,
                                          UINT nID,
                                          LPVOID lpData);
#endif

texu_env*           TexuGetEnv();
texu_i32            TexuGetMaxY();
texu_i32            TexuGetMaxX();

void                TexuExit();
void                TexuSetThemeColor(texu_i32 (*setcolor)(texu_i32));

texu_status         TexuRegisterClass(
                        texu_char*   clsname,
                        texu_wndproc wndproc
                    );

texu_wnd*           TexuCreateWindow(
                        const texu_char* text,
                        const texu_char* clsname,
                        texu_ui32  style,
                        texu_ui32  exstyle,
                        texu_i32   y,
                        texu_i32   x,
                        texu_i32   h,
                        texu_i32   w,
                        texu_wnd*  parent,
                        texu_i32   id,
                        void*      userdata
                    );
                    
texu_wnd*           TexuCreateWindow2(
                        const texu_char* text,
                        const texu_char* clsname,
                        texu_ui32  style,
                        texu_ui32  exstyle,
                        texu_i32   y,
                        texu_i32   x,
                        texu_i32   h,
                        texu_i32   w,
                        texu_wnd*  parent,
                        texu_i32   id,
                        void*      userdata,
                        texu_i32   (*on_validate)(texu_wnd*, texu_char*)
                    );
void                TexuDestroyWindow(
                        texu_wnd*  wnd
                    );
#if USE_TCL_AUTOMATION
texu_status         TexuCreateControls(texu_wnd *wnd, texu_wnd_template *templ, texu_i32 nitems, texu_char *templname);
texu_status         TexuCreateControls2(texu_wnd *wnd, texu_wnd_template2 *templ, texu_i32 nitems, texu_char *templname);
#else
texu_status         TexuCreateControls(texu_wnd* wnd, texu_wnd_template*, texu_i32);
texu_status         TexuCreateControls2(texu_wnd* wnd, texu_wnd_template2*, texu_i32);
#endif
texu_i64            TexuCloseWindow(texu_wnd *wnd);

#define TEXU_SW_HIDE            0
#define TEXU_SW_SHOW            1
void                TexuShowWindow(
                        texu_wnd*  wnd,
                        texu_i32   show
                    );
void                TexuInvalidateWindow(
                        texu_wnd*  wnd
                    );
texu_bool           TexuIsExecutableCommand(
                        texu_wnd*   wnd,
                        texu_ui32   id);
texu_wnd*
TexuMessageBox(
    texu_char* caption,
    texu_char* text,
    texu_wnd*  owner,
    texu_ui32  id,
    texu_ui32  buttons,
    void*      userdata
);

texu_i64
TexuSendMessage(
    texu_wnd*     wnd,
    texu_ui32     msg,
    texu_i64      param1,
    texu_i64      param2
);

texu_i64
TexuPostMessage(
    texu_wnd*     wnd,
    texu_ui32     msg,
    texu_i64      param1,
    texu_i64      param2
);

texu_wnd*
TexuGetWindowItem(
    texu_wnd*     wnd,
    texu_ui32     id
);

texu_wnd *
TexuGetParent(
    texu_wnd *wnd);
    
void
TexuSetWindowText(
    texu_wnd*         wnd,
    const texu_char*  text
);

texu_i32
TexuGetWindowText(
    texu_wnd*         wnd,
    texu_char*        text,
    texu_i32          len
);

void
TexuSetColor(
    texu_wnd*     wnd,
    texu_ui32      color,
    texu_ui32      discolor
);
void
TexuSetBgColor(
texu_wnd*     wnd,
texu_ui32      color,
texu_ui32      discolor
);

void
TexuAddHotKey(
    texu_wnd*   wnd,
    texu_i32    key,
    texu_ui32   cmd,
    texu_i32    alt
);

void            TexuEnableWindow(texu_wnd* wnd, texu_bool enable);
texu_i64        TexuSetFocus(texu_wnd *wnd, texu_wnd *prevwnd);

void
TexuSaveCursorPosition(texu_wnd* wnd);

void
TexuRestoreCursorPosition(texu_wnd* wnd);

/* menus */
texu_menu*              TexuCreateMenu(texu_wnd* owner, texu_ui32 id);
void                    TexuDestroyMenu(texu_menu* menu);
texu_popup_menu*        TexuAddPopupMenu(
                            texu_menu* menu,
                            const texu_char* text,
                            texu_bool enable,
                            const texu_char* info);
texu_popup_menu_item*   TexuAddPopupMenuItem(
                            texu_menu* menu,
                            texu_popup_menu* popup,
                            const texu_char* text,
                            texu_ui32 itemid,
                            texu_bool enable,
                            const texu_char* info);
texu_popup_menu*        TexuGetPopupMenu(texu_menu* menu, texu_ui32 idx);
texu_popup_menu_item*   TexuGetPopupMenuItem(
                            texu_menu* menu,
                            texu_popup_menu* popup,
                            texu_ui32 popupid);
texu_bool               TexuEnablePopupMenu(texu_menu* menu, texu_ui32 idx, texu_bool enable);
texu_bool               TexuEnablePopupMenuItem(texu_menu* menu, texu_popup_menu* popup, texu_ui32 id, texu_bool enable);
texu_menu*              TexuCreateMenus(texu_wnd* wnd, texu_ui32 id, const texu_menu_template* templ, texu_i32 npopups);

/* device context */
texu_cio*               TexuGetDC();
texu_i32                TexuGetColor(texu_cio* cio, texu_i32 clridx);
#if (defined WIN32 && defined _WINDOWS)
texu_i32                TexuDrawHRects(texu_cio* cio, texu_rect* rect, texu_i32* widths, texu_i32 nwidth, texu_ui32 color, texu_ui32 bgcolor);
texu_i32                TexuDrawVRects(texu_cio* cio, texu_rect* rect, texu_i32* heights, texu_i32 nheight, texu_ui32 color, texu_ui32 bgcolor);
texu_i32                TexuDrawFrame(texu_cio* cio, const texu_char* text, texu_rect* rect, texu_ui32 color, texu_ui32 bgcolor);
texu_i32                TexuDrawRect(texu_cio* cio, texu_rect* rect, texu_ui32 textcolor, texu_ui32 bgcolor);
#else
texu_i32                TexuDrawHRects(texu_cio* cio, texu_rect* rect, texu_i32* widths, texu_i32 nwidth, texu_i32 attrs);
texu_i32                TexuDrawVRects(texu_cio* cio, texu_rect* rect, texu_i32* heights, texu_i32 nheight, texu_i32 attrs);
texu_i32                TexuDrawFrame(texu_cio* cio, const texu_char* text, texu_rect* rect, texu_i32 attrs);
#endif


#ifdef __cplusplus
}
#endif

#endif /*_TEXU_H_*/

