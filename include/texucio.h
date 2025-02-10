/*
#
# File Name: texuio.h
# Author: Seree Rakwong
# Date: 13-MAY-2020
# Purpose: get console input/output from/to the screen
#
*/

#ifndef _TEXUIO_H_
#define _TEXUIO_H_

#include <stdio.h>
#ifdef __USE_CURSES__
#include <curses.h>
#endif

#ifdef __VMS__
#include <ssdef.h>
#include <starlet.h>
#include <stddef.h>
#include <iodef.h>
#include <descrip.h>
#include <stsdef.h>
#include <lib$routines.h>
#include <smgdef.h>
#include <smg$routines.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VMS_ROWS                25
#define VMS_COLUMNS             80

struct _iosb_term
{
  unsigned  short   status;    /* Return status */
  unsigned  short   offset;    /* Return */
  unsigned  short   terminator;/* Return terminate key ascii code */
  unsigned  short   size;      /* No use */
};
typedef struct _iosb_term iosb_t;

long vms_get_sysio(unsigned long*  iochan, char* sysio);
int vms_getint (unsigned long* kbid);
int vms_getch  (unsigned long* kbid);
unsigned long vms_writeio(unsigned long  wchannel, iosb_t* iosb, const char* text, unsigned long len);

#ifdef __cplusplus
}
#endif

#endif /* __VMS__ */

#include "texumacros.h"
#include "texutypes.h"
#include "texuconst.h"
#include "texust.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
# TexU io object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

texu_env            *texu_cio_get_env(texu_cio* cio);

texu_cio*           texu_cio_new();
void                texu_cio_del(texu_cio*);

#if (defined WIN32 && defined _WINDOWS)
texu_i32            texu_cio_init(texu_cio*, texu_env *env);
#elif (defined WIN32 && defined _CONSOLE)
texu_i32            texu_cio_init(texu_cio*, texu_env* env, texu_i32, texu_i32);
#elif (defined __USE_TERMIOS__ || defined __USE_TERMBOX2__)
texu_i32            texu_cio_init(texu_cio*, texu_env* env, texu_i32, texu_i32);
#else
texu_i32            texu_cio_init(texu_cio*, texu_i32, texu_i32);
#endif
void                texu_cio_release(texu_cio*);

#if (defined __USE_TERMIOS__ || defined __USE_TERMBOX2__)
void texu_cio_set_cbreak(texu_cio *cio, int cbreak, void (*sigint_handler)(void*), void *userdata);
void texu_cio_sigint(texu_cio *cio);
texu_i32 texu_cio_dispatch(texu_cio *cio); /*to get to modifier (ALT=1/CTRL=2/SHIFT=4), see also termbox2.h*/
texu_i32 texu_cio_translate(texu_cio *cio, texu_i32 key);
texu_i32 texu_cio_translate2(texu_cio *cio, texu_i32 key, texu_i32 *mod);
#endif


#if (defined VMS || defined __VMS__)
void texu_cio_set_timeout(texu_cio* cio, void(*on_timeout)(texu_cio*));
void texu_cio_erase_chars(texu_cio* cio, texu_i32 no_chars, texu_i32 y, texu_i32 x);
void texu_cio_flip_display(texu_cio* cio);
void texu_cio_begin_update(texu_cio* cio);
void texu_cio_end_update(texu_cio* cio);
#endif
texu_i32 texu_cio_get_keyname(texu_cio* cio, texu_i32 keycode, texu_char* keyname, texu_i32 len);

/* simple screen control */
texu_i32            texu_cio_clear(texu_cio*);
texu_i32            texu_cio_clearln(texu_cio*, texu_i32);
texu_i32            texu_cio_echo(texu_cio*, texu_bool);
/* cursor */
texu_i32            texu_cio_gotoyx(texu_cio*, texu_i32, texu_i32);
texu_i32            texu_cio_getyx(texu_cio*, texu_i32* y, texu_i32* x);
/* get/put */
texu_i32            texu_cio_getch(texu_cio*);
texu_i32            texu_cio_getch2(texu_cio*, texu_i32 timeout);
texu_i32            texu_cio_getstr(texu_cio*, texu_char*);
#if defined WIN32 && defined _CONSOLE
texu_i32
texu_cio_win32_getch(texu_cio* cio, texu_char* ascii, texu_i32* alt, texu_i32* ctrl, texu_i32* shift);
#endif
texu_i32            texu_cio_putch(texu_cio*, texu_i32, texu_i32, texu_i32);
texu_i32            texu_cio_putch_erase(texu_cio*, texu_i32, texu_i32, texu_i32, texu_bool);
texu_i32            texu_cio_putch_attr(texu_cio*, texu_i32, texu_i32, texu_i32, texu_i32);
texu_i32            texu_cio_putch_attr2(texu_cio*, texu_i32, texu_i32, texu_i32, texu_i32, texu_i32);

texu_i32            texu_cio_putch_attr_erase(texu_cio*, texu_i32, texu_i32, texu_i32, texu_i32,
                                       texu_bool erase);
texu_i32            texu_cio_putstr_attr_erase(texu_cio*, texu_i32, texu_i32, const texu_char*, texu_i32,
                                       texu_bool erase);

texu_i32            texu_cio_putstr(texu_cio*, texu_i32, texu_i32, const texu_char*);
texu_i32            texu_cio_putstr_attr(texu_cio*, texu_i32, texu_i32, const texu_char*, texu_i32);
texu_i32            texu_cio_putstr_attr2(texu_cio*, texu_i32, texu_i32, const texu_char*, texu_i32, texu_i32);

texu_i32            texu_cio_draw_text(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *text,
                                       texu_ui32 color, texu_ui32 bgcolor,
                                       const texu_char *clsname,
                                       texu_ui32 id);
texu_i32            texu_cio_draw_text2(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *text,
                                       texu_ui32 color, texu_ui32 bgcolor, texu_ui32 attrs,
                                       const texu_char *clsname,
                                       texu_ui32 id);

texu_i32            texu_cio_draw_text_erase(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *text,
                                       texu_ui32 color, texu_ui32 bgcolor,
                                       const texu_char *clsname,
                                       texu_ui32 id,
                                       texu_bool erase);

texu_i32            texu_cio_draw_char(texu_cio *cio, texu_i32 y, texu_i32 x, texu_char ch,
                                        texu_ui32 color, texu_ui32 bgcolor,
                                        const texu_char *clsname,
                                        texu_ui32 id);
texu_i32            texu_cio_draw_char_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_char ch,
                                        texu_ui32 color, texu_ui32 bgcolor,
                                        const texu_char *clsname,
                                        texu_ui32 id,
                                       texu_bool erase);
texu_i32            texu_cio_draw_char2(texu_cio *cio, texu_i32 y, texu_i32 x, texu_char ch,
                                        texu_ui32 color, texu_ui32 bgcolor, texu_ui32 attrs,
                                        const texu_char *clsname,
                                        texu_ui32 id);

texu_i32            texu_cio_refresh(texu_cio*);
/* text attributes */
texu_i32            texu_cio_attron(texu_cio*, texu_i32);
texu_i32            texu_cio_attroff(texu_cio*, texu_i32);


/* border */
#if 1//(defined __USE_TERMBOX2__)
texu_i32            texu_cio_draw_frame(texu_cio*, const texu_char* caption, texu_rect*, texu_i32 fg, texu_i32 bg, texu_i32 attrs);
texu_i32            texu_cio_draw_rect(texu_cio*, texu_rect*, texu_i32 fg, texu_i32 bg, texu_i32 attrs);
texu_i32            texu_cio_draw_line(texu_cio*, texu_i32 y, texu_i32 x, texu_i32 width, texu_i32 fg, texu_i32 bg, texu_i32 attrs);
texu_i32            texu_cio_draw_vline(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 height, texu_i32 fg, texu_i32 bg, texu_i32 attrs);

texu_i32            texu_cio_draw_hrects(texu_cio *cio, texu_rect *rect, texu_i32 *widths, texu_i32 nwidth, texu_i32 fg, texu_i32 bg, texu_i32 attrs);
texu_i32            texu_cio_draw_vrects(texu_cio *cio, texu_rect *rect, texu_i32 *heights, texu_i32 nheight, texu_i32 fg, texu_i32 bg, texu_i32 attrs);
#if defined __UTF8__
texu_i32            texu_cio_draw_line_utf8(texu_cio*, texu_i32 y, texu_i32 x, texu_i32 width, texu_i32 fg, texu_i32 bg, texu_i32 ch);
texu_i32            texu_cio_draw_vline_utf8(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 height, texu_i32 fg, texu_i32 bg, texu_i32 ch);
texu_i32            texu_cio_draw_box_utf8(texu_cio*, texu_i32 y, texu_i32 x, texu_i32 width, texu_i32 height, texu_i32 fg, texu_i32 bg, texu_i32 ch);
#endif
#else
texu_i32            texu_cio_draw_frame(texu_cio*, const texu_char*, texu_rect*, texu_i32);
texu_i32            texu_cio_draw_rect(texu_cio*, texu_rect*, texu_i32);
texu_i32            texu_cio_draw_line(texu_cio*, texu_i32, texu_i32, texu_i32, texu_i32);
texu_i32            texu_cio_draw_vline(texu_cio*, texu_i32, texu_i32, texu_i32, texu_i32);

texu_i32            texu_cio_draw_hrects(texu_cio*, texu_rect*, texu_i32*, texu_i32, texu_i32);
texu_i32            texu_cio_draw_vrects(texu_cio*, texu_rect*, texu_i32*, texu_i32, texu_i32);
#endif
texu_i32            texu_cio_get_color(texu_cio*, texu_i32);
texu_i32            texu_cio_get_underline(texu_cio*, texu_i32);
texu_i32            texu_cio_get_reverse(texu_cio*, texu_i32);
texu_i32            texu_cio_get_blink(texu_cio*, texu_i32);
texu_i32            texu_cio_get_color_attr(texu_cio*, texu_i32, texu_i32);
#if (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32            texu_cio_get_bgcolor(texu_cio*, texu_i32);
texu_i32            texu_cio_get_bgreverse(texu_cio*, texu_i32);
#endif

texu_i32            texu_cio_save_screen(texu_cio*, FILE*);
texu_i32            texu_cio_restore_screen(texu_cio*, FILE*);

texu_i32            texu_cio_nodelay(texu_cio* cio, texu_i32 delay);
void                texu_cio_interval(texu_cio* cio, texu_i32 msec /*1000 msec = 1sec*/);
#ifdef __cplusplus
}
#endif

#endif /* _TEXUIO_H_ */