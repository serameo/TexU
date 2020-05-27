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
    parent = (genv ? texu_env_get_desktop(genv) : 0);
  }
  rc = texu_wnd_create(wnd, parent, &attrs);

  if (rc != TEXU_OK)
  {
    texu_wnd_del(wnd);
    return 0;
  }
  return wnd;
}

void
TexuDestroyWindow(
  texu_wnd*  wnd
)
{
  texu_wnd_destroy(wnd);
  texu_wnd_del(wnd);
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







#ifdef __cplusplus
}
#endif
