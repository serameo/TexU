#ifndef __TEXU_TERMIOS_H__
#define __TEXU_TERMIOS_H__

#if defined __USE_TERMIOS__

#include "../termbox/termbox.h"

struct texu_cio;
#if defined __USE_TERMIOS__
typedef void (*fn_signal_callback)(int sig, void*);
typedef void (*fn_sigint_callback)(void*);
#endif

#ifdef __cplusplus
extern "C" {
#endif
int     texu_termios_init(int lines, int cols);
void    texu_termios_release();
int     texu_termios_getch(struct texu_cio *cio, struct tb_event *ev, int timeout);
void    texu_termios_gotoyx(int y, int x);
void    texu_termios_clear();
int     texu_termios_putch(int y, int x, char ch, unsigned short fg, unsigned short bg);
int     texu_termios_putstr(int y, int x, const char* str, unsigned short fg, unsigned short bg);
int     texu_termios_refresh();
void    texu_termios_cbreak(struct texu_cio *cio, int cbreak);/*, fn_sigint_callback sigint_handler, void *userdata);*/
void    texu_termios_getyx(int *y, int *x);

#ifdef __cplusplus
}
#endif

#endif /*__USE_TERMIOS__*/
#endif /*__TEXU_TERMIOS_H__*/
