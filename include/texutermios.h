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
void texu_term_write_screen(char *esc);
void texu_term_clear_screen();
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

/*UNICODE CHAR*/
#if defined __UTF8__
#define TEXU_TB2_UPPERLEFT          (0x250C)
#define TEXU_TB2_UPPERRIGHT         (0x2510)
#define TEXU_TB2_LOWERLEFT          (0x2514)
#define TEXU_TB2_LOWERRIGHT         (0x2518)
#define TEXU_TB2_HORZLINE           (0x2500)
#define TEXU_TB2_VERTLINE           (0x2502)
#define TEXU_TB2_LEFTTEE            (0x251C)
#define TEXU_TB2_RIGHTTEE           (0x2524)
#define TEXU_TB2_TOPTEE             (0x252C)
#define TEXU_TB2_BOTTOMTEE          (0x2534)
#define TEXU_TB2_ALLTEE             (0x253C)
#define TEXU_TB2_SHADE0             (0x2590)
#define TEXU_TB2_SHADE1             (0x2591)
#define TEXU_TB2_SHADE2             (0x2592)
#endif

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
