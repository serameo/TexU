#ifndef __TEXU_TERMIOS_H__
#define __TEXU_TERMIOS_H__

#include "../termbox/termbox.h"

#include "texutermios.h"
#if defined __USE_TERMIOS__

#ifdef __cplusplus
extern "C" {
#endif
int     texu_termios_init(int lines, int cols);
void    texu_termios_release();
int     texu_termios_getch(struct tb_event *ev, int timeout);
void    texu_termios_gotoyx(int y, int x);
void    texu_termios_clear();
int     texu_termios_putch(int y, int x, char ch, uint16_t fg, uint16_t bg);
int     texu_termios_putstr(int y, int x, const char* str, uint16_t fg, uint16_t bg);
int     texu_termios_refresh();

#ifdef __cplusplus
}
#endif

#endif /*__USE_TERMIOS__*/
#endif /*__TEXU_TERMIOS_H__*/
