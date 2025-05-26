/*
#
# File Name: texul.h
# Author: Seree Rakwong
# Date: 04-MAR-2025
#
*/

#ifndef __TEXUL_H__
#define __TEXUL_H__

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
# TexU Layer
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_layer *texu_layer_init(texu_wnd *owner);
void texu_layer_release(struct texu_layer *layer);
texu_i32 texu_layer_add_child(struct texu_layer *layer, texu_wnd *child);
void texu_layer_show(struct texu_layer *layer, texu_bool fShow);
void texu_layer_move(struct texu_layer *layer, const texu_urect *area);
void texu_layer_center(struct texu_layer *layer, const texu_urect *area);
void texu_layer_vcenter(struct texu_layer *layer, const texu_urect *area);


struct texu_layer_mgr *texu_layer_mgr_init(texu_wnd *owner);
void texu_layer_mgr_release(struct texu_layer_mgr *mgr);
texu_i32 texu_layer_mgr_add_layer(struct texu_layer_mgr* mgr, struct texu_layer* layer);
void texu_layer_mgr_show(struct texu_layer_mgr* mgr, texu_i32 id);

#ifdef __cplusplus
}
#endif

#endif /*#define __TEXUL_H__*/
