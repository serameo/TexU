/*
#
# File Name: texui.c
# Author: Seree Rakwong
# Date: 13-MAY-2020
#
*/

#include <stdlib.h>
#include <string.h>
#include "texulib.h"
#include "texutils.h"
#include "texui.h"
#include "texumenu.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
# TexU environment object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#define TEXU_ENV_SCRFILE   "./.texuenv"

struct texu_env
{
  texu_stack*       frames;  /* hold window frames */
  texu_cio*         cio;     /* console input/output */
  texu_wnd*         desktop; /* the root of all window frames */
  texu_list*        wndcls;  /* the registered classes */
  texu_bool         exit;
  texu_i32          ypos;   /* cursor position */
  texu_i32          xpos;   /* cursor position */
  FILE*             scrfp;  /* to write/read screen */
};
struct texu_env_wndcls
{
  texu_char*        clsname;
  texu_wndproc      proc;
};
typedef struct texu_env_wndcls texu_env_wndcls;

/* internally window procedure */
/* see texuproc.c */
texu_i64          _TexuDesktopProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          _TexuMsgBoxProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);


texu_i64          _TexuLabelProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          _TexuEditProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          _TexuListBoxProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          _TexuStatusBarProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);

/* see texuctrl.c */
texu_i64          _TexuListCtrlProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          _TexuTreeCtrlProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          _TexuUpDownCtrlProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          _TexuProgressBarProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);

/* menu texumenu.c */
texu_i64          _TexuMenuProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          _TexuMenuWndProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);




void              _texu_env_init_cls(texu_env*);
texu_wndproc      _texu_env_find_wndproc(texu_env*, texu_char*);
FILE*             _texu_env_init_screen(texu_env* env);


void
_texu_env_init_cls(texu_env* env)
{
  texu_env_register_cls(env, TEXU_DESKTOP_CLASS,      _TexuDesktopProc);
  texu_env_register_cls(env, TEXU_MSGBOX_CLASS,       _TexuMsgBoxProc);

  texu_env_register_cls(env, TEXU_LABEL_CLASS,        _TexuLabelProc);
  texu_env_register_cls(env, TEXU_EDIT_CLASS,         _TexuEditProc);
  texu_env_register_cls(env, TEXU_LISTBOX_CLASS,      _TexuListBoxProc);
  texu_env_register_cls(env, TEXU_LISTCTRL_CLASS,     _TexuListCtrlProc);
  texu_env_register_cls(env, TEXU_TREECTRL_CLASS,     _TexuTreeCtrlProc);
  texu_env_register_cls(env, TEXU_UPDOWNCTRL_CLASS,   _TexuUpDownCtrlProc);
  texu_env_register_cls(env, TEXU_PROGRESSBAR_CLASS,  _TexuProgressBarProc);
  texu_env_register_cls(env, TEXU_STATUSBAR_CLASS,    _TexuStatusBarProc);
  
  texu_env_register_cls(env, TEXU_MENU_CLASS,         _TexuMenuProc);
  texu_env_register_cls(env, TEXU_MENUWND_CLASS,      _TexuMenuWndProc);
  
}

FILE*
_texu_env_init_screen(texu_env* env)
{
  FILE* fp = fopen(TEXU_ENV_SCRFILE, "w");
  return fp;
}

texu_wndproc
_texu_env_find_wndproc(texu_env* env, texu_char* clsname)
{
  texu_wndproc wndproc = 0;
  texu_env_wndcls* wndcls = 0;
  texu_list_item* item = texu_list_first(env->wndcls);

  while (item != 0)
  {
    wndcls = (texu_env_wndcls*)item->data;
    if (0 == strcmp(wndcls->clsname, clsname))
    {
      /* registered already */
      wndproc = wndcls->proc;
      break;
    }
    item = item->next;
  }
  return wndproc;
}

texu_status
_texu_env_create_desktop(texu_env* env)
{
  texu_status rc = TEXU_OK;
  texu_wnd_attrs attrs;
  texu_wnd* desktop = 0;

  desktop = texu_wnd_new(env); /* no parent */
  if (!desktop)
  {
    return TEXU_NOMEM;
  }

  memset(&attrs, 0, sizeof(attrs));
  attrs.y          = 0;
  attrs.x          = 0;
  attrs.height     = LINES;
  attrs.width      = COLS;
  attrs.enable     = TEXU_TRUE;
  attrs.visible    = TEXU_TRUE;
  attrs.text       = 0;
  attrs.normalcolor     = TEXU_CIO_COLOR_WHITE_BLACK;
  attrs.disabledcolor   = TEXU_CIO_COLOR_WHITE_BLACK;
  attrs.focuscolor      = TEXU_CIO_COLOR_WHITE_BLACK;
  attrs.id         = 1;
  attrs.clsname    = TEXU_DESKTOP_CLASS;
  attrs.userdata   = 0;
  attrs.style      = 0;
  attrs.exstyle    = 0;

  rc = texu_wnd_create(desktop, 0, &attrs);
  if (TEXU_OK == rc)
  {
    env->desktop = desktop;
  }
  return rc;
}

texu_status
texu_env_register_cls(
  texu_env* env,
  texu_char* clsname,
  texu_wndproc proc)
{
  texu_status rc = TEXU_OK;
  texu_env_wndcls* wndcls = 0;
  texu_wndproc wndproc = _texu_env_find_wndproc(env, clsname);

  /* registered already */
  if (wndproc)
  {
    return rc;
  }

  /* allocate the new memory */
  wndcls = (texu_env_wndcls*)malloc(sizeof(texu_env_wndcls));
  if (!wndcls)
  {
    return TEXU_NOMEM;
  }

  /* add the new registered class */
  wndcls->clsname = clsname;
  wndcls->proc    = proc;
  texu_list_add(env->wndcls, (texu_i64)wndcls);

  return rc;
}

texu_env*
texu_env_new()
{
  texu_status rc = TEXU_OK;
  texu_env* env = (texu_env*)malloc(sizeof(texu_env));
  if (env)
  {
    memset(env, 0, sizeof(texu_env));

    /* console input/output */
    env->cio = texu_cio_new();
    texu_cio_init(env->cio);
    
    env->scrfp = _texu_env_init_screen(env);

    /* window classes */
    env->wndcls = texu_list_new();

    /* register internal classes */
    _texu_env_init_cls(env);

    /* desktop */
    rc = _texu_env_create_desktop(env);
    if (rc != TEXU_OK)
    {
      texu_list_del(env->wndcls);
      texu_cio_release(env->cio);
      free(env);
      env = 0;
      return env;
    }

    /* window frames */
    env->frames = texu_stack_new(TEXU_ENV_MAX_FRAMES+1);
    texu_stack_push(env->frames, (texu_i64)env->desktop);
  }
  return env;
}

void
texu_env_del(texu_env* env)
{
  if (env)
  {
    fclose(env->scrfp);
    texu_list_del(env->wndcls);
    texu_stack_del(env->frames);
    texu_wnd_del(env->desktop);
    texu_cio_release(env->cio);
    free(env);
    env = 0;
  }
}

texu_status
texu_env_run(texu_env* env)
{
  texu_i32 ch = 0;
  texu_i32 ch2 = 0;
  texu_wnd* activewnd = 0;
  texu_i32 altpressed = 0;
  texu_i32 ctlpressed = 0;
  texu_char* keypressed;

  while (!(env->exit))
  {
    altpressed = 0;
    ch = texu_cio_getch(env->cio);
    keypressed = keyname(ch);
    if ('^' == keypressed[0] && keypressed[1] != 0)
    {
      if ('[' == keypressed[1])
      {
        texu_cio_nodelay(env->cio, TEXU_TRUE);
        ch2 = texu_cio_getch(env->cio);
        texu_cio_nodelay(env->cio, TEXU_FALSE);
        
        keypressed = keyname(ch2);
        if (-1 != ch2)
        {
          altpressed = 1;
          if ('^' == keypressed[0])
          {
            ctlpressed = 2;
          }
          ch = keypressed[1];
        }
      }
      else if ('J' == keypressed[1] || 10 == ch)
      {
        /* enter key LF */
      }
      else
      {
        ctlpressed = 2;
        ch = keypressed[1];
      }
    }

    activewnd = (texu_wnd*)texu_stack_top(env->frames);
    if (!activewnd)
    {
      break;
    }
    texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch,
      (texu_i64)(altpressed|ctlpressed));
  }
  return TEXU_OK;
}

texu_cio*
texu_env_get_cio(texu_env* env)
{
  return env->cio;
}

void
texu_env_exit(texu_env* env)
{
  env->exit = TEXU_TRUE;
}

texu_wnd*
texu_env_get_desktop(texu_env* env)
{
  return env->desktop;
}

texu_status
texu_env_push_wnd(texu_env* env, texu_wnd* wnd)
{
  texu_status rc = TEXU_ERROR;
  texu_wnd* parent = texu_wnd_get_parent(wnd);
  if (parent == env->desktop)
  {
    /* can push if the parent window is the desktop */
    texu_stack_push(env->frames, (texu_i64)wnd);
    rc = TEXU_OK;
  }
  return rc;
}

texu_wnd*
texu_env_pop_wnd(texu_env* env)
{
  texu_wnd* topwnd = (texu_wnd*)texu_stack_top(env->frames);
  if (topwnd != env->desktop)
  {
    texu_stack_pop(env->frames);
    return topwnd;
  }
  return 0;
}


texu_wnd*
texu_env_top_wnd(texu_env* env)
{
  texu_wnd* topwnd = (texu_wnd*)texu_stack_top(env->frames);
  return topwnd;
}



texu_status
texu_env_save_curpos(texu_env* env, texu_i32 ypos, texu_i32 xpos)
{
  texu_status rc = texu_cio_getyx(env->cio, ypos, xpos);
  
  env->ypos = ypos;
  env->xpos = xpos;
  
  return rc;
}

texu_status
texu_env_restore_curpos(texu_env* env)
{
  texu_status rc = TEXU_OK;
  texu_i32 ypos = env->ypos;
  texu_i32 xpos = env->xpos;
  
  env->ypos = 0;
  env->xpos = 0;
  rc = texu_cio_gotoyx(env->cio, ypos, xpos);
  
  return rc;
}


void
texu_env_save_screen(texu_env* env)
{
  texu_cio_save_screen(env->cio, env->scrfp);
}


void
texu_env_restore_screen(texu_env* env)
{
  texu_cio_restore_screen(env->cio, env->scrfp);
}

/* cursor position */
/*
# TexU window object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_wnd
{
  texu_wnd*         parent;
  texu_list*        children;
  texu_wnd*         activechild;
  texu_wndproc      wndproc;
  texu_env*         env;     /* console input/output */
  texu_list*        keycmds;
  texu_bool         lockedupdate;
/*
  texu_wnd_attrs    attrs;
*/
  texu_i32          y;
  texu_i32          x;
  texu_i32          height;
  texu_i32          width;
  texu_ui32         style;
  texu_ui32         exstyle;
  texu_bool         enable;
  texu_bool         visible;
  texu_char         text[TEXU_MAX_WNDTEXT+1];
  texu_i32          normalcolor;
  texu_i32          disabledcolor;
  texu_i32          focuscolor;
  texu_ui32         id;
  texu_char*        clsname;
  void*             userdata;
  
  texu_menu*        menu;
};

/*
#-------------------------------------------------------------------------------
# TexU default window procedure
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

texu_i32           _TexuDefWndProc_OnChar(texu_wnd*, texu_i32 ch, texu_i32 alt);
texu_i32           _TexuDefWndProc_OnKillFocus(texu_wnd*, texu_wnd* nextwnd);
void               _TexuDefWndProc_OnSetFocus(texu_wnd*, texu_wnd* prevwnd);
void               _TexuDefWndProc_OnSetText(texu_wnd* wnd, const texu_char* text);
texu_i32           _TexuDefWndProc_OnGetText(texu_wnd* wnd, texu_char* text, texu_i32 textlen);
texu_status        _TexuDefWndProc_OnCreate(texu_wnd*, texu_wnd_attrs*);
void               _TexuDefWndProc_OnDestroy(texu_wnd*);
void               _TexuDefWndProc_OnPaint(texu_wnd*, texu_cio*);
texu_bool          _TexuDefWndProc_OnShow(texu_wnd*, texu_bool);
texu_bool          _TexuDefWndProc_OnEnable(texu_wnd*, texu_bool);
void               _TexuDefWndProc_OnEraseBg(texu_wnd*, texu_cio*);
texu_i64           _TexuDefWndProc_OnMsg(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2);
void               _TexuDefWndProc_OnMove(texu_wnd*, texu_rect*, texu_bool);
void               _TexuDefWndProc_OnCommand(texu_wnd*, texu_ui32);
void               _TexuDefWndProc_OnNotify(texu_wnd*, texu_wnd_notify*);
texu_menu*         _TexuDefWndProc_OnSetMenu(texu_wnd*, texu_menu*);
void               _TexuDefWndProc_OnRedrawMenu(texu_wnd*);
void               _TexuDefWndProc_OnEnterMenu(texu_wnd*, texu_i32, texu_i32);
void               _TexuDefWndProc_OnLeaveMenu(texu_wnd*);

texu_wnd*
_TexuDefWndProc_OpenMenuWnd(
  texu_wnd*  owner,
  texu_menu*  menu
);



texu_menu*
_TexuDefWndProc_OnSetMenu(texu_wnd* wnd, texu_menu* menu)
{
  texu_menu* oldmenu = wnd->menu;
  wnd->menu = menu;
  return oldmenu;
}

void
_TexuDefWndProc_OnRedrawMenu(texu_wnd* wnd)
{
  texu_wnd_invalidate(texu_menu_get_menubar(wnd->menu));
}




texu_wnd*
_TexuDefWndProc_OpenMenuWnd(
  texu_wnd*  owner,
  texu_menu*  menu
)
{
  texu_wnd* wnd = 0;
  texu_wnd_attrs attrs;
  texu_status rc = TEXU_OK;
  texu_env* env = texu_wnd_get_env(owner);
  texu_wnd* desktop = texu_env_get_desktop(env);


  wnd = texu_wnd_new(env);
  if (!wnd)
  {
    return 0;
  }
  memset(&attrs, 0, sizeof(attrs));
  attrs.y          = 0;
  attrs.x          = 0;
  attrs.height     = 1;
  attrs.width      = COLS;
  attrs.enable     = TEXU_TRUE;
  attrs.visible    = TEXU_TRUE;
  attrs.text       = "";
  attrs.normalcolor    = TEXU_CIO_COLOR_BLACK_WHITE;
  attrs.disabledcolor  = TEXU_CIO_COLOR_BLACK_WHITE;
  attrs.id         = -1;
  attrs.clsname    = TEXU_MENUWND_CLASS;
  attrs.userdata   = menu;
  attrs.style      = 0;
  attrs.exstyle    = 0;

  rc = texu_wnd_create(wnd, desktop, &attrs);

  if (rc != TEXU_OK)
  {
    texu_wnd_del(wnd);
    return 0;
  }
  
  texu_wnd_send_msg(wnd, TEXU_MBM_SETOWNER, (texu_i64)owner, 0);
  
  texu_env_push_wnd(env, wnd);
  
  texu_wnd_visible(wnd, TEXU_TRUE);
  texu_wnd_invalidate(wnd);
  
  return wnd;
}



void
_TexuDefWndProc_OnEnterMenu(texu_wnd* wnd, texu_i32 ch, texu_i32 alt)
{
  /*open the new window to draw menu*/
  texu_i32 selmenu = 0;
  texu_tree_item* curitem = 0;
  /*
  texu_env_save_screen(wnd->env);
  */
  selmenu = ch - '0' - 1;
  curitem = texu_menu_get_menu(wnd->menu, selmenu);
  if (!(curitem))
  {
    curitem = texu_menu_get_menu(wnd->menu, 0);
    if (!(curitem))
    {
      return;
    }
  }
  texu_menu_set_curmenu(wnd->menu, curitem);
  texu_menu_set_curmenuitem(wnd->menu, curitem->firstchild);
  _TexuDefWndProc_OpenMenuWnd(wnd, wnd->menu);
}

void
_TexuDefWndProc_OnLeaveMenu(texu_wnd* wnd)
{
  /*open the new window to draw menu*/
  /*
  texu_env_restore_screen(wnd->env);
  */
  texu_wnd_send_msg(wnd, TEXU_WM_REDRAWMENU, 0, 0);
}

void
_TexuDefWndProc_OnCommand(texu_wnd* wnd, texu_ui32 id)
{
}

void
_TexuDefWndProc_OnNotify(texu_wnd* wnd, texu_wnd_notify* notify)
{
}


texu_i64
_TexuDefWndProc_OnMsg(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  return 0;
}

void
_TexuDefWndProc_OnMove(texu_wnd* wnd, texu_rect* rect, texu_bool redraw)
{
  wnd->y = rect->y;
  wnd->x = rect->x;
  wnd->height = rect->lines;
  wnd->width  = rect->cols;
  if (redraw)
  {
    texu_wnd_invalidate(wnd);
  }
}


texu_bool
_TexuDefWndProc_OnShow(texu_wnd* wnd, texu_bool visible)
{
  texu_bool oldvisible = wnd->visible;
  wnd->visible = visible;

  texu_wnd_send_msg(wnd, TEXU_WM_PAINT, (texu_i64)wnd->env->cio, 0);
  return oldvisible;
}

texu_bool
_TexuDefWndProc_OnEnable(texu_wnd* wnd, texu_bool enable)
{
  texu_bool oldenable = wnd->enable;
  wnd->enable = enable;

  texu_wnd_send_msg(wnd, TEXU_WM_PAINT, (texu_i64)wnd->env->cio, 0);
  return oldenable;
}

void
_TexuDefWndProc_OnPaint(texu_wnd* wnd, texu_cio* cio)
{
}

void
_TexuDefWndProc_OnEraseBg(texu_wnd* wnd, texu_cio* cio)
{
  texu_char zblank[256];
  texu_i32 width = 0;
  texu_i32 height = 0;
  texu_i32 line = 0;

  width = wnd->width;
  height = wnd->height;

  memset(zblank, ' ', width);
  zblank[width] = 0;

  for (line = 0; line < height; ++line)
  {
    texu_cio_putstr_attr(
      cio,
      line + wnd->y, wnd->x,
      zblank,
      COLOR_PAIR(wnd->normalcolor));
  }
}

texu_status
_TexuDefWndProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
  return TEXU_OK;
}

void
_TexuDefWndProc_OnDestroy(texu_wnd* wnd)
{
  texu_menu* menu = wnd->menu;
  if (menu)
  {
    texu_menu_del(menu);
  }
}

void
_TexuDefWndProc_OnSetText(texu_wnd* wnd, const texu_char* text)
{
  texu_ui32 textlen = strlen(text);
  /*texu_ui32 len = strlen(wnd->text); */
  
  texu_wnd* frame = texu_wnd_get_frame(wnd);
  texu_wnd* activewnd = texu_wnd_get_activechild(frame);
  texu_i32 y = 0;
  texu_i32 x = 0;
  texu_i32 width = 0;
  texu_cio* cio = 0;

  if (text && textlen > 0)
  {
    strcpy(wnd->text, text);
  }
  texu_wnd_invalidate(wnd);
  
  if (activewnd)
  {
    y = texu_wnd_get_y(activewnd);
    x = texu_wnd_get_x(activewnd);
    width = texu_wnd_get_width(activewnd);
    cio = texu_env_get_cio(activewnd->env);
    texu_cio_gotoyx(cio, y, x+width-1);
  }
}

texu_i32
_TexuDefWndProc_OnGetText(texu_wnd* wnd, texu_char* text, texu_i32 textlen)
{
  texu_ui32 len = strlen(wnd->text);

  if (textlen < 0 || 0 == text)
  {
    return strlen(wnd->text);
  }

  memset(text, 0, textlen);
  textlen = TEXU_MIN(len, textlen);
  memcpy(text, wnd->text, textlen);

  return textlen;
}


texu_i32
_TexuDefWndProc_OnKillFocus(texu_wnd* wnd, texu_wnd* nextwnd)
{
  return TEXU_OK;
}

void
_TexuDefWndProc_OnSetFocus(texu_wnd* wnd, texu_wnd* prevwnd)
{
}

texu_i32
_TexuDefWndProc_OnChar(texu_wnd* wnd, texu_i32 ch, texu_i32 alt)
{
  texu_i64 rc = 0;
  texu_wnd* activewnd = texu_wnd_get_activechild(wnd);
  texu_wnd* nextwnd = 0;
  texu_wnd* parent = texu_wnd_get_parent(wnd);
  texu_wnd* desktop = texu_env_get_desktop(wnd->env);
  texu_i32 y = 0;
  texu_i32 x = 0;
  texu_i32 width = 0;
  texu_cio* cio = 0;
  texu_wnd_keycmd* keycmd = 0;
  texu_menu* menu = 0;

  keycmd = texu_wnd_find_keycmd(wnd, ch, alt);
  if (keycmd && parent == desktop)
  {
    /* if there are any hotkey registered */
    return texu_wnd_send_msg(wnd, TEXU_WM_COMMAND, (texu_i64)keycmd->cmd, alt);
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
        rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
        if (rc != TEXU_OK)
        {
          return -1;
        }
        rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);
        y = texu_wnd_get_y(nextwnd);
        x = texu_wnd_get_x(nextwnd);
        width = texu_wnd_get_width(nextwnd);
        
        cio = texu_wnd_get_cio(wnd);
        texu_cio_gotoyx(cio, y, x+width-1);

        /* the new active window */
        wnd->activechild = nextwnd;
        return 1;
      }
      else
      {
        return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
      }
    } /* child window is active */
    else
    {
      /* the current frame window is active */
      if (activewnd)
      {
        activewnd = texu_wnd_get_activechild(activewnd);
        return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
      }
    }
  }
  else
  {
    if (activewnd)
    {
      return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
    }
  }
  return 0;
}


texu_i64
TexuDefWndProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_SETMENU:
      _TexuDefWndProc_OnSetMenu(wnd, (texu_menu*)param1);
      return 0;

    case TEXU_WM_REDRAWMENU:
      _TexuDefWndProc_OnRedrawMenu(wnd);
      return 0;

    case TEXU_WM_ENTERMENU:
      _TexuDefWndProc_OnEnterMenu(wnd, (texu_i32)param1, (texu_i32)param2);
      return 0;

    case TEXU_WM_LEAVEMENU:
      _TexuDefWndProc_OnLeaveMenu(wnd);
      return 0;

    case TEXU_WM_NOTIFY:
      _TexuDefWndProc_OnNotify(wnd, (texu_wnd_notify*)param1);
      return 0;

    case TEXU_WM_COMMAND:
      _TexuDefWndProc_OnCommand(wnd, (texu_ui32)param1);
      return 0;
      
    case TEXU_WM_MOVE:
      _TexuDefWndProc_OnMove(wnd, (texu_rect*)param1, (texu_bool)param2);
      return 0;

    case TEXU_WM_SHOW:
      return _TexuDefWndProc_OnShow(wnd, (texu_bool)param1);

    case TEXU_WM_ENABLE:
      return _TexuDefWndProc_OnEnable(wnd, (texu_bool)param1);

    case TEXU_WM_CHAR:
      return _TexuDefWndProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);

    case TEXU_WM_SETFOCUS:
      _TexuDefWndProc_OnSetFocus(wnd, (texu_wnd*)param1);
      break;

    case TEXU_WM_KILLFOCUS:
      return _TexuDefWndProc_OnKillFocus(wnd, (texu_wnd*)param1);

    case TEXU_WM_SETTEXT:
      _TexuDefWndProc_OnSetText(wnd, (const texu_char*)param1);
      break;

    case TEXU_WM_GETTEXT:
      return _TexuDefWndProc_OnGetText(wnd, (texu_char*)param1, (texu_i32)param2);

    case TEXU_WM_PAINT:
      _TexuDefWndProc_OnPaint(wnd, (texu_cio*)param1);
      break; 

    case TEXU_WM_ERASEBG:
      _TexuDefWndProc_OnEraseBg(wnd, (texu_cio*)param1);
      break; 

    case TEXU_WM_CREATE:
      return _TexuDefWndProc_OnCreate(wnd, (texu_wnd_attrs*)param1);

    case TEXU_WM_DESTROY:
      _TexuDefWndProc_OnDestroy(wnd);
      break;
  }
  return 0;
}

/*
#
#
*/

texu_list_item*         _texu_wnd_find_child(texu_wnd* wnd, texu_ui32 id);
texu_wnd*               _texu_wnd_is_window(texu_wnd* parent, texu_wnd* wnd);
texu_list_item*         _texu_wnd_find_keycmd(texu_wnd* wnd, texu_i32 key, texu_i32 alt);

texu_wnd*
_texu_wnd_is_window(texu_wnd* parent, texu_wnd* wnd)
{
  texu_wnd* findwnd = 0;
  findwnd = texu_wnd_firstchild(parent);
  
  if (parent && wnd)
  {
    if (parent == wnd)
    {
      return parent;
    }
  }
  if (findwnd)
  {
    if (findwnd == wnd)
    {
      return findwnd;
    }
    findwnd = _texu_wnd_is_window(findwnd, wnd);
    if (findwnd)
    {
      return findwnd;
    }
  }
  else
  {
    findwnd = texu_wnd_nextwnd(parent);
    while (findwnd)
    {
      findwnd = _texu_wnd_is_window(findwnd, wnd);
      if (findwnd)
      {
        return findwnd;
      }
      findwnd = texu_wnd_nextwnd(findwnd);
    }
  }
  return 0;
}

texu_wnd*
texu_wnd_is_window(texu_wnd* wnd)
{
  texu_wnd* parent = (wnd ? wnd->parent : 0);
/*  texu_wnd* findwnd = 0;*/
  if (!wnd)
  {
    return 0;
  }
  if (!parent)
  {
    parent = texu_env_get_desktop(wnd->env);
  }
  return _texu_wnd_is_window(parent, wnd);
}

texu_wnd*
texu_wnd_new(texu_env* env)
{
  texu_wnd* wnd = (texu_wnd*)malloc(sizeof(texu_wnd));
  if (wnd)
  {
    memset(wnd, 0, sizeof(texu_wnd));
    wnd->env = env;
    wnd->children = texu_list_new();
    wnd->keycmds  = texu_list_new();
  }
  return wnd;
}

texu_status
texu_wnd_create(texu_wnd* wnd, texu_wnd* parent, const texu_wnd_attrs* attrs)
{
  texu_status rc = TEXU_OK;
  texu_wndproc wndproc = 0;

  /* find the window procedure */
  wndproc = _texu_env_find_wndproc(wnd->env, attrs->clsname);
  if (!wndproc)
  {
    return TEXU_NOTREGISTERED_CLASS;
  }
  wnd->y = attrs->y;
  wnd->x = attrs->x;
  wnd->height = attrs->height;
  wnd->width = attrs->width;
  wnd->style = attrs->style;
  wnd->exstyle = attrs->exstyle;
  wnd->enable = attrs->enable;
  wnd->visible = attrs->visible;
  if (attrs->text && strlen(attrs->text))
  {
    strcpy(wnd->text, attrs->text);
  }
  wnd->normalcolor = attrs->normalcolor;
  wnd->disabledcolor = attrs->disabledcolor;
  wnd->focuscolor = attrs->focuscolor;
  wnd->id = attrs->id;
  wnd->clsname = attrs->clsname;
  wnd->userdata = attrs->userdata;
  wnd->wndproc = wndproc;
  wnd->parent = parent;
  if (parent)
  {
    texu_wnd_add_child(parent, wnd);
  }
  wnd->lockedupdate = TEXU_FALSE;

  rc = texu_wnd_send_msg(wnd, TEXU_WM_CREATE, (texu_i64)attrs, 0);
  return rc;
}

void
texu_wnd_del(texu_wnd* wnd)
{
  if (wnd)
  {
    texu_list_del(wnd->children);
    texu_list_del(wnd->keycmds);
    free(wnd);
    wnd = 0;
  }
}

void
_texu_wnd_destroy(texu_wnd* wnd)
{
  texu_wnd* childwnd = 0;
  texu_wnd* prevwnd = 0;

  if (!wnd)
  {
    return;
  }
  childwnd = texu_wnd_lastchild(wnd);
  while (childwnd)
  {
    /* prev wnd */
    prevwnd = childwnd;
    childwnd = texu_wnd_prevwnd(childwnd);
    
    _texu_wnd_destroy(prevwnd);
  }
  texu_wnd_send_msg(wnd, TEXU_WM_DESTROY, 0, 0);
}

void
texu_wnd_destroy(texu_wnd* wnd)
{
  texu_wnd* parent = wnd->parent;
  
  _texu_wnd_destroy(wnd);
  if (parent)
  {
    texu_wnd_remove_child(parent, wnd);
  }
}


texu_i64
texu_wnd_send_msg(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  texu_i64 rc = 0;

  if (wnd && wnd->wndproc)
  {
    rc = wnd->wndproc(wnd, msg, param1, param2);
  }
  return rc;
}

texu_i64
texu_wnd_invalidate(texu_wnd* wnd)
{
  texu_wnd* childwnd = 0;
  texu_wnd* activewnd = 0;
  texu_i32 y = 0;
  texu_i32 x = 0;
  texu_i32 width = 0;
  texu_cio* cio = 0;

  if (!wnd || !texu_wnd_is_visible(wnd))
  {
    return 0;
  }
  /* firstly, paint itself */
  texu_wnd_send_msg(wnd, TEXU_WM_ERASEBG, (texu_i64)(wnd->env->cio), 0);
  texu_wnd_send_msg(wnd, TEXU_WM_PAINT, (texu_i64)(wnd->env->cio), 0);

  childwnd = texu_wnd_firstchild(wnd);
  if (childwnd)
  {
    texu_wnd_invalidate(childwnd);
    childwnd = texu_wnd_nextwnd(childwnd);
    while (childwnd)
    {
        texu_wnd_invalidate(childwnd);
        childwnd = texu_wnd_nextwnd(childwnd);
    }
  }
  /* move cursor to the active window */
  activewnd = texu_wnd_get_activechild(wnd);
  if (activewnd)
  {
    y = texu_wnd_get_y(activewnd);
    x = texu_wnd_get_x(activewnd);
    width = texu_wnd_get_width(activewnd);
  }
  else
  {
    y = texu_wnd_get_y(wnd);
    x = texu_wnd_get_x(wnd);
    width = texu_wnd_get_width(wnd);
  }
  cio = texu_wnd_get_cio(wnd);
  texu_cio_gotoyx(cio, y, x+width-1);
  return 0;
}

texu_wnd*
texu_wnd_get_frame(texu_wnd* wnd)
{
  texu_wnd* framewnd = wnd;
  texu_wnd* parent = texu_wnd_get_parent(framewnd);
  texu_wnd* desktop = texu_env_get_desktop(framewnd->env);
  
  if (!wnd || wnd == desktop)
  {
    return 0;
  }
  while (parent != desktop)
  {
    framewnd = parent;
    parent = texu_wnd_get_parent(framewnd);
  }
  return framewnd;
}

texu_wnd*
texu_wnd_get_parent(texu_wnd* wnd)
{
  texu_wnd* parent = (wnd ? wnd->parent : 0);
  return parent;
}

texu_ui32
texu_wnd_get_id(texu_wnd* wnd)
{
  texu_ui32 id = (wnd ? wnd->id : 0);
  return id;
}


texu_wnd*
texu_wnd_get_activechild(texu_wnd* wnd)
{
  texu_wnd* activechild = (wnd ? wnd->activechild : 0);
  if (!activechild)
  {
    activechild = texu_wnd_get_first_activechild(wnd);
    if (activechild)
    {
      wnd->activechild = activechild;
    }
  }
  return activechild;
}

texu_wnd*
texu_wnd_get_first_activechild(texu_wnd* wnd)
{
  texu_list_item* item = (wnd->children ? texu_list_first(wnd->children) : 0);
  texu_wnd* activechild = 0;
  while (item)
  {
    activechild = (texu_wnd*)item->data;
    if (texu_wnd_is_visible(activechild) && 
        texu_wnd_is_enable(activechild))
    {
      return activechild;
    }
    item = item->next;
  }
  return 0;
}

texu_wnd*
texu_wnd_get_last_activechild(texu_wnd* wnd)
{
  texu_list_item* item = (wnd->children ? texu_list_last(wnd->children) : 0);
  texu_wnd* activechild = 0;
  while (item)
  {
    activechild = (texu_wnd*)item->data;
    if (texu_wnd_is_visible(activechild) && 
        texu_wnd_is_enable(activechild))
    {
      return activechild;
    }
    item = item->prev;
  }
  return 0;
}

texu_wnd*
texu_wnd_get_next_activechild(texu_wnd* wnd, texu_wnd* childwnd)
{
  texu_list_item* item = 0;
  texu_wnd* activechild = 0;
  if (!childwnd)
  {
    return texu_wnd_get_first_activechild(wnd);
  }
  /* find the current child window */
  item = texu_list_find_first(wnd->children, (texu_i64)childwnd);
  if (item)
  {
    item = item->next;
    while (item)
    {
      activechild = (texu_wnd*)item->data;
      if (texu_wnd_is_visible(activechild) && 
          texu_wnd_is_enable(activechild))
      {
        return activechild;
      }
      item = item->next;
    }
  }

  return 0;
}

texu_wnd*
texu_wnd_get_prev_activechild(texu_wnd* wnd, texu_wnd* childwnd)
{
  texu_list_item* item = 0;
  texu_wnd* activechild = 0;
  if (!childwnd)
  {
    return texu_wnd_get_last_activechild(wnd);
  }
  /* find */
  item = texu_list_rfind_last(wnd->children, (texu_i64)childwnd);
  if (item)
  {
    item = item->prev;
    while (item)
    {
      activechild = (texu_wnd*)item->data;
      if (texu_wnd_is_visible(activechild) && 
          texu_wnd_is_enable(activechild))
      {
        return activechild;
      }
      item = item->prev;
    }
  }

  return 0;
}

texu_wnd*
texu_wnd_firstchild(texu_wnd* wnd)
{
  texu_list_item* item = (wnd->children ? texu_list_first(wnd->children) : 0);
  return (item ? (texu_wnd*)item->data : 0);
}

texu_wnd*
texu_wnd_lastchild(texu_wnd* wnd)
{
  texu_list_item* item = (wnd->children ? texu_list_last(wnd->children) : 0);
  return (item ? (texu_wnd*)item->data : 0);
}

texu_wnd*
texu_wnd_nextwnd(texu_wnd* wnd)
{
  texu_list_item* item = 0;
  texu_wnd* parent = (wnd ? wnd->parent : 0);
  if (!parent)
  {
    return 0;
  }
  /* find */
  item = texu_list_find_first(parent->children, (texu_i64)wnd);
  if (item)
  {
    item = item->next;
  }

  return (item ? (texu_wnd*)item->data : 0);
}

texu_wnd*
texu_wnd_prevwnd(texu_wnd* wnd)
{
  texu_list_item* item = 0;
  texu_wnd* parent = (wnd ? wnd->parent : 0);
  if (!parent)
  {
    return 0;
  }
  /* find */
  item = texu_list_rfind_last(parent->children, (texu_i64)wnd);
  if (item)
  {
    item = item->prev;
  }

  return (item ? (texu_wnd*)item->data : 0);
}



texu_status
texu_wnd_visible(texu_wnd* wnd, texu_bool visible)
{
  return texu_wnd_send_msg(wnd, TEXU_WM_SHOW, (texu_i64)visible, 0);
}

texu_status
texu_wnd_enable(texu_wnd* wnd, texu_bool enable)
{
  return texu_wnd_send_msg(wnd, TEXU_WM_ENABLE, (texu_i64)enable, 0);
}

texu_bool
texu_wnd_is_visible(texu_wnd* wnd)
{
  return wnd->visible;
}
texu_bool
texu_wnd_is_enable(texu_wnd* wnd)
{
  return wnd->enable;
}

texu_i32
texu_wnd_get_width(texu_wnd* wnd)
{
  return wnd->width;
}

texu_i32
texu_wnd_get_height(texu_wnd* wnd)
{
  return wnd->height;
}

texu_i32
texu_wnd_get_y(texu_wnd* wnd)
{
  return wnd->y;
}

texu_i32
texu_wnd_get_x(texu_wnd* wnd)
{
  return wnd->x;
}

texu_ui32
texu_wnd_get_style(texu_wnd* wnd)
{
  return wnd->style;
}

void
texu_wnd_move(texu_wnd* wnd, texu_i32 y, texu_i32 x, texu_i32 w, texu_i32 h, texu_bool redraw)
{
  texu_rect rect = { y, x, h, w };
  texu_wnd_send_msg(wnd, TEXU_WM_MOVE, (texu_i64)&rect, redraw);
}


void
texu_wnd_set_style(texu_wnd* wnd, texu_ui32 style)
{
  wnd->style = style;
  texu_wnd_invalidate(wnd);
}


void
texu_wnd_set_exstyle(texu_wnd* wnd, texu_ui32 exstyle)
{
  wnd->exstyle = exstyle;
  texu_wnd_invalidate(wnd);
}

texu_ui32
texu_wnd_get_exstyle(texu_wnd* wnd)
{
  return wnd->exstyle;
}


texu_list_item*
_texu_wnd_find_keycmd(texu_wnd* wnd, texu_i32 key, texu_i32 alt)
{
  texu_wnd_keycmd *keycmd = 0;
  texu_list_item* item = texu_list_first(wnd->keycmds);

  while (item)
  {
    keycmd = (texu_wnd_keycmd*)item->data;
    if (keycmd->key == key && keycmd->alt == alt)
    {
      return item;
    }
    item = item->next;
  }
  return 0;
}

texu_wnd_keycmd*
texu_wnd_find_keycmd(texu_wnd* wnd, texu_i32 key, texu_i32 alt)
{
  texu_list_item* item = _texu_wnd_find_keycmd(wnd, key, alt);
  return (item ? (texu_wnd_keycmd*)item->data : 0);
}

texu_status
texu_wnd_add_keycmd(texu_wnd* wnd, texu_i32 key, texu_ui32 cmd, texu_i32 alt)
{
  texu_status rc = TEXU_OK;
  texu_wnd_keycmd *keycmd = (texu_wnd_keycmd*)malloc(sizeof(texu_wnd_keycmd));
  keycmd->key = key;
  keycmd->cmd = cmd;
  keycmd->alt = alt;
  texu_list_add(wnd->keycmds, (texu_i64)keycmd);
  return rc;
}


texu_status
texu_wnd_add_child(texu_wnd* wnd, texu_wnd* childwnd)
{
  texu_status rc = TEXU_OK;
  texu_wnd* findwnd = 0;

  if (!wnd || !childwnd)
  {
    return TEXU_ERROR;
  }
  findwnd = texu_wnd_find_child(wnd, childwnd->id);
  if (findwnd)
  {
    return TEXU_DUPLICATED_ID;
  }

  texu_list_add(wnd->children, (texu_i64)childwnd);
  return rc;
}

texu_status
texu_wnd_remove_child(texu_wnd* wnd, texu_wnd* childwnd)
{
  texu_status rc = TEXU_OK;
  texu_list_item* item = 0;

  if (!wnd || !childwnd)
  {
    return TEXU_ERROR;
  }
  item = _texu_wnd_find_child(wnd, childwnd->id);
  if (!item)
  {
    return TEXU_NOTFOUND;
  }

  texu_list_remove(wnd->children, item);
  return rc;
}

texu_wnd*
texu_wnd_find_child(texu_wnd* wnd, texu_ui32 id)
{
  texu_list_item* item = _texu_wnd_find_child(wnd, id);
  return (item ? (texu_wnd*)item->data : 0);
}

texu_list_item*
_texu_wnd_find_child(texu_wnd* wnd, texu_ui32 id)
{
  texu_wnd* childwnd = 0;
  texu_list_item* item = texu_list_first(wnd->children);

  while (item)
  {
    childwnd = (texu_wnd*)item->data;
    if (childwnd->id == id)
    {
      return item;
    }
    item = item->next;
  }
  return 0;
}

void
texu_wnd_set_text(texu_wnd* wnd, const texu_char* text)
{
  texu_wnd_send_msg(wnd, TEXU_WM_SETTEXT, (texu_i64)text, 0);
}

texu_i32
texu_wnd_get_text(texu_wnd* wnd, texu_char* text, texu_i32 len)
{
  return (texu_i32)texu_wnd_send_msg(wnd, TEXU_WM_GETTEXT, (texu_i64)text, len);
}

void
texu_wnd_set_color(texu_wnd* wnd, texu_i32 normalcolor, texu_i32 disabledcolor)
{
  wnd->normalcolor = normalcolor;
  wnd->disabledcolor = disabledcolor;
}

texu_status
texu_wnd_get_color(texu_wnd* wnd, texu_i32* normalcolor, texu_i32* disabledcolor)
{
  *normalcolor = wnd->normalcolor;
  *disabledcolor = wnd->disabledcolor;
  return TEXU_OK;
}

void
texu_wnd_set_focuscolor(texu_wnd* wnd, texu_i32 color)
{
  wnd->focuscolor = color;
}

texu_i32
texu_wnd_get_focuscolor(texu_wnd* wnd)
{
  return wnd->focuscolor;
}


const texu_char*
texu_wnd_get_clsname(texu_wnd* wnd)
{
  return wnd->clsname;
}


void*
texu_wnd_get_userdata(texu_wnd* wnd)
{
  return wnd->userdata;
}

void
texu_wnd_set_userdata(texu_wnd* wnd, void* userdata)
{
  wnd->userdata = userdata;
}

texu_env*
texu_wnd_get_env(texu_wnd* wnd)
{
  return wnd->env;
}

void
texu_wnd_get_rect(texu_wnd* wnd, texu_rect* rect)
{
  rect->y = wnd->y;
  rect->x = wnd->x;
  rect->lines = wnd->height;
  rect->cols  = wnd->width;
}

texu_cio*
texu_wnd_get_cio(texu_wnd* wnd)
{
  return wnd->env->cio;
}

void
texu_wnd_lock_update(texu_wnd* wnd, texu_bool locked)
{
  if (wnd->lockedupdate != locked)
  {
    if (!locked)
    {
      texu_wnd_invalidate(wnd);
    }
  }
  wnd->lockedupdate = locked;
}

texu_bool
texu_wnd_is_update_locked(texu_wnd* wnd)
{
  return wnd->lockedupdate;
}


texu_status
texu_wnd_save_curpos(texu_wnd* wnd)
{
  texu_i32 y = texu_wnd_get_y(wnd);
  texu_i32 x = texu_wnd_get_x(wnd);
  texu_i32 width = texu_wnd_get_width(wnd);
  return texu_env_save_curpos(wnd->env, y, x+width-1);
}

texu_status
texu_wnd_restore_curpos(texu_wnd* wnd)
{
  return texu_env_restore_curpos(wnd->env);
}

texu_menu*
texu_wnd_set_menu(texu_wnd* wnd, texu_menu* newmenu)
{
  texu_menu* oldmenu = wnd->menu;
  wnd->menu = newmenu;
  return oldmenu;
}



#ifdef __cplusplus
}
#endif


