#include "texutermios.h"
#include <stdio.h>

#if defined __USE_TERMIOS__
#include "../termbox/termbox.h"
#elif defined __USE_TERMBOX2__
#define TB_OPT_V1_COMPAT
#include "../termbox/termbox2.h"
#endif

#define TEXU_WIDE_SCREEN        "\\033[?3h"
#define TEXU_NORMAL_SCREEN      "\\033[?3l"

/*
dev-[LDV170]#echo -e "\033[10;10H\033(0j\033[10;11H\033(0k\033[10;12H\033(0l\033[10;13H\033(0m\033(B"
*/
#define TEXU_TERMBOX_GOTOYX(esc, len, row, col)         snprintf((esc), (len), "\033[%d;%dH", (row), (col))
#define TEXU_TERMBOX_CHAR(esc, len, row, col, ch)       snprintf((esc), (len), "\033[%d;%dH\033(0%c\033(B", (row), (col), (ch))
#define TEXU_TERMBOX_LR(esc, len, row, col)             TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'j')
#define TEXU_TERMBOX_UR(esc, len, row, col)             TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'k')
#define TEXU_TERMBOX_UL(esc, len, row, col)             TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'l')
#define TEXU_TERMBOX_LL(esc, len, row, col)             TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'm')
#define TEXU_TERMBOX_CROSS(esc, len, row, col)          TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'n')
#define TEXU_TERMBOX_HORZ1(esc, len, row, col)          TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'o')
#define TEXU_TERMBOX_HORZ2(esc, len, row, col)          TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'p')
#define TEXU_TERMBOX_HORZ3(esc, len, row, col)          TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'q')
#define TEXU_TERMBOX_HORZ4(esc, len, row, col)          TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'r')
#define TEXU_TERMBOX_HORZ5(esc, len, row, col)          TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 's')
#define TEXU_TERMBOX_LTEE(esc, len, row, col)           TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 't')
#define TEXU_TERMBOX_RTEE(esc, len, row, col)           TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'u')
#define TEXU_TERMBOX_BTEE(esc, len, row, col)           TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'v')
#define TEXU_TERMBOX_TTEE(esc, len, row, col)           TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'w')
#define TEXU_TERMBOX_VERT(esc, len, row, col)           TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'x')
#define TEXU_TERMBOX_HORZ(esc, len, row, col)           TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'q')
#define TEXU_TERMBOX_SHADE(esc, len, row, col)          TEXU_TERMBOX_CHAR((esc), (len), (row), (col), 'a')
#define TEXU_TERMBOX_HLINE(esc, len, row, col, width)   \
    do{ \
        char line[256]; \
        (width)=((width)>255?255:(width)); \
        memset(line,'q',(width)); \
        line[width]=0; \
        snprintf((esc),(len),"\033[%d;%dH\033(0%s\033(B",(row),(col),line); \
    }while (0)

#define TEXU_TERMBOX_VLINE(esc, len, row, col, height)  \
    do{ \
        char line[256]; \
        int r=0; \
        int line_len=0; \
        strcpy(esc,""); \
        for(r=(row);r<((row)+(height));++r) { \
            TEXU_TERMBOX_VERT(line,256,r,(col)); \
            line_len=strlen(line); \
            if (line_len+strlen(esc)<(len)) { \
                strcat(esc,line); \
            } else {break;} \
        } \
    }while (0)

#ifdef __cplusplus
extern "C" {
#endif

void texu_term_sprint_hline(char *esc, int len, int row, int col, int width)
{
    char line[256];
    width = ((width > 255) || (width < 1) ? 255 : width);
    memset(line , 'q', width);
    line[width] = 0;
    snprintf(esc, len, "\033[%d;%dH\033(0%s\033(B", row, col, line);
}

void texu_term_sprint_vline(char *esc, int len, int row, int col, int height)
{
    char line[256];
    int  r = 0;
    int  line_len = 0;
    strcpy(esc, "");
    for(r = row; r < ((row)+(height)); ++r) 
    {
        TEXU_TERMBOX_VERT(line, 255, r, col);
        line_len = strlen(line);
        if (line_len+strlen(esc) < len) 
        {
            strcat(esc, line);
        } 
        else 
        {
            break;
        }
    }
}

void texu_term_sprint_box(char *esc, int len, int row, int col, int height, int width)
{
    char top[1025];
    char bottom[1025];
    char left[1025];
    char right[1025];
    char ul[256];   /*upper left*/
    char ll[256];   /*lower left*/
    char ur[256];   /*upper right*/
    char lr[256];   /*lower right*/
    texu_term_sprint_hline(top, 1024, row, col, width);
    texu_term_sprint_hline(bottom, 1024, row+height, col, width);
    texu_term_sprint_vline(left, 1042, row, col, height);
    texu_term_sprint_vline(right, 1042, row, col+width, height);
    TEXU_TERMBOX_UL(ul, 255, row, col);
    TEXU_TERMBOX_UR(ur, 255, row, col+width);
    TEXU_TERMBOX_LL(ll, 255, row+height, col);
    TEXU_TERMBOX_LR(lr, 255, row+height, col+width);
    snprintf(esc, len, "%s%s%s%s%s%s%s%s", top, bottom, left, right, ul, ur, ll, lr);
}

void texu_term_sprint_shade_line(char *esc, int len, int row, int col, int width)
{
    char line[256];
    width = ((width > 255) || (width < 1) ? 255 : width);
    memset(line , 'a', width);
    line[width] = 0;
    snprintf(esc, len, "\033[%d;%dH\033(0%s\033(B", row, col, line);
}

void texu_term_sprint_shadow(char *esc, int len, int row, int col, int height, int width)
{
    char line[256];
    int  r = 0;
    int  line_len = 0;
    strcpy(esc, "");
    for(r = row; r < ((row)+(height)); ++r) 
    {
        texu_term_sprint_shade_line(line, 255, r, col, width);
        line_len = strlen(line);
        if (line_len+strlen(esc) < len) 
        {
            strcat(esc, line);
        } 
        else 
        {
            break;
        }
    }
}

#ifdef __cplusplus
}
#endif

#if defined __USE_TERMIOS__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#include "texuconst.h"
#include "texust.h"
#include "texucio.h"
#ifdef __cplusplus
extern "C" {
#endif

void texu_term_write_screen(char *esc)
{
    tb_write(esc, strlen(esc));
}

void texu_term_clear_screen()
{

}


static void print_tb(const char *str, int x, int y, unsigned short fg, unsigned short bg)
{
    while (*str) 
    {
        struct tb_cell cell = { *str, fg, bg };
        unsigned int uni;
        unsigned short fg2 = fg;
        unsigned short bg2 = bg;

        str += tb_utf8_char_to_unicode(&uni, str);

        tb_get_cell(x, y, &cell);
        if((unsigned short)(-1) == fg) fg2 = cell.fg;
        if((unsigned short)(-1) == bg) bg2 = cell.bg;
        tb_change_cell(x, y, uni, fg2, bg2);
        x++;
    }
}
/*
    struct tb_cell cell = { ch, fg, bg };
    tb_get_cell(x, y, &cell);

    if((unsigned short)(-1) == fg) fg = cell.fg;
    if((unsigned short)(-1) == bg) bg = cell.bg;
    tb_change_cell(x, y, ch, fg, bg);
*/
static void printf_tb(int x, int y, unsigned short fg, unsigned short bg, const char *fmt, ...)
{
    char buf[4096];
    va_list vl;
    va_start(vl, fmt);
    vsnprintf(buf, sizeof(buf), fmt, vl);
    va_end(vl);
    print_tb(buf, x, y, fg, bg);
}


void texu_termios_set_wide_size()
{
    //system("echo -e \"\\033[?3h\""); /*set terminal window size 132x25*/
    texu_term_write_screen("\033[?3h");
}

void texu_termios_set_normal_size()
{
    //system("echo -e \"\\033[?3l\""); /*set terminal window size 80x25*/
    texu_term_write_screen("\033[?3l");
}

int texu_termios_init(int lines, int cols)
{
    int rc = 0;
    
    texu_termios_set_wide_size();
    rc = tb_init();
    if (0 == rc)
    {
        /*struct sigaction sa;

        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = sigint_handler;
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, 0);*/
        /*tb_set_term_size(cols, lines);*/
        /*tb_select_input_mode(TB_INPUT_ESC);*/
        tb_select_input_mode(TB_INPUT_ALT);
    }
    else
    {
        texu_termios_set_normal_size();
    }
    return rc;
}

void    texu_termios_cbreak(struct texu_cio *cio, int cbreak)/*, fn_sigint_callback sigint_handler, void *userdata)*/
{
    int fd = tb_get_term_fd();
    struct termios tios;
    memset(&tios, 0, sizeof(tios));
    tcgetattr(fd, &tios);

    /*tios.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                            | INLCR | IGNCR | ICRNL | IXON);
    tios.c_oflag &= ~OPOST;
    tios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tios.c_cflag &= ~(CSIZE | PARENB);
    tios.c_cflag |= CS8;
    tios.c_cc[VMIN] = 0;
    tios.c_cc[VTIME] = 0;*/
    if (cbreak)
    {
        tios.c_lflag |= (ISIG);
        /*texu_cio_set_cbreak(cio, 1, sigint_handler, cio);*/
        /*cio->sigint_handler = sigint_handler;
        cio->userdata  = userdata;*/
    }
    else
    {
        tios.c_lflag &= ~(ISIG);
        /*texu_cio_set_cbreak(cio, 0, 0, 0);*/
        /*cio->sigint_handler = 0;
        cio->userdata  = 0;*/
    }
    tcsetattr(fd, TCSAFLUSH, &tios);
}

void texu_termios_release()
{
    tb_shutdown();
    texu_termios_set_normal_size();
}

int
texu_termios_getch(struct texu_cio *cio, struct tb_event *ev, int timeout)
{
    int rv = -1;
    if (timeout < 1)
    {
        rv = tb_poll_event(ev); /*wait until getting event*/
    }
    else
    {
        rv = tb_peek_event(ev, timeout);
    }
    if (0 != rv)
    {
        switch (ev->type)
        {
            case TB_EVENT_KEY:
                /*return (ev->key > 0 ? ev->key : ev->ch);*/
                {
                    if (ev->key == TB_KEY_CTRL_C)/*&& cio->sigint_handler)*/
                    {
                        /*cio->sigint_handler(cio->userdata);*/
                        texu_cio_sigint(cio);
                        return -1;
                    }
                    return (ev->key > 0 ? ev->key : ev->ch);
                }
            case TB_EVENT_RESIZE:
            case TB_EVENT_MOUSE:
                break;
        }
    }
    return -1;
}

void texu_termios_gotoyx(int y, int x)
{
    tb_set_cursor(x, y);
}

void texu_termios_clear()
{
    tb_clear();
}

int texu_termios_putch_attrs(int y, int x, char ch, unsigned short fg, unsigned short bg, unsigned short attrs)
{
    unsigned int uni;
    struct tb_cell cell = { ch, fg, bg };
    char str[4] = { ch, 0, 0, 0 };
    tb_utf8_char_to_unicode(&uni, str);

    tb_get_cell(x, y, &cell);

    if((unsigned short)(-1) == fg) fg = cell.fg;
    if((unsigned short)(-1) == bg) bg = cell.bg;
    tb_change_cell(x, y, uni, fg|attrs, bg|attrs);
    return 0;
}

int texu_termios_putch(int y, int x, char ch, unsigned short fg, unsigned short bg)
{
    return texu_termios_putch_attrs(y, x, ch, fg, bg, 0);
#if 0
    unsigned int uni;
    struct tb_cell cell = { ch, fg, bg };
    char str[4] = { ch, 0, 0, 0 };
    tb_utf8_char_to_unicode(&uni, str);

    tb_get_cell(x, y, &cell);

    if((unsigned short)(-1) == fg) fg = cell.fg;
    if((unsigned short)(-1) == bg) bg = cell.bg;
    tb_change_cell(x, y, uni, fg, bg);
    return 0;
#endif
}


int texu_termios_putstr(int y, int x, const char* str, unsigned short fg, unsigned short bg)
{
    print_tb(str, x, y, fg, bg);
    return 0;
}


int texu_termios_putstr_attrs(int y, int x, const char* str, unsigned short fg, unsigned short bg, unsigned short attrs)
{
    print_tb(str, x, y, fg|attrs, bg|attrs);
    return 0;
}

int texu_termios_refresh()
{
    tb_present();
    return 0;
}

void texu_termios_getyx(int *y, int *x)
{
    tb_get_cursor(x, y);
}
#ifdef __cplusplus
}
#endif

#elif defined __USE_TERMBOX2__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#include "texuconst.h"
#include "texust.h"
#include "texucio.h"
#ifdef __cplusplus
extern "C" {
#endif

void texu_term_write_screen(char *esc)
{
    tb_send(esc, strlen(esc));
}

void texu_term_clear_screen()
{
    
}

void texu_tb2_set_wide_size()
{
    //system("echo -e \"\\033[?3h\""); /*set terminal window size 132x25*/
    texu_term_write_screen("\033[?3h");
    //tb_write(TEXU_WIDE_SCREEN, strlen(TEXU_WIDE_SCREEN));
}

void texu_tb2_set_normal_size()
{
    //system("echo -e \"\\033[?3l\""); /*set terminal window size 80x25*/
    texu_term_write_screen("\033[?3l");
    //tb_write(TEXU_NORMAL_SCREEN, strlen(TEXU_NORMAL_SCREEN));
}

int texu_tb2_init(int lines, int cols)
{
    int rc = 0;
#if 0 //!(defined TERMBOX2_MAX_WIDTH) || (TERMBOX2_MAX_WIDTH <= 80)
    texu_tb2_set_wide_size();
#endif
    rc = tb_init();
    if (0 == rc)
    {
        /*tb_set_input_mode(TB_INPUT_ALT);*/
        tb_set_input_mode(TB_INPUT_ESC/*| TB_INPUT_MOUSE*/);
    }
    else
    {
        texu_tb2_set_normal_size();
    }
    return rc;
}

void    texu_tb2_cbreak(struct texu_cio *cio, int cbreak)
{
    int fd = 0;
    int szfd = 0;
    struct termios tios;
    memset(&tios, 0, sizeof(tios));
    tcgetattr(fd, &tios);

    tb_get_fds(&fd, &szfd);
    if (cbreak)
    {
        tios.c_lflag |= (ISIG);
    }
    else
    {
        tios.c_lflag &= ~(ISIG);
    }
    tcsetattr(fd, TCSAFLUSH, &tios);
}

void texu_tb2_release()
{
    tb_shutdown();
    /*texu_tb2_set_normal_size();*/
}

int
texu_tb2_getch(struct texu_cio *cio, struct tb_event *ev, int timeout)
{
    int rv = -1;
    if (timeout < 1)
    {
        rv = tb_poll_event(ev); /*wait until getting event*/
    }
    else
    {
        rv = tb_peek_event(ev, timeout);
    }
    if (0 == rv)
    {
        switch (ev->type)
        {
            case TB_EVENT_KEY:
                {
                    if (ev->key == TB_KEY_CTRL_C)
                    {
                        texu_cio_sigint(cio);
                        return -1;
                    }
                    return (ev->key > 0 ? ev->key : ev->ch);
                }
            case TB_EVENT_RESIZE:
            case TB_EVENT_MOUSE:
                break;
        }
    }
    return -1;
}

void texu_tb2_gotoyx(int y, int x)
{
    tb_set_cursor(x, y);
}

void texu_tb2_clear()
{
    tb_clear();
}

int texu_tb2_putch_attrs(int y, int x, char ch, unsigned short fg, unsigned short bg, unsigned short attrs)
{
    tb2_ui32 uni;
    struct tb_cell cell = { ch, fg, bg };
    char str[4] = { ch, 0, 0, 0 };
    tb_utf8_char_to_unicode(&uni, str);

    tb_get_cell(x, y, &cell);

    if((unsigned short)(-1) == fg) fg = cell.fg;
    if((unsigned short)(-1) == bg) bg = cell.bg;
    tb_change_cell(x, y, uni, fg|attrs, bg|attrs);
    return 0;
}

int texu_tb2_putch(int y, int x, char ch, unsigned short fg, unsigned short bg)
{
    return texu_tb2_putch_attrs(y, x, ch, fg, bg, 0);
}


int texu_tb2_putstr(int y, int x, const char* str, unsigned short fg, unsigned short bg)
{
    tb_print(x, y, fg, bg, str);
    return 0;
}


int texu_tb2_putstr_attrs(int y, int x, const char* str, unsigned short fg, unsigned short bg, unsigned short attrs)
{
    tb_print(x, y, fg|attrs, bg|attrs, str);
    return 0;
}

int texu_tb2_refresh()
{
    tb_present();
    return 0;
}

void texu_tb2_getyx(int *y, int *x)
{
    tb_get_cursor(x, y);
}
#ifdef __cplusplus
}
#endif

#endif /*__USE_TERMIOS__*/
