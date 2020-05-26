/*
#
# File Name: texuio.h
# Author: Seree Rakwong
# Date: 13-MAY-2020
# Purpose: get input/output from/to the screen
#
*/

#ifndef _TEXUIO_H_
#define _TEXUIO_H_

#include <stdio.h>
#include <ncurses.h>

#include "texumacros.h"
#include "texutypes.h"
#include "texuconst.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
# TexU io object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

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
  TEXU_CIO_COLOR_WHITE_WHITE
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
  TEXU_CIO_COLOR_WHITE   = COLOR_WHITE
};


struct texu_cio;
typedef struct texu_cio texu_cio;

texu_cio*          texu_cio_new();
void               texu_cio_del(texu_cio*);

texu_i32           texu_cio_init(texu_cio*);
void               texu_cio_release(texu_cio*);
/* simple screen control */
texu_i32           texu_cio_clear(texu_cio*);
texu_i32           texu_cio_clearln(texu_cio*, texu_i32);
texu_i32           texu_cio_echo(texu_cio*, texu_bool);
/* cursor */
texu_i32           texu_cio_gotoyx(texu_cio*, texu_i32, texu_i32);
texu_i32           texu_cio_getyx(texu_cio*, texu_i32, texu_i32);
/* get/put */
texu_i32           texu_cio_getch(texu_cio*);
texu_i32           texu_cio_getstr(texu_cio*, texu_char*);

texu_i32           texu_cio_putch(texu_cio*, texu_i32, texu_i32, texu_i32);
texu_i32           texu_cio_putch_attr(texu_cio*, texu_i32, texu_i32, texu_i32, texu_i32);
texu_i32           texu_cio_putstr(texu_cio*, texu_i32, texu_i32, texu_char*);
texu_i32           texu_cio_putstr_attr(texu_cio*, texu_i32, texu_i32, texu_char*, texu_i32);

texu_i32           texu_cio_refresh(texu_cio*);
/* text attributes */
texu_i32           texu_cio_attron(texu_cio*, texu_i32);
texu_i32           texu_cio_attroff(texu_cio*, texu_i32);

/*
# TexU window object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/











#ifdef __cplusplus
}
#endif

#endif /* _TEXUIO_H_ */


