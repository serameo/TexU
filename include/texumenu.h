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
#include "texucio.h"
#include "texust.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
# TexU menu object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

#define TEXU_MS_TEXT          1
#define TEXU_MS_BREAK         0


texu_menu*        texu_menu_new(texu_wnd*, texu_ui32);
void              texu_menu_del(texu_menu*);

texu_wnd*         texu_menu_get_menubar(texu_menu*);
texu_tree_item*   texu_menu_add_menu(texu_menu*, const texu_char* text);
texu_tree_item*   texu_menu_add_item(texu_menu*, texu_tree_item*, const texu_char* text, texu_ui32 id);


texu_tree_item*   texu_menu_find_item(texu_menu*, texu_ui32);
texu_menu_item*   texu_menu_get_menu(texu_menu*, texu_ui32);
texu_menu_item*   texu_menu_get_menuitem(texu_menu*, texu_tree_item*, texu_ui32);
void              texu_menu_enable_item(texu_menu*, texu_menu_item*, texu_bool);



#ifdef __cplusplus
}
#endif

#endif /* _TEXUMENU_H_ */
