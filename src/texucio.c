/*
#
# File Name: texuio.c
# Author: Seree Rakwong
# Date: 14-MAY-2020
#
*/

#include <stdlib.h>
#include <string.h>

#include "texulib.h"
#include "texutils.h"
#include "texucio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
# TexU io object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_cio
{
  WINDOW*          wndscr;
};

void               _texu_cio_init_colors(texu_cio* cio);

texu_cio*
texu_cio_new()
{
  texu_cio* cio = (texu_cio*)malloc(sizeof(texu_cio));
  if (cio)
  {
    memset(cio, 0, sizeof(texu_cio));
  }
  return cio;
}

void
texu_cio_del(texu_cio* cio)
{
  if (cio)
  {
    texu_cio_release(cio);
    free(cio);
    cio = 0;
  }
}

void
_texu_cio_init_colors(texu_cio* cio)
{
  init_color(COLOR_WHITE, 1000, 1000, 1000);
  
  init_pair(TEXU_CIO_COLOR_BLACK_BLACK,     COLOR_BLACK,    COLOR_BLACK);
  init_pair(TEXU_CIO_COLOR_BLACK_RED,       COLOR_BLACK,    COLOR_RED);
  init_pair(TEXU_CIO_COLOR_BLACK_GREEN,     COLOR_BLACK,    COLOR_GREEN);
  init_pair(TEXU_CIO_COLOR_BLACK_YELLOW,    COLOR_BLACK,    COLOR_YELLOW);
  init_pair(TEXU_CIO_COLOR_BLACK_BLUE,      COLOR_BLACK,    COLOR_BLUE);
  init_pair(TEXU_CIO_COLOR_BLACK_MAGENTA,   COLOR_BLACK,    COLOR_MAGENTA);
  init_pair(TEXU_CIO_COLOR_BLACK_CYAN,      COLOR_BLACK,    COLOR_CYAN);
  init_pair(TEXU_CIO_COLOR_BLACK_WHITE,     COLOR_BLACK,    COLOR_WHITE);
  init_pair(TEXU_CIO_COLOR_RED_BLACK,       COLOR_RED,      COLOR_BLACK);
  init_pair(TEXU_CIO_COLOR_RED_RED,         COLOR_RED,      COLOR_RED);
  init_pair(TEXU_CIO_COLOR_RED_GREEN,       COLOR_RED,      COLOR_GREEN);
  init_pair(TEXU_CIO_COLOR_RED_YELLOW,      COLOR_RED,      COLOR_YELLOW);
  init_pair(TEXU_CIO_COLOR_RED_BLUE,        COLOR_RED,      COLOR_BLUE);
  init_pair(TEXU_CIO_COLOR_RED_MAGENTA,     COLOR_RED,      COLOR_MAGENTA);
  init_pair(TEXU_CIO_COLOR_RED_CYAN,        COLOR_RED,      COLOR_CYAN);
  init_pair(TEXU_CIO_COLOR_RED_WHITE,       COLOR_RED,      COLOR_WHITE);
  init_pair(TEXU_CIO_COLOR_GREEN_BLACK,     COLOR_GREEN,    COLOR_BLACK);
  init_pair(TEXU_CIO_COLOR_GREEN_RED,       COLOR_GREEN,    COLOR_RED);
  init_pair(TEXU_CIO_COLOR_GREEN_GREEN,     COLOR_GREEN,    COLOR_GREEN);
  init_pair(TEXU_CIO_COLOR_GREEN_YELLOW,    COLOR_GREEN,    COLOR_YELLOW);
  init_pair(TEXU_CIO_COLOR_GREEN_BLUE,      COLOR_GREEN,    COLOR_BLUE);
  init_pair(TEXU_CIO_COLOR_GREEN_MAGENTA,   COLOR_GREEN,    COLOR_MAGENTA);
  init_pair(TEXU_CIO_COLOR_GREEN_CYAN,      COLOR_GREEN,    COLOR_CYAN);
  init_pair(TEXU_CIO_COLOR_GREEN_WHITE,     COLOR_GREEN,    COLOR_WHITE);
  init_pair(TEXU_CIO_COLOR_YELLOW_BLACK,    COLOR_YELLOW,   COLOR_BLACK);
  init_pair(TEXU_CIO_COLOR_YELLOW_RED,      COLOR_YELLOW,   COLOR_RED);
  init_pair(TEXU_CIO_COLOR_YELLOW_GREEN,    COLOR_YELLOW,   COLOR_GREEN);
  init_pair(TEXU_CIO_COLOR_YELLOW_YELLOW,   COLOR_YELLOW,   COLOR_YELLOW);
  init_pair(TEXU_CIO_COLOR_YELLOW_BLUE,     COLOR_YELLOW,   COLOR_BLUE);
  init_pair(TEXU_CIO_COLOR_YELLOW_MAGENTA,  COLOR_YELLOW,   COLOR_MAGENTA);
  init_pair(TEXU_CIO_COLOR_YELLOW_CYAN,     COLOR_YELLOW,   COLOR_CYAN);
  init_pair(TEXU_CIO_COLOR_YELLOW_WHITE,    COLOR_YELLOW,   COLOR_WHITE);
  init_pair(TEXU_CIO_COLOR_BLUE_BLACK,      COLOR_BLUE,     COLOR_BLACK);
  init_pair(TEXU_CIO_COLOR_BLUE_RED,        COLOR_BLUE,     COLOR_RED);
  init_pair(TEXU_CIO_COLOR_BLUE_GREEN,      COLOR_BLUE,     COLOR_GREEN);
  init_pair(TEXU_CIO_COLOR_BLUE_YELLOW,     COLOR_BLUE,     COLOR_YELLOW);
  init_pair(TEXU_CIO_COLOR_BLUE_BLUE,       COLOR_BLUE,     COLOR_BLUE);
  init_pair(TEXU_CIO_COLOR_BLUE_MAGENTA,    COLOR_BLUE,     COLOR_MAGENTA);
  init_pair(TEXU_CIO_COLOR_BLUE_CYAN,       COLOR_BLUE,     COLOR_CYAN);
  init_pair(TEXU_CIO_COLOR_BLUE_WHITE,      COLOR_BLUE,     COLOR_WHITE);
  init_pair(TEXU_CIO_COLOR_MAGENTA_BLACK,   COLOR_MAGENTA,  COLOR_BLACK);
  init_pair(TEXU_CIO_COLOR_MAGENTA_RED,     COLOR_MAGENTA,  COLOR_RED);
  init_pair(TEXU_CIO_COLOR_MAGENTA_GREEN,   COLOR_MAGENTA,  COLOR_GREEN);
  init_pair(TEXU_CIO_COLOR_MAGENTA_YELLOW,  COLOR_MAGENTA,  COLOR_YELLOW);
  init_pair(TEXU_CIO_COLOR_MAGENTA_BLUE,    COLOR_MAGENTA,  COLOR_BLUE);
  init_pair(TEXU_CIO_COLOR_MAGENTA_MAGENTA, COLOR_MAGENTA,  COLOR_MAGENTA);
  init_pair(TEXU_CIO_COLOR_MAGENTA_CYAN,    COLOR_MAGENTA,  COLOR_CYAN);
  init_pair(TEXU_CIO_COLOR_MAGENTA_WHITE,   COLOR_MAGENTA,  COLOR_WHITE);
  init_pair(TEXU_CIO_COLOR_CYAN_BLACK,      COLOR_CYAN,     COLOR_BLACK);
  init_pair(TEXU_CIO_COLOR_CYAN_RED,        COLOR_CYAN,     COLOR_RED);
  init_pair(TEXU_CIO_COLOR_CYAN_GREEN,      COLOR_CYAN,     COLOR_GREEN);
  init_pair(TEXU_CIO_COLOR_CYAN_YELLOW,     COLOR_CYAN,     COLOR_YELLOW);
  init_pair(TEXU_CIO_COLOR_CYAN_BLUE,       COLOR_CYAN,     COLOR_BLUE);
  init_pair(TEXU_CIO_COLOR_CYAN_MAGENTA,    COLOR_CYAN,     COLOR_MAGENTA);
  init_pair(TEXU_CIO_COLOR_CYAN_CYAN,       COLOR_CYAN,     COLOR_CYAN);
  init_pair(TEXU_CIO_COLOR_CYAN_WHITE,      COLOR_CYAN,     COLOR_WHITE);
  init_pair(TEXU_CIO_COLOR_WHITE_BLACK,     COLOR_WHITE,    COLOR_BLACK);
  init_pair(TEXU_CIO_COLOR_WHITE_RED,       COLOR_WHITE,    COLOR_RED);
  init_pair(TEXU_CIO_COLOR_WHITE_GREEN,     COLOR_WHITE,    COLOR_GREEN);
  init_pair(TEXU_CIO_COLOR_WHITE_YELLOW,    COLOR_WHITE,    COLOR_YELLOW);
  init_pair(TEXU_CIO_COLOR_WHITE_BLUE,      COLOR_WHITE,    COLOR_BLUE);
  init_pair(TEXU_CIO_COLOR_WHITE_MAGENTA,   COLOR_WHITE,    COLOR_MAGENTA);
  init_pair(TEXU_CIO_COLOR_WHITE_CYAN,      COLOR_WHITE,    COLOR_CYAN);
  init_pair(TEXU_CIO_COLOR_WHITE_WHITE,     COLOR_WHITE,    COLOR_WHITE);

/*BRIGHT WHITE*/  
  init_pair(TEXU_CIO_BRIGHT_WHITE_BLACK,    TEXU_CIO_BRIGHT_WHITE,  COLOR_BLACK);
  init_pair(TEXU_CIO_BRIGHT_WHITE_RED,      TEXU_CIO_BRIGHT_WHITE,  COLOR_RED);
  init_pair(TEXU_CIO_BRIGHT_WHITE_GREEN,    TEXU_CIO_BRIGHT_WHITE,  COLOR_GREEN);
  init_pair(TEXU_CIO_BRIGHT_WHITE_YELLOW,   TEXU_CIO_BRIGHT_WHITE,  COLOR_YELLOW);
  init_pair(TEXU_CIO_BRIGHT_WHITE_BLUE,     TEXU_CIO_BRIGHT_WHITE,  COLOR_BLUE);
  init_pair(TEXU_CIO_BRIGHT_WHITE_MAGENTA,  TEXU_CIO_BRIGHT_WHITE,  COLOR_MAGENTA);
  init_pair(TEXU_CIO_BRIGHT_WHITE_CYAN,     TEXU_CIO_BRIGHT_WHITE,  COLOR_CYAN);
  init_pair(TEXU_CIO_BRIGHT_WHITE_WHITE,    TEXU_CIO_BRIGHT_WHITE,  COLOR_WHITE);
  init_pair(TEXU_CIO_BLACK_BRIGHT_WHITE,    COLOR_BLACK,            TEXU_CIO_BRIGHT_WHITE);
  init_pair(TEXU_CIO_RED_BRIGHT_WHITE,      COLOR_RED,              TEXU_CIO_BRIGHT_WHITE);
  init_pair(TEXU_CIO_GREEN_BRIGHT_WHITE,    COLOR_GREEN,            TEXU_CIO_BRIGHT_WHITE);
  init_pair(TEXU_CIO_YELLOW_BRIGHT_WHITE,   COLOR_YELLOW,           TEXU_CIO_BRIGHT_WHITE);
  init_pair(TEXU_CIO_BLUE_BRIGHT_WHITE,     COLOR_BLUE,             TEXU_CIO_BRIGHT_WHITE);
  init_pair(TEXU_CIO_MAGENTA_BRIGHT_WHITE,  COLOR_MAGENTA,          TEXU_CIO_BRIGHT_WHITE);
  init_pair(TEXU_CIO_CYAN_BRIGHT_WHITE,     COLOR_CYAN,             TEXU_CIO_BRIGHT_WHITE);

}

texu_i32
texu_cio_init(texu_cio* cio)
{
  initscr();
  /*raw();*/
  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  start_color();
  _texu_cio_init_colors(cio);

  /*save window*/
  cio->wndscr = stdscr;
  return TEXU_OK;
}

void
texu_cio_release(texu_cio* cio)
{
  endwin();
  /* release window */
  cio->wndscr = 0;
}

texu_i32
texu_cio_echo(texu_cio* cio, texu_bool echo_)
{
  if (echo_)
  {
    return echo();
  }
  return noecho();
}

texu_i32
texu_cio_getch(texu_cio* cio)
{
  return getch();
}


texu_i32
texu_cio_getstr(texu_cio* cio, texu_char* str)
{
  return wgetstr(cio->wndscr, str);
}


texu_i32
texu_cio_gotoyx(texu_cio* cio, texu_i32 y, texu_i32 x)
{
  return wmove(cio->wndscr, y, x);
}

texu_i32
texu_cio_getyx(texu_cio* cio, texu_i32 y, texu_i32 x)
{
  return getyx(cio->wndscr, y, x);
}


texu_i32
texu_cio_clear(texu_cio* cio)
{
  return wclear(cio->wndscr);
}

texu_i32
texu_cio_clearln(texu_cio* cio, texu_i32 y)
{
  texu_cio_gotoyx(cio, y, 0);
  return wclrtoeol(cio->wndscr);
}

texu_i32
texu_cio_putch(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch)
{
  return mvwaddch(cio->wndscr, y, x, ch);
}

texu_i32
texu_cio_putch_attr(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs)
{
  texu_cio_attron(cio, attrs);
  mvwaddch(cio->wndscr, y, x, ch);
  return texu_cio_attroff(cio, attrs);
}

texu_i32
texu_cio_putch_attr2(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 color, texu_i32 attrs)
{
  return texu_cio_putch_attr(cio, y, x, ch, color | attrs);
}


texu_i32
texu_cio_putstr(texu_cio* cio, texu_i32 y, texu_i32 x, texu_char* str)
{
  return mvwprintw(cio->wndscr, y, x, str);
}

texu_i32
texu_cio_putstr_attr(texu_cio* cio, texu_i32 y, texu_i32 x, texu_char* str, texu_i32 attrs)
{
  texu_cio_attron(cio, attrs);
  mvwprintw(cio->wndscr, y, x, str);
  return texu_cio_attroff(cio, attrs);
}

texu_i32
texu_cio_putstr_attr2(texu_cio* cio, texu_i32 y, texu_i32 x, texu_char* str, texu_i32 color, texu_i32 attrs)
{
  return texu_cio_putstr_attr(cio, y, x, str, color | attrs);
}

texu_i32
texu_cio_attron(texu_cio* cio, texu_i32 attrs)
{
  return wattron(cio->wndscr, attrs);
}


texu_i32
texu_cio_attroff(texu_cio* cio, texu_i32 attrs)
{
  return wattroff(cio->wndscr, attrs);
}


texu_i32
texu_cio_refresh(texu_cio* cio)
{
  return wrefresh(cio->wndscr);
}

texu_i32
texu_cio_draw_frame(texu_cio* cio, texu_char* text, texu_rect* rect, texu_i32 attrs)
{
  texu_i32 len = 0;
  
  texu_cio_draw_rect(cio, rect, attrs);
  
  if (text)
  {
    len = strlen(text);
    texu_cio_putstr_attr(cio, rect->y,
      rect->x + (rect->cols - len)/2, text, attrs | A_REVERSE);
  }
  return TEXU_OK;
}

texu_i32
texu_cio_draw_rect(texu_cio* cio, texu_rect* rect, texu_i32 attrs)
{
  texu_cio_attron(cio, attrs);
  
  /*draw top line*/
  texu_cio_draw_line(cio, rect->y, rect->x, rect->cols, attrs);
  /*draw bottom line*/
  texu_cio_draw_line(cio, rect->y+rect->lines, rect->x, rect->cols, attrs);
  /* draw left vertical line */
  texu_cio_draw_vline(cio, rect->y, rect->x, rect->lines, attrs);
  /* draw right vertical line */
  texu_cio_draw_vline(cio, rect->y, rect->x+rect->cols, rect->lines, attrs);
  
  /* draw upper left*/
  texu_cio_putch_attr(cio, rect->y, rect->x, ACS_ULCORNER, attrs);
  /* draw lower left*/
  texu_cio_putch_attr(cio, rect->y+rect->lines, rect->x, ACS_LLCORNER, attrs);
  /* draw upper right*/
  texu_cio_putch_attr(cio, rect->y, rect->x+rect->cols, ACS_URCORNER, attrs);
  /* draw lower right*/
  texu_cio_putch_attr(cio, rect->y+rect->lines, rect->x+rect->cols, ACS_LRCORNER, attrs);
  
  
  return texu_cio_attroff(cio, attrs);
}

texu_i32
texu_cio_draw_line(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 width, texu_i32 attrs)
{
  texu_i32 i = 0;
  texu_cio_attron(cio, attrs);
  for (i=0; i<width; ++i)
  {
    texu_cio_putch_attr(cio, y, x+i, ACS_HLINE, attrs);
  }
  return texu_cio_attroff(cio, attrs);
}

texu_i32
texu_cio_draw_vline(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 height, texu_i32 attrs)
{
  texu_i32 i = 0;
  texu_cio_attron(cio, attrs);
  for (i=0; i<height; ++i)
  {
    texu_cio_putch_attr(cio, y+i, x, ACS_VLINE, attrs);
  }
  return texu_cio_attroff(cio, attrs);
}

texu_i32
texu_cio_draw_hrects(texu_cio* cio, texu_rect* rect, texu_i32* widths, texu_i32 nwidth, texu_i32 attrs)
{
  texu_i32 i = 0;
  texu_rect rc = *rect;
  
  for (i=0; i<nwidth; ++i)
  {
    rc.cols = widths[i];
    texu_cio_draw_rect(cio, &rc, attrs);
    
    texu_cio_putch_attr(cio, rc.y, rc.x, ACS_TTEE, attrs);
    texu_cio_putch_attr(cio, rc.y+rc.lines, rc.x, ACS_BTEE, attrs);
    rc.x += widths[i];
  }
  texu_cio_putch_attr(cio, rect->y, rect->x, ACS_ULCORNER, attrs);
  texu_cio_putch_attr(cio, rect->y+rect->lines, rect->x, ACS_LLCORNER, attrs);
  
  return TEXU_OK;
}



texu_i32
texu_cio_draw_vrects(texu_cio* cio, texu_rect* rect, texu_i32* heights, texu_i32 nheight, texu_i32 attrs)
{
  texu_i32 i = 0;
  texu_rect rc = *rect;
  
  for (i=0; i<nheight; ++i)
  {
    rc.lines = heights[i];
    texu_cio_draw_rect(cio, &rc, attrs);
    
    texu_cio_putch_attr(cio, rc.y, rc.x, ACS_LTEE, attrs);
    texu_cio_putch_attr(cio, rc.y, rc.x+rc.cols, ACS_RTEE, attrs);
    rc.y += heights[i];
  }
  texu_cio_putch_attr(cio, rect->y, rect->x, ACS_ULCORNER, attrs);
  texu_cio_putch_attr(cio, rect->y, rect->x+rect->cols, ACS_URCORNER, attrs);
  
  return TEXU_OK;
}


texu_i32
texu_cio_get_color(texu_cio* cio, texu_i32 clridx)
{
  return COLOR_PAIR(clridx);
}

texu_i32
texu_cio_get_underline(texu_cio* cio, texu_i32 clridx)
{
  return COLOR_PAIR(clridx) | A_UNDERLINE;
}

texu_i32
texu_cio_get_reverse(texu_cio* cio, texu_i32 clridx)
{
  return COLOR_PAIR(clridx) | A_REVERSE;
}

texu_i32
texu_cio_get_blink(texu_cio* cio, texu_i32 clridx)
{
  return COLOR_PAIR(clridx) | A_BLINK;
}

texu_i32
texu_cio_get_color_attr(texu_cio* cio, texu_i32 clridx, texu_i32 attrs)
{
  return COLOR_PAIR(clridx) | attrs;
}




#ifdef __cplusplus
}
#endif


