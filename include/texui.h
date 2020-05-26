/*
#
# File Name: texui.h
# Author: Seree Rakwong
# Date: 13-MAY-2020
#
*/

#ifndef _TEXUI_H_
#define _TEXUI_H_

#include <stdio.h>

#include "texumacros.h"
#include "texutypes.h"
#include "texuconst.h"
#include "texucio.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
# TexU environment object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_env;
typedef struct texu_env texu_env;

struct texu_wnd;
typedef struct texu_wnd texu_wnd;

typedef texu_i64  (*texu_wndproc)(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          TexuDefWndProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);


texu_env*         texu_env_new();
void              texu_env_del(texu_env*);

texu_status       texu_env_register_cls(
                    texu_env*,
                    texu_char*,
                    texu_wndproc);
texu_status       texu_env_run(texu_env*);
texu_cio*         texu_env_get_cio(texu_env*);
texu_status       texu_env_push_wnd(texu_env*, texu_wnd*);
texu_wnd*         texu_env_pop_wnd(texu_env*);
texu_wnd*         texu_env_top_wnd(texu_env*);
texu_wnd*         texu_env_get_desktop(texu_env*);
void              texu_env_exit(texu_env*);

/*
# TexU window object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
/*
struct texu_wnd;
typedef struct texu_wnd texu_wnd;
*/


texu_wnd*          texu_wnd_new(texu_env*);
void               texu_wnd_del(texu_wnd*);

struct texu_wnd_attrs
{
  texu_i32         y;
  texu_i32         x;
  texu_i32         height;
  texu_i32         width;
  texu_ui32        style;
  texu_ui32        exstyle;
  texu_bool        enable;
  texu_bool        visible;
  texu_char*       text;
  texu_i32         normalcolor;
  texu_i32         disabledcolor;
  texu_i32         focuscolor;
  texu_ui32        id;
  texu_char*       clsname;
  void*            userdata;
};
typedef struct texu_wnd_attrs texu_wnd_attrs;

struct texu_wnd_notify
{
  texu_wnd*        wnd;
  texu_ui32        id;
  texu_ui32        code;
};
typedef struct texu_wnd_notify texu_wnd_notify;

struct texu_wnd_keycmd
{
  texu_i32        key;
  texu_ui32       cmd;
};
typedef struct texu_wnd_keycmd texu_wnd_keycmd;

struct texu_lbwnd_notify
{
  texu_wnd_notify  hdr;
  texu_i32         index;
};
typedef struct texu_lbwnd_notify texu_lbwnd_notify;




texu_status        texu_wnd_create(texu_wnd*, texu_wnd*, const texu_wnd_attrs*);
void               texu_wnd_destroy(texu_wnd*);

texu_status        texu_wnd_add_child(texu_wnd*, texu_wnd*);
texu_status        texu_wnd_remove_child(texu_wnd*, texu_wnd*);
texu_wnd*          texu_wnd_find_child(texu_wnd*, texu_ui32);
texu_wnd*          texu_wnd_is_window(texu_wnd*);

texu_status        texu_wnd_add_keycmd(texu_wnd*, texu_i32, texu_ui32);
texu_wnd_keycmd*   texu_wnd_find_keycmd(texu_wnd*, texu_i32);

texu_i64           texu_wnd_send_msg(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64           texu_wnd_invalidate(texu_wnd*);

texu_wnd*          texu_wnd_get_frame(texu_wnd*);
texu_ui32          texu_wnd_get_id(texu_wnd*);
texu_wnd*          texu_wnd_get_parent(texu_wnd*);
texu_wnd*          texu_wnd_get_activechild(texu_wnd*);
texu_wnd*          texu_wnd_get_first_activechild(texu_wnd*);
texu_wnd*          texu_wnd_get_last_activechild(texu_wnd*);
texu_wnd*          texu_wnd_get_next_activechild(texu_wnd*, texu_wnd*);
texu_wnd*          texu_wnd_get_prev_activechild(texu_wnd*, texu_wnd*);
texu_wnd*          texu_wnd_firstchild(texu_wnd*);
texu_wnd*          texu_wnd_lastchild(texu_wnd*);
texu_wnd*          texu_wnd_nextwnd(texu_wnd*);
texu_wnd*          texu_wnd_prevwnd(texu_wnd*);

texu_status        texu_wnd_visible(texu_wnd*, texu_bool);
texu_status        texu_wnd_enable(texu_wnd*, texu_bool);
texu_bool          texu_wnd_is_visible(texu_wnd*);
texu_bool          texu_wnd_is_enable(texu_wnd*);
void               texu_wnd_set_text(texu_wnd*, const texu_char*);
texu_i32           texu_wnd_get_text(texu_wnd*, texu_char*, texu_i32);
void               texu_wnd_set_color(texu_wnd*, texu_i32, texu_i32);
texu_status        texu_wnd_get_color(texu_wnd*, texu_i32*, texu_i32*);
const texu_char*   texu_wnd_get_clsname(texu_wnd*);
void               texu_wnd_set_focuscolor(texu_wnd*, texu_i32);
texu_i32           texu_wnd_get_focuscolor(texu_wnd*);


texu_i32           texu_wnd_get_y(texu_wnd*);
texu_i32           texu_wnd_get_x(texu_wnd*);
texu_i32           texu_wnd_get_width(texu_wnd*);
texu_i32           texu_wnd_get_height(texu_wnd*);
texu_ui32          texu_wnd_get_style(texu_wnd*);
texu_ui32          texu_wnd_get_exstyle(texu_wnd*);
void*              texu_wnd_get_userdata(texu_wnd*);
void               texu_wnd_set_userdata(texu_wnd*, void*);

texu_cio*          texu_wnd_get_cio(texu_wnd*);
void               texu_wnd_lock_update(texu_wnd*, texu_bool);
texu_bool          texu_wnd_is_update_locked(texu_wnd* wnd);

/* edit */
struct texu_editminmax
{
  texu_i32         min; 
  texu_i32         max; 
};
typedef struct texu_editminmax texu_editminmax;


#ifdef __cplusplus
}
#endif

#endif /* _TEXUI_H_ */


