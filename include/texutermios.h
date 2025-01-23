#ifndef __TEXU_TERMIOS_H__
#define __TEXU_TERMIOS_H__

#ifdef __cplusplus
extern "C" {
#endif
void texu_term_sprint_box(char *esc, int len, int row, int col, int height, int width);
void texu_term_sprint_vline(char *esc, int len, int row, int col, int height);
void texu_term_sprint_hline(char *esc, int len, int row, int col, int width);
void texu_term_sprint_shade_line(char *esc, int len, int row, int col, int width);
void texu_term_sprint_shadow(char *esc, int len, int row, int col, int height, int width);
#ifdef __cplusplus
}
#endif


#if defined __USE_TERMIOS__
struct texu_cio; /*see texucio.h*/
struct tb_event; /*see termbox2.h*/

typedef void (*fn_signal_callback)(int sig, void*);
typedef void (*fn_sigint_callback)(void*);

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
int     texu_termios_putch_attrs(int y, int x, char ch, unsigned short fg, unsigned short bg, unsigned short attrs);
int     texu_termios_putstr_attrs(int y, int x, const char* str, unsigned short fg, unsigned short bg, unsigned short attrs);
int     texu_termios_refresh();
void    texu_termios_cbreak(struct texu_cio *cio, int cbreak);/*, fn_sigint_callback sigint_handler, void *userdata);*/
void    texu_termios_getyx(int *y, int *x);
#ifdef __cplusplus
}
#endif

#elif defined __USE_TERMBOX2__
struct texu_cio; /*see texucio.h*/
struct tb_event; /*see termbox2.h*/
typedef void (*fn_signal_callback)(int sig, void*);
typedef void (*fn_sigint_callback)(void*);

#ifdef __cplusplus
extern "C" {
#endif
int     texu_tb2_init(int lines, int cols);
void    texu_tb2_release();
int     texu_tb2_getch(struct texu_cio *cio, struct tb_event *ev, int timeout);
void    texu_tb2_gotoyx(int y, int x);
void    texu_tb2_clear();
int     texu_tb2_putch(int y, int x, char ch, unsigned short fg, unsigned short bg);
int     texu_tb2_putstr(int y, int x, const char* str, unsigned short fg, unsigned short bg);
int     texu_tb2_putch_attrs(int y, int x, char ch, unsigned short fg, unsigned short bg, unsigned short attrs);
int     texu_tb2_putstr_attrs(int y, int x, const char* str, unsigned short fg, unsigned short bg, unsigned short attrs);
int     texu_tb2_refresh();
void    texu_tb2_cbreak(struct texu_cio *cio, int cbreak);/*, fn_sigint_callback sigint_handler, void *userdata);*/
void    texu_tb2_getyx(int *y, int *x);
#ifdef __cplusplus
}
#endif

#endif /*__USE_TERMIOS__*/
#endif /*__TEXU_TERMIOS_H__*/
