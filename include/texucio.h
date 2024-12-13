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

#if 0

enum
{
  TEXU_CIO_COLOR_BLACK_BLACK,
  TEXU_CIO_COLOR_BLACK_RED,
  TEXU_CIO_COLOR_BLACK_GREEN,
  TEXU_CIO_COLOR_BLACK_YELLOW,
  TEXU_CIO_COLOR_BLACK_BLUE,
  TEXU_CIO_COLOR_BLACK_MAGENTA,
  TEXU_CIO_COLOR_BLACK_CYAN,
  TEXU_CIO_COLOR_BLACK_WHITE,
  TEXU_CIO_COLOR_RED_BLACK,
  TEXU_CIO_COLOR_RED_RED,
  TEXU_CIO_COLOR_RED_GREEN,
  TEXU_CIO_COLOR_RED_YELLOW,
  TEXU_CIO_COLOR_RED_BLUE,
  TEXU_CIO_COLOR_RED_MAGENTA,
  TEXU_CIO_COLOR_RED_CYAN,
  TEXU_CIO_COLOR_RED_WHITE,
  TEXU_CIO_COLOR_GREEN_BLACK,
  TEXU_CIO_COLOR_GREEN_RED,
  TEXU_CIO_COLOR_GREEN_GREEN,
  TEXU_CIO_COLOR_GREEN_YELLOW,
  TEXU_CIO_COLOR_GREEN_BLUE,
  TEXU_CIO_COLOR_GREEN_MAGENTA,
  TEXU_CIO_COLOR_GREEN_CYAN,
  TEXU_CIO_COLOR_GREEN_WHITE,
  TEXU_CIO_COLOR_YELLOW_BLACK,
  TEXU_CIO_COLOR_YELLOW_RED,
  TEXU_CIO_COLOR_YELLOW_GREEN,
  TEXU_CIO_COLOR_YELLOW_YELLOW,
  TEXU_CIO_COLOR_YELLOW_BLUE,
  TEXU_CIO_COLOR_YELLOW_MAGENTA,
  TEXU_CIO_COLOR_YELLOW_CYAN,
  TEXU_CIO_COLOR_YELLOW_WHITE,
  TEXU_CIO_COLOR_BLUE_BLACK,
  TEXU_CIO_COLOR_BLUE_RED,
  TEXU_CIO_COLOR_BLUE_GREEN,
  TEXU_CIO_COLOR_BLUE_YELLOW,
  TEXU_CIO_COLOR_BLUE_BLUE,
  TEXU_CIO_COLOR_BLUE_MAGENTA,
  TEXU_CIO_COLOR_BLUE_CYAN,
  TEXU_CIO_COLOR_BLUE_WHITE,
  TEXU_CIO_COLOR_MAGENTA_BLACK,
  TEXU_CIO_COLOR_MAGENTA_RED,
  TEXU_CIO_COLOR_MAGENTA_GREEN,
  TEXU_CIO_COLOR_MAGENTA_YELLOW,
  TEXU_CIO_COLOR_MAGENTA_BLUE,
  TEXU_CIO_COLOR_MAGENTA_MAGENTA,
  TEXU_CIO_COLOR_MAGENTA_CYAN,
  TEXU_CIO_COLOR_MAGENTA_WHITE,
  TEXU_CIO_COLOR_CYAN_BLACK,
  TEXU_CIO_COLOR_CYAN_RED,
  TEXU_CIO_COLOR_CYAN_GREEN,
  TEXU_CIO_COLOR_CYAN_YELLOW,
  TEXU_CIO_COLOR_CYAN_BLUE,
  TEXU_CIO_COLOR_CYAN_MAGENTA,
  TEXU_CIO_COLOR_CYAN_CYAN,
  TEXU_CIO_COLOR_CYAN_WHITE,
  TEXU_CIO_COLOR_WHITE_BLACK,
  TEXU_CIO_COLOR_WHITE_RED,
  TEXU_CIO_COLOR_WHITE_GREEN,
  TEXU_CIO_COLOR_WHITE_YELLOW,
  TEXU_CIO_COLOR_WHITE_BLUE,
  TEXU_CIO_COLOR_WHITE_MAGENTA,
  TEXU_CIO_COLOR_WHITE_CYAN,
  TEXU_CIO_COLOR_WHITE_WHITE,
  TEXU_CIO_BRIGHT_WHITE_BLACK,
  TEXU_CIO_BRIGHT_WHITE_RED,
  TEXU_CIO_BRIGHT_WHITE_GREEN,
  TEXU_CIO_BRIGHT_WHITE_YELLOW,
  TEXU_CIO_BRIGHT_WHITE_BLUE,
  TEXU_CIO_BRIGHT_WHITE_MAGENTA,
  TEXU_CIO_BRIGHT_WHITE_CYAN,
  TEXU_CIO_BRIGHT_WHITE_WHITE,
  TEXU_CIO_BLACK_BRIGHT_WHITE,
  TEXU_CIO_RED_BRIGHT_WHITE,
  TEXU_CIO_GREEN_BRIGHT_WHITE,
  TEXU_CIO_YELLOW_BRIGHT_WHITE,
  TEXU_CIO_BLUE_BRIGHT_WHITE,
  TEXU_CIO_MAGENTA_BRIGHT_WHITE,
  TEXU_CIO_CYAN_BRIGHT_WHITE,
  TEXU_CIO_LAST_COLOR
};

enum
{
  TEXU_CIO_COLOR_BLACK   = COLOR_BLACK,
  TEXU_CIO_COLOR_RED     = COLOR_RED,
  TEXU_CIO_COLOR_GREEN   = COLOR_GREEN,
  TEXU_CIO_COLOR_YELLOW  = COLOR_YELLOW,
  TEXU_CIO_COLOR_BLUE    = COLOR_BLUE,
  TEXU_CIO_COLOR_MAGENTA = COLOR_MAGENTA,
  TEXU_CIO_COLOR_CYAN    = COLOR_CYAN,
  TEXU_CIO_COLOR_WHITE   = COLOR_WHITE,
#ifdef XTERM_256COLOR
  TEXU_CIO_BRIGHT_WHITE  = 15
#else
  TEXU_CIO_BRIGHT_WHITE  = COLOR_WHITE
#endif
};

#endif

texu_env            *texu_cio_get_env(texu_cio* cio);

texu_cio*           texu_cio_new();
void                texu_cio_del(texu_cio*);

#if (defined WIN32 && defined _WINDOWS)
texu_i32            texu_cio_init(texu_cio*, texu_env *env);
#elif (defined WIN32 && defined _CONSOLE)
texu_i32            texu_cio_init(texu_cio*, texu_env* env, texu_i32, texu_i32);
#else
texu_i32            texu_cio_init(texu_cio*, texu_i32, texu_i32);
#endif
void                texu_cio_release(texu_cio*);


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

texu_i32            texu_cio_refresh(texu_cio*);
/* text attributes */
texu_i32            texu_cio_attron(texu_cio*, texu_i32);
texu_i32            texu_cio_attroff(texu_cio*, texu_i32);


/* border */
texu_i32            texu_cio_draw_frame(texu_cio*, const texu_char*, texu_rect*, texu_i32);
texu_i32            texu_cio_draw_rect(texu_cio*, texu_rect*, texu_i32);
texu_i32            texu_cio_draw_line(texu_cio*, texu_i32, texu_i32, texu_i32, texu_i32);
texu_i32            texu_cio_draw_vline(texu_cio*, texu_i32, texu_i32, texu_i32, texu_i32);

texu_i32            texu_cio_draw_hrects(texu_cio*, texu_rect*, texu_i32*, texu_i32, texu_i32);
texu_i32            texu_cio_draw_vrects(texu_cio*, texu_rect*, texu_i32*, texu_i32, texu_i32);

texu_i32            texu_cio_get_color(texu_cio*, texu_i32);
texu_i32            texu_cio_get_underline(texu_cio*, texu_i32);
texu_i32            texu_cio_get_reverse(texu_cio*, texu_i32);
texu_i32            texu_cio_get_blink(texu_cio*, texu_i32);
texu_i32            texu_cio_get_color_attr(texu_cio*, texu_i32, texu_i32);

texu_i32            texu_cio_save_screen(texu_cio*, FILE*);
texu_i32            texu_cio_restore_screen(texu_cio*, FILE*);

texu_i32            texu_cio_nodelay(texu_cio* cio, texu_i32 delay);
void                texu_cio_interval(texu_cio* cio, texu_i32 msec /*1000 msec = 1sec*/);
#ifdef __cplusplus
}
#endif

#endif /* _TEXUIO_H_ */