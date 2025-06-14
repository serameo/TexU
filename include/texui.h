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

#if (defined VMS || defined __VMS__)
#include <descrip.h>
#include <descrip.h>
#include <ssdef.h>
#include <smgdef.h>
#include <smg$routines.h>
#include <stsdef.h>
#include <starlet.h>
#include <lib$routines.h>
#include <libdtdef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*
# TexU environment object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#if defined SIZE_132X25
#ifndef COLS
#define COLS    (100)
#endif
#ifndef LINES
#if defined __USE_TERMIOS__
#define LINES   (24)
#else
#define LINES   (25)
#endif
#endif
#else
#ifndef COLS
#define COLS    (80)
#endif
#ifndef LINES
#if defined __USE_TERMIOS__
#define LINES   (24)
#else
#define LINES   (25)
#endif
#endif
#endif
enum
{
    TEXU_COLOR_WINDOW = 0,
    TEXU_COLOR_DESKTOP,
    TEXU_COLOR_DIALOG,
    TEXU_COLOR_TITLE_WINDOW,
    TEXU_COLOR_BORDER,
    TEXU_COLOR_BUTTON_OK,
    TEXU_COLOR_BUTTON_YES,
    TEXU_COLOR_BUTTON_NO,
    TEXU_COLOR_BUTTON_CANCEL,
    TEXU_COLOR_BUTTON_DEFAULT,
    TEXU_COLOR_MENU,
    TEXU_COLOR_MENU_DISABLED,
    TEXU_COLOR_MENU_SELECTED,
    TEXU_COLOR_MENU_FOCUSED,
    TEXU_COLOR_MENUITEM,
    TEXU_COLOR_MENUITEM_DISABLED,
    TEXU_COLOR_MENUITEM_SELECTED,
    TEXU_COLOR_MENUITEM_FOCUSED,
    TEXU_COLOR_LABEL,
    TEXU_COLOR_LABEL_DISABLED,
    TEXU_COLOR_STATUSBAR,
    TEXU_COLOR_EDIT,
    TEXU_COLOR_EDIT_DISABLED,
    TEXU_COLOR_EDIT_SELECTED,
    TEXU_COLOR_EDIT_FOCUSED,
    TEXU_COLOR_EDIT_INVALID,
    TEXU_COLOR_LISTBOX,
    TEXU_COLOR_LISTBOX_DISABLED,
    TEXU_COLOR_LISTBOX_SELECTED,
    TEXU_COLOR_LISTBOX_FOCUSED,
    TEXU_COLOR_COMBOBOX,
    TEXU_COLOR_COMBOBOX_DISABLED,
    TEXU_COLOR_COMBOBOX_SELECTED,
    TEXU_COLOR_COMBOBOX_FOCUSED,
    TEXU_COLOR_LISTCTRL,
    TEXU_COLOR_LISTCTRL_DISABLED,
    TEXU_COLOR_LISTCTRL_SELECTED,
    TEXU_COLOR_LISTCTRL_FOCUSED,
    TEXU_COLOR_LISTCTRL_ITEM,
    TEXU_COLOR_LISTCTRL_ITEM_DISABLED,
    TEXU_COLOR_LISTCTRL_ITEM_SELECTED,
    TEXU_COLOR_LISTCTRL_ITEM_FOCUSED,
    TEXU_COLOR_LISTCTRL_ITEM_SELFOCUSED,
    TEXU_COLOR_TREECTRL,
    TEXU_COLOR_TREECTRL_DISABLED,
    TEXU_COLOR_TREECTRL_SELECTED,
    TEXU_COLOR_TREECTRL_FOCUSED,
    TEXU_COLOR_TREECTRL_ITEM,
    TEXU_COLOR_TREECTRL_ITEM_DISABLED,
    TEXU_COLOR_TREECTRL_ITEM_SELECTED,
    TEXU_COLOR_TREECTRL_ITEM_FOCUSED,
    TEXU_COLOR_UPDOWNCTRL,
    TEXU_COLOR_UPDOWNCTRL_DISABLED,
    TEXU_COLOR_UPDOWNCTRL_SELECTED,
    TEXU_COLOR_UPDOWNCTRL_FOCUSED,
    TEXU_COLOR_UPDOWNCTRL_MINUS,
    TEXU_COLOR_UPDOWNCTRL_PLUS,
    TEXU_COLOR_PROGRESSBAR,
    TEXU_COLOR_PROGRESSBAR_DISABLED,
    TEXU_COLOR_PROGRESSBAR_SELECTED,
    TEXU_COLOR_PROGRESSBAR_FOCUSED,
    TEXU_COLOR_REBAR,
    TEXU_COLOR_REBAR_DISABLED,
    TEXU_COLOR_REBAR_SELECTED,
    TEXU_COLOR_REBAR_FOCUSED,
    TEXU_COLOR_BUTTON,
    TEXU_COLOR_BUTTON_DISABLED,
    TEXU_COLOR_BUTTON_SELECTED,
    TEXU_COLOR_BUTTON_FOCUSED,
    TEXU_COLOR_IPADDRESSCTRL,
    TEXU_COLOR_IPADDRESSCTRL_DISABLED,
    TEXU_COLOR_IPADDRESSCTRL_SELECTED,
    TEXU_COLOR_IPADDRESSCTRL_FOCUSED,
    TEXU_COLOR_EDITMASKCTRL,
    TEXU_COLOR_EDITMASKCTRL_DISABLED,
    TEXU_COLOR_EDITMASKCTRL_SELECTED,
    TEXU_COLOR_EDITMASKCTRL_FOCUSED,
    TEXU_COLOR_EDITPRICESPREADCTRL,
    TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED,
    TEXU_COLOR_EDITPRICESPREADCTRL_SELECTED,
    TEXU_COLOR_EDITPRICESPREADCTRL_FOCUSED,
    TEXU_COLOR_EDITPRICESPREADCTRL_LOWER,
    TEXU_COLOR_EDITPRICESPREADCTRL_EQUAL,
    TEXU_COLOR_EDITPRICESPREADCTRL_GREATER,
    TEXU_COLOR_PANEL,
    TEXU_COLOR_PANEL_DISABLED,
    TEXU_COLOR_PANEL_BORDER,
    TEXU_COLOR_PANEL_TITLE,
    TEXU_COLOR_EDITVOLUMECTRL,
    TEXU_COLOR_EDITVOLUMECTRL_DISABLED,
    TEXU_COLOR_EDITVOLUMECTRL_SELECTED,
    TEXU_COLOR_EDITVOLUMECTRL_FOCUSED,
    TEXU_COLOR_DEFAULT
};

typedef texu_longptr    (*texu_wndproc)(texu_wnd*, texu_ui32, texu_lparam, texu_lparam);
texu_longptr            TexuDefWndProc(texu_wnd*, texu_ui32, texu_lparam, texu_lparam);
texu_longptr            TexuFrameWndProc(texu_wnd*, texu_ui32, texu_lparam, texu_lparam);
texu_longptr            TexuManualFrameWndProc(texu_wnd*, texu_ui32, texu_lparam, texu_lparam);


#ifdef USE_TCL_AUTOMATION
texu_env*       texu_env_new(texu_i32 lines, texu_i32 cols, const char* pathname);
FILE*           texu_env_get_stdout(texu_env *env);
#else
#if (defined WIN32 && defined _WINDOWS)
#include "cjson.h"
HFONT           texu_env_get_clsfont(texu_env *env, const texu_char *clsname);
HFONT           texu_env_get_insfont(texu_env *env, const texu_char *clsname, texu_ui32 id);
cJSON*          texu_env_get_prop(texu_env *env);
texu_i32        texu_env_invalidate(texu_env *env);
HDC             texu_env_get_hdc(texu_env *env);
HFONT           texu_env_get_hfont(texu_env *env);
void            texu_env_set_memdc(texu_env *env, HDC hmemdc);
HDC             texu_env_get_memdc(texu_env *env);
texu_env*       texu_env_new(HINSTANCE hinst, texu_i32 lines, texu_i32 cols);
texu_status     texu_env_register_envcls(texu_env *env, HINSTANCE hinst, texu_i32 lines, texu_i32 cols);
texu_status     texu_env_easy_createwnd(texu_env *env,
                                   HWND hWndParent,
                                   UINT nID,
                                   LPVOID lpData);
texu_status     texu_env_create_mainwnd(texu_env *env,
                                   DWORD dwExStyle,
                                   DWORD dwStyle,
                                   int x, int y, int cx, int cy,
                                   LPVOID lpData);
texu_status     texu_env_create_childwnd(texu_env *env,
                                   DWORD dwExStyle,
                                   DWORD dwStyle,
                                   int x, int y, int cx, int cy,
                                   HWND hWndParent,
                                   UINT nID,
                                   LPVOID lpData);
HWND            texu_env_get_hwnd(texu_env *env);
texu_i32        texu_env_get_cxcaps(texu_env *env);
texu_i32        texu_env_get_cyline(texu_env *env);
texu_status     texu_env_gotoyx(texu_env *env, texu_i32 y, texu_i32 x);
texu_status     texu_env_text_to_screen(texu_env *env, texu_pos* spos, const texu_pos* tpos);
texu_status     texu_env_screen_to_text(texu_env *env, texu_pos* tpos, const texu_pos* spos);
texu_wnd*       texu_env_set_focus_from_pos(texu_env *env, const texu_pos *pos);
texu_wnd*       texu_get_wnd_from_pos(texu_env *env, const texu_pos *pos);
texu_bool       texu_pos_in_rect(const texu_rect* rect, const texu_pos *pos);
texu_status     texu_env_draw_text(texu_env *env,
                                   texu_i32 y,
                                   texu_i32 x,
                                   const texu_char * text,
                                   texu_ui32 textcolor,
                                   texu_ui32 bgcolor);
texu_status     texu_env_draw_char(texu_env *env,
                                   texu_i32 y,
                                   texu_i32 x,
                                   texu_char ch,
                                   texu_ui32 textcolor,
                                   texu_ui32 bgcolor); 
texu_status     texu_env_draw_text_ex(texu_env *env,
                                   texu_i32 y,
                                   texu_i32 x,
                                   const texu_char * text,
                                   texu_ui32 textcolor,
                                   texu_ui32 bgcolor,
                                   const texu_char *clsname,
                                   texu_ui32    id);
texu_status     texu_env_draw_char_ex(texu_env *env,
                                   texu_i32 y,
                                   texu_i32 x,
                                   texu_char ch,
                                   texu_ui32 textcolor,
                                   texu_ui32 bgcolor,
                                   const texu_char *clsname,
                                   texu_ui32    id);


texu_status     texu_env_update_screen(texu_env *env);
texu_status     texu_env_draw_vframes(texu_env *env, texu_rect* rect, texu_i32* widths, texu_i32 frames, texu_ui32 textcolor, texu_ui32 bgcolor);
texu_status     texu_env_draw_hframes(texu_env *env, texu_rect* rect, texu_i32* heights, texu_i32 frames, texu_ui32 textcolor, texu_ui32 bgcolor);
texu_status     texu_env_draw_frame(texu_env *env, texu_rect* rect, texu_ui32 textcolor, texu_ui32 bgcolor);
texu_status     texu_env_draw_hline(texu_env *env, texu_i32 y, texu_i32 x, texu_i32 blocks, texu_ui32 textcolor, texu_ui32 bgcolor);
texu_status     texu_env_draw_vline(texu_env *env, texu_i32 y, texu_i32 x, texu_i32 blocks, texu_ui32 textcolor, texu_ui32 bgcolor);
texu_status     texu_env_draw_frame_char(texu_env *env, texu_i32 y, texu_i32 x, texu_i32 ch, texu_ui32 textcolor, texu_ui32 bgcolor);
texu_status     texu_env_draw_rect(texu_env *env, texu_rect* rect, texu_ui32 textcolor, texu_ui32 bgcolor);
/*cursor*/
texu_bool       texu_env_new_cursor(texu_env *env);
texu_bool       texu_env_set_cursor(texu_env *env, texu_i32 y, texu_i32 x);
texu_bool       texu_env_show_cursor(texu_env *env, texu_bool show);
texu_bool       texu_env_del_cursor(texu_env *env);

#else
texu_env*       texu_env_new(texu_i32, texu_i32);
#endif


/*cursor*/
texu_bool       texu_env_new_cursor(texu_env *env);
texu_bool       texu_env_set_cursor(texu_env *env, texu_i32 y, texu_i32 x);
texu_bool       texu_env_show_cursor(texu_env *env, texu_bool show);
texu_bool       texu_env_del_cursor(texu_env *env);


#endif /*USE_TCL_AUTOMATION*/
void            texu_env_del(texu_env*);
texu_i32        texu_env_get_wnd_id(texu_env* env);
texu_status     texu_env_register_cls(
                    texu_env*,
                    const texu_char*,
                    texu_wndproc);
texu_i32        texu_env_get_state(texu_env* env);
#if (defined WIN32 && defined _WINDOWS)
texu_status     texu_env_run(texu_env *env, UINT message, WPARAM wParam, LPARAM lParam);
#else
/*
options:
    0 = PEEK EVENT
    otherwise = POLL EVENT
*/
#define TEXU_ENV_PEEK_EVENT         0
#define TEXU_ENV_POLL_EVENT         1

texu_status     texu_env_run_base(texu_env*, texu_i32 options);
texu_status     texu_env_run(texu_env*); /*peek event*/
texu_status     texu_env_run_poll(texu_env*); /*poll event*/
#endif
texu_cio*       texu_env_get_cio(texu_env*);
texu_status     texu_env_push_wnd(texu_env*, texu_wnd*);
texu_wnd*       texu_env_pop_wnd(texu_env*);
texu_wnd*       texu_env_top_wnd(texu_env*);
texu_wnd*       texu_env_get_desktop(texu_env*);
void            texu_env_exit(texu_env*);
texu_i32        texu_env_screen_height(texu_env*);
texu_i32        texu_env_screen_width(texu_env*);
texu_wnd*       texu_env_get_mainwnd(texu_env *);

texu_status     texu_env_save_curpos(texu_env* env, texu_i32 ypos, texu_i32 xpos);
texu_status     texu_env_restore_curpos(texu_env*);

void            texu_env_save_screen(texu_env* env);
void            texu_env_restore_screen(texu_env* env);

void            texu_env_set_syscolors(texu_env* env, texu_i32 (*setcolor)(texu_i32));
texu_ui32       texu_env_get_syscolor(texu_env*, texu_i32);
void            texu_env_set_sysbgcolors(texu_env* env, texu_i32(*setcolor)(texu_i32));
texu_ui32       texu_env_get_sysbgcolor(texu_env*, texu_i32);
texu_wnd*       texu_env_get_focus(texu_env *env);
void            texu_env_set_focus(texu_env *env, texu_wnd *wnd);
void            texu_env_set_movenext(texu_env *env, texu_i32 nextkey);
texu_i32        texu_env_get_movenext(texu_env *env);
void            texu_env_set_moveprev(texu_env *env, texu_i32 prevkey);
texu_i32        texu_env_get_moveprev(texu_env *env);

texu_i32        texu_env_get_state(texu_env* env);
void            texu_env_set_state(texu_env* env, texu_i32 newstate);

#if 0
texu_i32        texu_env_set_timer(texu_env*, texu_i32, texu_i32, texu_i32);
#endif
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


texu_wnd*           texu_wnd_new(texu_env*);
void                texu_wnd_del(texu_wnd*);

texu_status         texu_wnd_create(texu_wnd*, texu_wnd*, const texu_wnd_attrs*);
void                texu_wnd_destroy(texu_wnd*);
texu_longptr        texu_wnd_close(texu_wnd *wnd);
texu_status         texu_wnd_add_child(texu_wnd*, texu_wnd*);
texu_status         texu_wnd_remove_child(texu_wnd*, texu_wnd*);
texu_wnd*           texu_wnd_find_child(texu_wnd*, texu_ui32);
texu_wnd*           texu_wnd_is_window(texu_wnd*);
texu_status         texu_wnd_create_controls(texu_wnd *wnd, const texu_wnd_template3 *templ, texu_i32 nitems);

texu_status         texu_wnd_add_keycmd(texu_wnd*, texu_i32, texu_ui32, texu_i32);
texu_wnd_keycmd*    texu_wnd_find_keycmd(texu_wnd*, texu_i32, texu_i32);

texu_longptr        texu_wnd_send_msg(texu_wnd*, texu_ui32, texu_lparam, texu_lparam);  /*synchronous*/
texu_i32            texu_wnd_invalidate(texu_wnd*);
texu_longptr        texu_wnd_post_msg(texu_wnd*, texu_ui32, texu_lparam, texu_lparam);

texu_wnd*           texu_wnd_get_frame(texu_wnd*);
texu_i32            texu_wnd_get_id(texu_wnd*);
texu_wnd*           texu_wnd_get_parent(texu_wnd*);
texu_wnd*           texu_wnd_get_activechild(texu_wnd*);
texu_wnd*           texu_wnd_set_activechild(texu_wnd* wnd, texu_wnd* childwnd);
texu_wnd*           texu_wnd_get_first_activechild(texu_wnd*);
texu_wnd*           texu_wnd_get_last_activechild(texu_wnd*);
texu_wnd*           texu_wnd_get_next_activechild(texu_wnd*, texu_wnd*);
texu_wnd*           texu_wnd_get_prev_activechild(texu_wnd*, texu_wnd*);
texu_wnd*           texu_wnd_firstchild(texu_wnd*);
texu_wnd*           texu_wnd_lastchild(texu_wnd*);
texu_wnd*           texu_wnd_nextwnd(texu_wnd*);
texu_wnd*           texu_wnd_prevwnd(texu_wnd*);
texu_i32            texu_wnd_children(texu_wnd*);

void                texu_wnd_set_focus(texu_wnd *wnd);
void                texu_wnd_kill_focus(texu_wnd *wnd);
texu_status         texu_wnd_visible(texu_wnd*, texu_bool);
texu_status         texu_wnd_enable(texu_wnd*, texu_bool);
texu_bool           texu_wnd_is_parent_enable(texu_wnd *wnd);
texu_bool           texu_wnd_is_visible(texu_wnd*);
texu_bool           texu_wnd_is_parent_visible(texu_wnd *wnd);
texu_bool           texu_wnd_is_enable(texu_wnd*);
texu_bool           texu_wnd_is_active(texu_wnd *wnd);
void                texu_wnd_set_text(texu_wnd*, const texu_char*);
texu_i32            texu_wnd_get_text(texu_wnd*, texu_char*, texu_i32);
void                texu_wnd_set_color(texu_wnd*, texu_ui32, texu_ui32);
texu_status         texu_wnd_get_color(texu_wnd*, texu_ui32*, texu_ui32*);
const texu_char*    texu_wnd_get_clsname(texu_wnd*);
texu_bool           texu_wnd_is_executable(texu_wnd *wnd, texu_ui32 id);
void                texu_wnd_set_focused_color(texu_wnd*, texu_ui32);
texu_ui32           texu_wnd_get_focused_color(texu_wnd*);
void                texu_wnd_set_selected_color(texu_wnd*, texu_ui32);
texu_ui32           texu_wnd_get_selected_color(texu_wnd*);

void                texu_wnd_set_bgcolor(texu_wnd*, texu_ui32, texu_ui32);
texu_status         texu_wnd_get_bgcolor(texu_wnd*, texu_ui32*, texu_ui32*);
void                texu_wnd_set_bgfocused_color(texu_wnd*, texu_ui32);
texu_ui32           texu_wnd_get_bgfocused_color(texu_wnd*);
void                texu_wnd_set_bgselected_color(texu_wnd*, texu_ui32);
texu_ui32           texu_wnd_get_bgselected_color(texu_wnd*);


texu_i32            texu_wnd_get_y(texu_wnd*);
texu_i32            texu_wnd_get_x(texu_wnd*);
texu_i32            texu_wnd_get_width(texu_wnd*);
texu_i32            texu_wnd_get_height(texu_wnd*);
texu_ui32           texu_wnd_get_style(texu_wnd*);
void                texu_wnd_set_style(texu_wnd*, texu_ui32);
texu_ui32           texu_wnd_get_exstyle(texu_wnd*);
void                texu_wnd_set_exstyle(texu_wnd*, texu_ui32);
void*               texu_wnd_get_userdata(texu_wnd*);
void                texu_wnd_set_userdata(texu_wnd*, void*);
void                texu_wnd_move(texu_wnd*, texu_i32, texu_i32, texu_i32, texu_i32, texu_bool);
void                texu_wnd_move_center(texu_wnd *wnd, texu_bool redraw);
void                texu_wnd_align_center(texu_wnd *wnd, texu_bool redraw);
void                texu_wnd_align_vcenter(texu_wnd *wnd, texu_bool redraw);

texu_cio*           texu_wnd_get_cio(texu_wnd*);
void                texu_wnd_lock_update(texu_wnd*, texu_bool);
texu_bool           texu_wnd_is_update_locked(texu_wnd* wnd);
texu_env*           texu_wnd_get_env(texu_wnd*);
void                texu_wnd_get_rect(texu_wnd*, texu_rect*);
void                texu_wnd_get_screen_rect(texu_wnd *wnd, texu_urect *rect);
void                texu_wnd_get_window_rect(texu_wnd *wnd, texu_urect *rect);
texu_wndproc        texu_wnd_get_wndproc(texu_wnd *wnd);
texu_wndproc        texu_wnd_subclass(texu_wnd *wnd, texu_wndproc new_wndproc);
texu_rect           texu_wnd_get_clipped(texu_wnd* wnd);

texu_status         texu_wnd_save_curpos(texu_wnd*);
texu_status         texu_wnd_restore_curpos(texu_wnd*);

texu_menu*          texu_wnd_set_menu(texu_wnd*, texu_menu*);
texu_menu*          texu_wnd_get_menu(texu_wnd*);

texu_bool texu_wnd_inbound(texu_wnd* parent, texu_wnd* child);
texu_bool texu_wnd_clip_bound(texu_wnd* parent, texu_wnd* child);
#if 0
void        texu__wnd_set_blank_lines_color(texu_wnd* parent, texu_i32 color, texu_i32 bgcolor);
void        texu__wnd_set_blank_line_color(texu_wnd* parent, texu_i32 id, texu_i32 color, texu_i32 bgcolor);
texu_i32    texu__wnd_create_blank_lines(texu_env *env, texu_wnd* parent);
#endif
texu_i32    texu_wnd_can_paint(texu_wnd* wnd);

texu_wnd *
texu_wnd_msgbox(
    const texu_char *caption,
    const texu_char *text,
    texu_wnd *owner,
    texu_ui32 id,
    texu_ui32 buttons,
    void *userdata);

texu_wnd *
texu_wnd_msgbox2(
    const texu_char *caption,
    const texu_char *text,
    texu_wnd *owner,
    texu_ui32 id,
    texu_ui32 buttons,
    void *userdata,
    texu_msgbox_attrs *mbxattrs);



/*tree window*/
/*typedef texu_i32  (*texu_tree_find_proc)(const void*, const void*);*/
typedef texu_i32  (*texu_tree_exp_proc)(FILE*, texu_treewnd_item*);
typedef texu_i32  (*texu_tree_imp_proc)(texu_treewnd_item*, const texu_char*);

typedef texu_i32  (*texu_tree_find_proc)(texu_lparam, texu_lparam, void*);


/*rectangle*/
texu_bool texu_rect_is_empty(const texu_rect* rect);
texu_bool texu_rect_is_equal(const texu_rect* rc1, const texu_rect* rc2);


#ifdef __cplusplus
}
#endif

#endif /* _TEXUI_H_ */
