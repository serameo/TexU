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
#include "texust.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
# TexU environment object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
enum
{
  TEXU_COLOR_WINDOW = 0,
  TEXU_COLOR_DESKTOP,
  TEXU_COLOR_DIALOG,
  TEXU_COLOR_TITLE_WINDOW,
  TEXU_COLOR_BUTTON_OK,
  TEXU_COLOR_BUTTON_YES,
  TEXU_COLOR_BUTTON_NO,
  TEXU_COLOR_BUTTON_CANCEL,
  TEXU_COLOR_MENU,
  TEXU_COLOR_MENU_DISABLED,
  TEXU_COLOR_MENU_SELECTED,
  TEXU_COLOR_MENUITEM,
  TEXU_COLOR_MENUITEM_DISABLED,
  TEXU_COLOR_MENUITEM_SELECTED,
  TEXU_COLOR_LABEL,
  TEXU_COLOR_LABEL_DISABLED,
  TEXU_COLOR_STATUSBAR,
  TEXU_COLOR_EDIT,
  TEXU_COLOR_EDIT_DISABLED,
  TEXU_COLOR_EDIT_SELECTED,
  TEXU_COLOR_LISTBOX,
  TEXU_COLOR_LISTBOX_DISABLED,
  TEXU_COLOR_LISTBOX_SELECTED,
  TEXU_COLOR_COMBOBOX,
  TEXU_COLOR_COMBOBOX_DISABLED,
  TEXU_COLOR_COMBOBOX_SELECTED,
  TEXU_COLOR_LISTCTRL,
  TEXU_COLOR_LISTCTRL_DISABLED,
  TEXU_COLOR_LISTCTRL_SELECTED,
  TEXU_COLOR_LISTCTRL_ITEM,
  TEXU_COLOR_LISTCTRL_ITEM_DISABLED,
  TEXU_COLOR_LISTCTRL_ITEM_SELECTED,
  TEXU_COLOR_TREECTRL,
  TEXU_COLOR_TREECTRL_DISABLED,
  TEXU_COLOR_TREECTRL_SELECTED,
  TEXU_COLOR_TREECTRL_ITEM,
  TEXU_COLOR_TREECTRL_ITEM_DISABLED,
  TEXU_COLOR_TREECTRL_ITEM_SELECTED,
  TEXU_COLOR_UPDOWNCTRL,
  TEXU_COLOR_UPDOWNCTRL_DISABLED,
  TEXU_COLOR_UPDOWNCTRL_SELECTED,
  TEXU_COLOR_PROGRESSBAR,
  TEXU_COLOR_PROGRESSBAR_DISABLED,
  TEXU_COLOR_PROGRESSBAR_SELECTED,
  TEXU_COLOR_REBAR,
  TEXU_COLOR_REBAR_DISABLED,
  TEXU_COLOR_REBAR_SELECTED,
  TEXU_COLOR_BUTTON,
  TEXU_COLOR_BUTTON_DISABLED,
  TEXU_COLOR_DEFAULT
};

typedef texu_i64  (*texu_wndproc)(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          TexuDefWndProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64          TexuFrameWndProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);


#ifdef USE_TCL_AUTOMATION
texu_env*         texu_env_new(texu_i32 lines, texu_i32 cols, const char* pathname);
#else
texu_env*         texu_env_new(texu_i32, texu_i32);
#endif /*USE_TCL_AUTOMATION*/
void              texu_env_del(texu_env*);

texu_status       texu_env_register_cls(
                    texu_env*,
                    const texu_char*,
                    texu_wndproc);
texu_status       texu_env_run(texu_env*);
texu_cio*         texu_env_get_cio(texu_env*);
texu_status       texu_env_push_wnd(texu_env*, texu_wnd*);
texu_wnd*         texu_env_pop_wnd(texu_env*);
texu_wnd*         texu_env_top_wnd(texu_env*);
texu_wnd*         texu_env_get_desktop(texu_env*);
void              texu_env_exit(texu_env*);
texu_i32          texu_env_screen_height(texu_env*);
texu_i32          texu_env_screen_width(texu_env*);

texu_status       texu_env_save_curpos(texu_env* env, texu_i32 ypos, texu_i32 xpos);
texu_status       texu_env_restore_curpos(texu_env*);

void              texu_env_save_screen(texu_env* env);
void              texu_env_restore_screen(texu_env* env);

void              texu_env_set_syscolors(texu_env* env, texu_i32 (*setcolor)(texu_i32));
texu_i32          texu_env_get_syscolor(texu_env*, texu_i32);

texu_i32          texu_env_set_timer(texu_env*, texu_i32, texu_i32, texu_i32);

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


texu_wnd*         texu_wnd_new(texu_env*);
void              texu_wnd_del(texu_wnd*);


texu_status       texu_wnd_create(texu_wnd*, texu_wnd*, const texu_wnd_attrs*);
void              texu_wnd_destroy(texu_wnd*);

texu_status       texu_wnd_add_child(texu_wnd*, texu_wnd*);
texu_status       texu_wnd_remove_child(texu_wnd*, texu_wnd*);
texu_wnd*         texu_wnd_find_child(texu_wnd*, texu_ui32);
texu_wnd*         texu_wnd_is_window(texu_wnd*);

texu_status       texu_wnd_add_keycmd(texu_wnd*, texu_i32, texu_ui32, texu_i32);
texu_wnd_keycmd*  texu_wnd_find_keycmd(texu_wnd*, texu_i32, texu_i32);

texu_i64          texu_wnd_send_msg(texu_wnd*, texu_ui32, texu_i64, texu_i64);  /*synchronous*/
texu_i64          texu_wnd_invalidate(texu_wnd*);
texu_i64          texu_wnd_post_msg(texu_wnd*, texu_ui32, texu_i64, texu_i64);

texu_wnd*         texu_wnd_get_frame(texu_wnd*);
texu_ui32         texu_wnd_get_id(texu_wnd*);
texu_wnd*         texu_wnd_get_parent(texu_wnd*);
texu_wnd*         texu_wnd_get_activechild(texu_wnd*);
texu_wnd*         texu_wnd_set_activechild(texu_wnd* wnd, texu_wnd* childwnd);
texu_wnd*         texu_wnd_get_first_activechild(texu_wnd*);
texu_wnd*         texu_wnd_get_last_activechild(texu_wnd*);
texu_wnd*         texu_wnd_get_next_activechild(texu_wnd*, texu_wnd*);
texu_wnd*         texu_wnd_get_prev_activechild(texu_wnd*, texu_wnd*);
texu_wnd*         texu_wnd_firstchild(texu_wnd*);
texu_wnd*         texu_wnd_lastchild(texu_wnd*);
texu_wnd*         texu_wnd_nextwnd(texu_wnd*);
texu_wnd*         texu_wnd_prevwnd(texu_wnd*);
texu_i32          texu_wnd_children(texu_wnd*);

texu_status       texu_wnd_visible(texu_wnd*, texu_bool);
texu_status       texu_wnd_enable(texu_wnd*, texu_bool);
texu_bool         texu_wnd_is_visible(texu_wnd*);
texu_bool         texu_wnd_is_enable(texu_wnd*);
texu_bool         texu_wnd_is_active(texu_wnd *wnd);
void              texu_wnd_set_text(texu_wnd*, const texu_char*);
texu_i32          texu_wnd_get_text(texu_wnd*, texu_char*, texu_i32);
void              texu_wnd_set_color(texu_wnd*, texu_i32, texu_i32);
texu_status       texu_wnd_get_color(texu_wnd*, texu_i32*, texu_i32*);
const texu_char*  texu_wnd_get_clsname(texu_wnd*);
void              texu_wnd_set_focuscolor(texu_wnd*, texu_i32);
texu_i32          texu_wnd_get_focuscolor(texu_wnd*);


texu_i32          texu_wnd_get_y(texu_wnd*);
texu_i32          texu_wnd_get_x(texu_wnd*);
texu_i32          texu_wnd_get_width(texu_wnd*);
texu_i32          texu_wnd_get_height(texu_wnd*);
texu_ui32         texu_wnd_get_style(texu_wnd*);
void              texu_wnd_set_style(texu_wnd*, texu_ui32);
texu_ui32         texu_wnd_get_exstyle(texu_wnd*);
void              texu_wnd_set_exstyle(texu_wnd*, texu_ui32);
void*             texu_wnd_get_userdata(texu_wnd*);
void              texu_wnd_set_userdata(texu_wnd*, void*);
void              texu_wnd_move(texu_wnd*, texu_i32, texu_i32, texu_i32, texu_i32, texu_bool);

texu_cio*         texu_wnd_get_cio(texu_wnd*);
void              texu_wnd_lock_update(texu_wnd*, texu_bool);
texu_bool         texu_wnd_is_update_locked(texu_wnd* wnd);
texu_env*         texu_wnd_get_env(texu_wnd*);
void              texu_wnd_get_rect(texu_wnd*, texu_rect*);

texu_status       texu_wnd_save_curpos(texu_wnd*);
texu_status       texu_wnd_restore_curpos(texu_wnd*);

texu_menu*        texu_wnd_set_menu(texu_wnd*, texu_menu*);
texu_menu*        texu_wnd_get_menu(texu_wnd*);







/*tree window*/
/*typedef texu_i32  (*texu_tree_find_proc)(const void*, const void*);*/
typedef texu_i32  (*texu_tree_exp_proc)(FILE*, texu_treewnd_item*);
typedef texu_i32  (*texu_tree_imp_proc)(texu_treewnd_item*, const texu_char*);

typedef texu_i32  (*texu_tree_find_proc)(texu_i64, texu_i64, void*);


#ifdef __cplusplus
}
#endif

#endif /* _TEXUI_H_ */
