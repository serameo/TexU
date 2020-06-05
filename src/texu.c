/*
#
# File Name: texu.c
# Author: Seree Rakwong
# Date: 15-MAY-2020
#
*/

#include <stdlib.h>
#include <string.h>
#include "texu.h"

#ifdef __cplusplus
extern "C" {
#endif


void               TexuPushWindow(
                     texu_wnd*  wnd
                   );
texu_wnd*          TexuPopWindow();
texu_wnd*          TexuTopWindow();

/* global variables */
texu_env*  genv = 0;



texu_status
TexuStartup()
{
  genv = texu_env_new();
  return (genv ? TEXU_OK : TEXU_ERROR);
}

texu_status
TexuShutdown()
{
  texu_env_del(genv);
  return TEXU_OK;
}

texu_status
TexuRun()
{
  return (genv ? texu_env_run(genv) : TEXU_ERROR);
}

texu_env*
TexuGetEnv()
{
  return genv;
}

texu_cio*
TexuGetDC()
{
  return (genv ? texu_env_get_cio(genv) : 0);
}

void
TexuExit()
{
  if (genv)
  {
    texu_env_exit(genv);
  }
}

texu_status
TexuRegisterClass(
  texu_char*   clsname,
  texu_wndproc wndproc
)
{
  return (genv ? texu_env_register_cls(genv, clsname, wndproc) : TEXU_ERROR);
}

texu_wnd*
TexuMessageBox(
  texu_char* caption,
  texu_char* text,
  texu_wnd*  owner,
  texu_ui32  id,
  texu_ui32  buttons,
  void*      userdata
)
{
  texu_wnd* wnd = 0;
  texu_wnd_attrs attrs;
  texu_status rc = TEXU_OK;
  texu_wnd* desktop = texu_env_get_desktop(genv);
  texu_wnd* childwnd = 0;


  if (!genv)
  {
    return 0;
  }
  wnd = texu_wnd_new(genv);
  if (!wnd)
  {
    return 0;
  }
  memset(&attrs, 0, sizeof(attrs));
  attrs.y          = 0;
  attrs.x          = 0;
  attrs.height     = 0;
  attrs.width      = 0;
  attrs.enable     = TEXU_TRUE;
  attrs.visible    = TEXU_TRUE;
  attrs.text       = text;
  attrs.normalcolor    = TEXU_CIO_COLOR_BLACK_WHITE;
  attrs.disabledcolor  = TEXU_CIO_COLOR_BLACK_WHITE;
  attrs.id         = id;
  attrs.clsname    = TEXU_MSGBOX_CLASS;
  attrs.userdata   = userdata;
  attrs.style      = buttons;
  attrs.exstyle    = 0;

  rc = texu_wnd_create(wnd, desktop, &attrs);

  if (rc != TEXU_OK)
  {
    texu_wnd_del(wnd);
    return 0;
  }
  
  texu_wnd_send_msg(wnd, TEXU_MBM_SETOWNER, (texu_i64)owner, 0);
  texu_wnd_send_msg(wnd, TEXU_MBM_SETCAPTION, (texu_i64)caption, 0);
  
  TexuPushWindow(wnd);
  
  childwnd = texu_wnd_get_activechild(wnd);
  if (childwnd)
  {
    texu_wnd_send_msg(childwnd, TEXU_WM_SETFOCUS, 0, 0);
  }
  
  TexuShowWindow(wnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(wnd);
  
  return wnd;
}

texu_wnd*
TexuCreateWindow(
  texu_char* text,
  texu_char* clsname,
  texu_ui32  style,
  texu_ui32  exstyle,
  texu_i32   y,
  texu_i32   x,
  texu_i32   h,
  texu_i32   w,
  texu_wnd*  parent,
  texu_i32   id,
  void*      userdata
)
{
  texu_wnd* wnd = 0;
  texu_wnd_attrs attrs;
  texu_status rc = TEXU_OK;
  texu_wnd* desktop = texu_env_get_desktop(genv);
  texu_wnd* childwnd = 0;

  if (!genv)
  {
    return 0;
  }
  wnd = texu_wnd_new(genv);
  if (!wnd)
  {
    return 0;
  }
  memset(&attrs, 0, sizeof(attrs));
  attrs.y          = y;
  attrs.x          = x;
  attrs.height     = h;
  attrs.width      = w;
  attrs.enable     = TEXU_TRUE;
  attrs.visible    = TEXU_TRUE;
  attrs.text       = text;
  attrs.normalcolor    = TEXU_CIO_COLOR_WHITE_BLACK;
  attrs.disabledcolor  = TEXU_CIO_COLOR_WHITE_BLACK;
  attrs.id         = id;
  attrs.clsname    = clsname;
  attrs.userdata   = userdata;
  attrs.style      = style;
  attrs.exstyle    = exstyle;

  if (!(parent))
  {
    parent = desktop;
  }
  rc = texu_wnd_create(wnd, parent, &attrs);

  if (rc != TEXU_OK)
  {
    texu_wnd_del(wnd);
    return 0;
  }
  
  if (parent == desktop)
  {
    TexuPushWindow(wnd);
    
    childwnd = texu_wnd_get_activechild(wnd);
    if (childwnd)
    {
      texu_wnd_send_msg(childwnd, TEXU_WM_SETFOCUS, 0, 0);
    }
  }

  return wnd;
}

void
TexuDestroyWindow(
  texu_wnd*  wnd
)
{
  texu_wnd* topwnd = 0;
  TexuPopWindow();

  texu_wnd_destroy(wnd);
  texu_wnd_del(wnd);
  
  topwnd = TexuTopWindow();
  TexuShowWindow(topwnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(topwnd);
}

void
TexuPushWindow(
  texu_wnd*  wnd
)
{
  /* can push a window if its parent is null */
  /* because it is a frame window */
  if (!(genv) || !(wnd))
  {
    return;
  }
  
  texu_env_push_wnd(genv, wnd);
}

texu_wnd*
TexuTopWindow()
{
  return (genv ? texu_env_top_wnd(genv) : 0);
}

texu_wnd*
TexuPopWindow()
{
  return (genv ? texu_env_pop_wnd(genv) : 0);
}

texu_i32
TexuGetMaxY()
{
  return LINES;
}

texu_i32
TexuGetMaxX()
{
  return COLS;
}

void
TexuShowWindow(
  texu_wnd*  wnd,
  texu_i32   show
)
{
  texu_wnd_visible(wnd, (TEXU_WS_SHOW == show));
}

void
TexuInvalidateWindow(
  texu_wnd*  wnd
)
{
  texu_wnd_invalidate(wnd);
}

texu_i64
TexuSendMessage(
  texu_wnd*     wnd,
  texu_ui32     msg,
  texu_i64      param1,
  texu_i64      param2
)
{
  return texu_wnd_send_msg(wnd, msg, param1, param2);
}


texu_wnd*
TexuGetWindowItem(
  texu_wnd*     wnd,
  texu_ui32     id
)
{
  return texu_wnd_find_child(wnd, id);
}

void
TexuGetWindowText(
  texu_wnd*         wnd,
  const texu_char*  text
)
{
  texu_wnd_set_text(wnd, (text ? text : ""));
}

void
TexuSetColor(
  texu_wnd*     wnd,
  texu_i32      color,
  texu_i32      discolor
)
{
  texu_wnd_set_color(wnd, color, discolor);
}



void
TexuAddKeyCommands(
  texu_wnd*   wnd,
  texu_i32    key,
  texu_ui32   cmd
)
{
  texu_wnd_add_keycmd(wnd, key, cmd);
}







#ifdef __cplusplus
}
#endif

