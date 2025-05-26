/*
#
# File Name: texumenu.h
# Author: Seree Rakwong
# Date: 05-JUN-2020
#
*/

#ifndef _TEXUMENU_H_
#define _TEXUMENU_H_

#include <stdio.h>

#include "texumacros.h"
#include "texutypes.h"
#include "texuconst.h"
#include "texulib.h"
#include "texust.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
# TexU menu object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890


# Example menu
root (texu_menu*)
 |
 -- File (texu_popup_menu*)
 |   |
 |   -- New (texu_popup_menu_item*)
 |   -- Open (texu_popup_menu_item*)
 |   -- Save (texu_popup_menu_item*)
 |   -- (SEPARATOR) (texu_popup_menu_item*)
 |   -- Exit (texu_popup_menu_item*)
 -- Edit (texu_popup_menu*)
 |   |
 |   -- Undo (texu_popup_menu_item*)
 |   -- (SEPARATOR) (texu_popup_menu_item*)
 |   -- Cut (texu_popup_menu_item*)
 |   -- Copy (texu_popup_menu_item*)
 -- Help (texu_popup_menu*)
     |
     -- About (texu_popup_menu_item*)
*/

#define TEXU_MS_BREAK           0
#define TEXU_MS_TEXT            1
#define TEXU_MS_POPUP           2

typedef texu_tree_item    texu_popup_menu;
typedef texu_tree_item    texu_popup_menu_item;

struct _texu_popup_menu2
{
    texu_tree_item      hdr;
    texu_menu_item      item;
    void                *userdata;
};
typedef struct _texu_popup_menu2 texu_popup_menu2;

texu_menu*        texu_menu_new(texu_wnd*, texu_i32);
void              texu_menu_del(texu_menu*);

texu_wnd*           texu_menu_get_menubar(texu_menu*);
texu_popup_menu*    texu_menu_add_menu(
                        texu_menu*,
                        const texu_char* text,
                        texu_bool enable);
texu_popup_menu_item*   texu_menu_add_item(
                            texu_menu*,
                            texu_popup_menu*,
                            const texu_char* text,
                            texu_i32 id,
                            texu_bool enable);
texu_popup_menu*    texu_menu_add_menubar_info(
                        texu_menu*,
                        const texu_char* text,
                        texu_bool enable,
                        const texu_char* info);
texu_popup_menu*    texu_menu_add_menu_info(
                        texu_menu*,
                        const texu_char* text,
                        texu_bool enable,
                        const texu_char* info);
texu_popup_menu*   texu_menu_add_submenu_info(
                            texu_menu*, /*main menu*/
                            texu_popup_menu*, /*parent popup*/
                            const texu_char* text,
                            texu_i32 id,
                            texu_bool enable,
                            const texu_char* info);
texu_popup_menu_item*   texu_menu_add_item_info(
                            texu_menu*,
                            texu_popup_menu*,
                            const texu_char* text,
                            texu_i32 id,
                            texu_bool enable,
                            const texu_char* info);
texu_popup_menu_item*   texu_menu_add_baritem_info(
                            texu_menu*,
                            texu_popup_menu* baritem,
                            const texu_char* text,
                            texu_i32 id,
                            texu_bool enable,
                            const texu_char* info);
texu_popup_menu_item*   texu_menu_add_subitem_info(
                            texu_menu           *menu,
                            texu_popup_menu     *submenu,
                            texu_i32            style,
                            const texu_char     * text,
                            texu_i32            id,
                            texu_bool           enable,
                            const texu_char     * info);

texu_popup_menu*   texu_menu_find_item(texu_menu*, texu_i32);
texu_popup_menu*   texu_menu_get_menu(texu_menu*, texu_i32);
texu_popup_menu*   texu_menu_set_curmenu(texu_menu*, texu_popup_menu*);

texu_popup_menu_item*   texu_menu_get_menuitem(texu_menu*, texu_popup_menu*, texu_i32);
texu_popup_menu_item*   texu_menu_set_curmenuitem(texu_menu*, texu_popup_menu_item*);

texu_bool         texu_menu_enable_menu(texu_menu*, texu_i32, texu_bool);
texu_bool         texu_menu_enable_menuitem(texu_menu*, texu_popup_menu*, texu_i32, texu_bool);

texu_wnd            *texu_menu_track_menu_popup(texu_wnd *owner, texu_menu* menu, texu_i32 barid, texu_i32 y, texu_i32 x);

#define texu_menu_add_popup_menu        texu_menu_add_menu_info
#define texu_menu_add_popup_menu_item   texu_menu_add_item_info

#ifdef __cplusplus
}
#endif

#endif /* _TEXUMENU_H_ */
