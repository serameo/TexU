#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texu.h"

#define MyWndClass  "MyWndClass"
#define MyWndClass2 "MyWndClass2"
#define MyWndClass3 "MyWndClass3"

#define ID_HELP             (TEXU_WM_USER + 1)
#define ID_ADD              (TEXU_WM_USER + 2)
#define ID_DELETE           (TEXU_WM_USER + 3)
#define ID_FIND             (TEXU_WM_USER + 4)
#define IDC_STATUSBAR       12

texu_i64 MyWndProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64 MyWndProc2(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64 MyWndProc3(texu_wnd*, texu_ui32, texu_i64, texu_i64);

int main()
{
  texu_status rc = 0;
  texu_wnd* wnd = 0;

  rc = TexuStartup();
  if (rc != TEXU_OK)
  {
    printf("cannot initialized environment\n");
    return -1;
  }

  rc = TexuRegisterClass(
         MyWndClass,
         MyWndProc
         );
  rc = TexuRegisterClass(
         MyWndClass2,
         MyWndProc2
         );
  rc = TexuRegisterClass(
         MyWndClass3,
         MyWndProc3
         );
  
  wnd = TexuCreateWindow(
          "Test TexU App",
          MyWndClass,
          0, /* style*/
          0, /* exstyle*/
          0, /* y */
          0, /* x */
          TexuGetMaxY(),
          TexuGetMaxX(),
          0, /* parent */
          1, /* id */
          0  /* user data */
          );
  TexuPushWindow(wnd);
  TexuShowWindow(wnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(wnd);
  
  TexuRun();

  TexuShutdown();
  return 0;
}


void _MyWndProc_OnHelp(texu_wnd* wnd)
{
  texu_wnd* ctl = texu_wnd_find_child(wnd, IDC_STATUSBAR);
  texu_wnd_set_text(ctl, "Pressed F1 to help");
}

void _MyWndProc_OnAdd(texu_wnd* wnd)
{
  texu_wnd* ctl = texu_wnd_find_child(wnd, IDC_STATUSBAR);
  texu_wnd* newwnd = 0;
  
  texu_wnd_set_text(ctl, "Pressed F2 to add");
  
  newwnd = TexuCreateWindow(
          "Test TexU App - window 2",
          MyWndClass2,
          0, /* style*/
          0, /* exstyle*/
          0, /* y */
          0, /* x */
          TexuGetMaxY(),
          TexuGetMaxX(),
          0, /* parent */
          2, /* id */
          0  /* user data */
          );
  TexuPushWindow(newwnd);
  TexuShowWindow(newwnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(newwnd);
}

void _MyWndProc_OnExit(texu_wnd* wnd)
{
  TexuExit();
}

void _MyWndProc_OnNotify(texu_wnd* wnd, texu_wnd_notify* notify)
{
  texu_lbwnd_notify* lbntf = (texu_lbwnd_notify*)notify;
  texu_wnd* status = texu_wnd_find_child(wnd, IDC_STATUSBAR);
  texu_wnd* lb = notify->wnd;
  texu_char text[TEXU_MAX_WNDTEXT+1];
  
  memset(text, 0, sizeof(text));
  texu_wnd_send_msg(lb, TEXU_LBM_GETITEMTEXT, (texu_i64)lbntf->index, (texu_i64)text);
  texu_wnd_set_text(status, text);
}

void
_MyWndProc_OnPaint(texu_wnd* wnd, texu_cio* dc)
{
  texu_rect rect = { 9, 0, 4, 100 };
  /*texu_cio_draw_rect(dc, &rect, COLOR_PAIR(TEXU_CIO_BRIGHT_WHITE_BLUE));*/
  texu_i32 widths[4] = { 10, 20, 30, 40 };

  texu_rect rect2 = { 14, 0, 10, 100 };
  texu_i32 heights[2] = { 4, 4 };
  
  texu_cio_draw_hrects(dc, &rect, widths, 4, 
    texu_cio_get_color(dc, TEXU_CIO_BRIGHT_WHITE_BLUE));
    
  texu_cio_draw_vrects(dc, &rect2, heights, 2, 
    texu_cio_get_color(dc, TEXU_CIO_BRIGHT_WHITE_YELLOW));
}

texu_i64
MyWndProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_PAINT:
    {
      _MyWndProc_OnPaint(wnd, (texu_cio*)param1);
      return 0;
    }
    
    case TEXU_WM_NOTIFY:
    {
      _MyWndProc_OnNotify(wnd, (texu_wnd_notify*)param1);
      break;
    }
    
    case TEXU_WM_COMMAND:
    {
      switch (param1)
      {
        case ID_DELETE:
          _MyWndProc_OnExit(wnd);
          break;
        case ID_HELP:
          _MyWndProc_OnHelp(wnd);
          break;
        case ID_ADD:
          _MyWndProc_OnAdd(wnd);
          break;
        default:
          break;
      }
      break;
    }

    case TEXU_WM_CREATE:
    {
      texu_wnd* child = 0;
      child = TexuCreateWindow(
              "Text:",
              TEXU_LABEL_CLASS,
              TEXU_WS_RIGHT, /* style*/
              0, /* exstyle*/
              0, /* y */
              0, /* x */
              1,
              20,
              wnd, /* parent */
              1, /* id */
              0  /* user data */
              );
      child = TexuCreateWindow(
              "Text",
              TEXU_EDIT_CLASS,
              TEXU_ES_AUTOHSCROLL|TEXU_ES_A2Z, /* style*/
              0, /* exstyle*/
              0, /* y */
              21, /* x */
              1,
              10,
              wnd, /* parent */
              2, /* id */
              0  /* user data */
              );
      texu_wnd_set_color(child, TEXU_CIO_BRIGHT_WHITE_BLUE, TEXU_CIO_BRIGHT_WHITE_BLUE);
      
      child = TexuCreateWindow(
              "Number:",
              TEXU_LABEL_CLASS,
              TEXU_WS_CENTER, /* style*/
              0, /* exstyle*/
              1, /* y */
              0, /* x */
              1,
              20,
              wnd, /* parent */
              3, /* id */
              0  /* user data */
              );
      child = TexuCreateWindow(
              "123412341234234",
              TEXU_EDIT_CLASS,
              TEXU_ES_AUTOHSCROLL|TEXU_ES_NUMBER, /* style*/
              0, /* exstyle*/
              1, /* y */
              21, /* x */
              1,
              10,
              wnd, /* parent */
              4, /* id */
              0  /* user data */
              );
      child = TexuCreateWindow(
              "Decimal:",
              TEXU_LABEL_CLASS,
              0, /* style*/
              0, /* exstyle*/
              2, /* y */
              0, /* x */
              1,
              10,
              wnd, /* parent */
              5, /* id */
              0  /* user data */
              );
      child = TexuCreateWindow(
              "1.234",
              TEXU_EDIT_CLASS,
              TEXU_ES_AUTOHSCROLL|TEXU_ES_DECIMAL, /* style*/
              0, /* exstyle*/
              2, /* y */
              21, /* x */
              1,  /* height */
              10, /* width */
              wnd, /* parent */
              6, /* id */
              0  /* user data */
              );
      child = TexuCreateWindow(
              "1.234",
              TEXU_LISTBOX_CLASS,
              0, /* style*/
              0, /* exstyle*/
              3, /* y */
              0, /* x */
              5,  /* height */
              20, /* width */
              wnd, /* parent */
              7, /* id */
              0  /* user data */
              );
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
      
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)2, (texu_i64)TEXU_FALSE);
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)3, (texu_i64)TEXU_FALSE);
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)5, (texu_i64)TEXU_FALSE);
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)7, (texu_i64)TEXU_FALSE);
      
      child = TexuCreateWindow(
              "1.234",
              TEXU_LISTBOX_CLASS,
              TEXU_LBS_RADIOBOX, /* style*/
              0, /* exstyle*/
              3, /* y */
              21, /* x */
              5,  /* height */
              20, /* width */
              wnd, /* parent */
              8, /* id */
              0  /* user data */
              );
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
      
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)2, (texu_i64)TEXU_FALSE);
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)3, (texu_i64)TEXU_FALSE);
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)5, (texu_i64)TEXU_FALSE);
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)7, (texu_i64)TEXU_FALSE);
      
      child = TexuCreateWindow(
              "1.234",
              TEXU_LISTBOX_CLASS,
              TEXU_LBS_CHECKBOX, /* style*/
              0, /* exstyle*/
              3, /* y */
              42, /* x */
              5,  /* height */
              20, /* width */
              wnd, /* parent */
              9, /* id */
              0  /* user data */
              );
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
      
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)2, (texu_i64)TEXU_FALSE);
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)3, (texu_i64)TEXU_FALSE);
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)5, (texu_i64)TEXU_FALSE);
      texu_wnd_send_msg(child, TEXU_LBM_ENABLEITEM, (texu_i64)7, (texu_i64)TEXU_FALSE);
      
      child = TexuCreateWindow(
              "F1 - Help",
              TEXU_LABEL_CLASS,
              TEXU_WS_CENTER, /* style*/
              0, /* exstyle*/
              23, /* y */
              0, /* x */
              1,  /* height */
              16, /* width */
              wnd, /* parent */
              10, /* id */
              0  /* user data */
              );
      texu_wnd_set_color(child, TEXU_CIO_COLOR_YELLOW_BLUE, TEXU_CIO_COLOR_YELLOW_BLUE);
      child = TexuCreateWindow(
              "F2 - Add",
              TEXU_LABEL_CLASS,
              TEXU_WS_CENTER, /* style*/
              0, /* exstyle*/
              23, /* y */
              20, /* x */
              1,  /* height */
              16, /* width */
              wnd, /* parent */
              11, /* id */
              0  /* user data */
              );
      texu_wnd_set_color(child, TEXU_CIO_COLOR_YELLOW_BLUE, TEXU_CIO_COLOR_YELLOW_BLUE);
      
      
      child = TexuCreateWindow(
              "F3 - Exit",
              TEXU_LABEL_CLASS,
              TEXU_WS_CENTER, /* style*/
              0, /* exstyle*/
              23, /* y */
              40, /* x */
              1,  /* height */
              16, /* width */
              wnd, /* parent */
              13, /* id */
              0  /* user data */
              );
      texu_wnd_set_color(child, TEXU_CIO_COLOR_WHITE_RED, TEXU_CIO_COLOR_WHITE_RED);
      
      texu_wnd_add_keycmd(wnd, KEY_F(1), ID_HELP);
      texu_wnd_add_keycmd(wnd, KEY_F(2), ID_ADD);
      texu_wnd_add_keycmd(wnd, KEY_F(3), ID_DELETE);

      child = TexuCreateWindow(
              "",
              TEXU_LABEL_CLASS,
              0, /* style*/
              0, /* exstyle*/
              24, /* y */
              0, /* x */
              1,  /* height */
              100, /* width */
              wnd, /* parent */
              IDC_STATUSBAR, /* id */
              0  /* user data */
              );
      texu_wnd_set_color(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
      
      return TEXU_OK;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}



void _MyWndProc2_OnAdd(texu_wnd* wnd)
{
  texu_wnd* ctl = texu_wnd_find_child(wnd, IDC_STATUSBAR);
  texu_wnd* newwnd = 0;
  
  texu_wnd_set_text(ctl, "Pressed F2 to add");
  
  newwnd = TexuCreateWindow(
          "Test TexU App - window 3",
          MyWndClass3,
          0, /* style*/
          0, /* exstyle*/
          0, /* y */
          0, /* x */
          TexuGetMaxY(),
          TexuGetMaxX(),
          0, /* parent */
          3, /* id */
          0  /* user data */
          );
  TexuPushWindow(newwnd);
  TexuShowWindow(newwnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(newwnd);
}

void _MyWndProc2_OnHelp(texu_wnd* wnd)
{
  texu_wnd* ctl = texu_wnd_find_child(wnd, IDC_STATUSBAR);
  texu_wnd_set_text(ctl, "Pressed F12 to exit");
}

void _MyWndProc2_OnExit(texu_wnd* wnd)
{
  texu_wnd* topwnd = 0;
  TexuPopWindow();
  TexuDestroyWindow(wnd);
  
  topwnd = TexuTopWindow();
  TexuShowWindow(topwnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(topwnd);
}


texu_i64
MyWndProc2(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_COMMAND:
    {
      switch (param1)
      {
        case ID_DELETE:
          _MyWndProc2_OnExit(wnd);
          break;
          
        case ID_ADD:
          _MyWndProc2_OnAdd(wnd);
          break;
          
        case ID_HELP:
          _MyWndProc2_OnHelp(wnd);
          break;
      }
      break;
    }

    case TEXU_WM_CREATE:
    {
      texu_wnd* child = 0;
      child = TexuCreateWindow(
              "Text:",
              TEXU_LABEL_CLASS,
              TEXU_WS_RIGHT, /* style*/
              0, /* exstyle*/
              0, /* y */
              0, /* x */
              1,
              20,
              wnd, /* parent */
              1, /* id */
              0  /* user data */
              );
      child = TexuCreateWindow(
              "Text",
              TEXU_EDIT_CLASS,
              TEXU_ES_AUTOHSCROLL|TEXU_ES_A2Z, /* style*/
              0, /* exstyle*/
              0, /* y */
              21, /* x */
              1,
              10,
              wnd, /* parent */
              2, /* id */
              0  /* user data */
              );
      texu_wnd_enable(child, TEXU_FALSE);
      
      child = TexuCreateWindow(
              "Number:",
              TEXU_LABEL_CLASS,
              TEXU_WS_CENTER, /* style*/
              0, /* exstyle*/
              1, /* y */
              0, /* x */
              1,
              20,
              wnd, /* parent */
              3, /* id */
              0  /* user data */
              );
      child = TexuCreateWindow(
              "123412341234234",
              TEXU_EDIT_CLASS,
              TEXU_ES_AUTOHSCROLL|TEXU_ES_NUMBER, /* style*/
              0, /* exstyle*/
              1, /* y */
              21, /* x */
              1,
              10,
              wnd, /* parent */
              4, /* id */
              0  /* user data */
              );
      child = TexuCreateWindow(
              "Decimal:",
              TEXU_LABEL_CLASS,
              0, /* style*/
              0, /* exstyle*/
              2, /* y */
              0, /* x */
              1,
              10,
              wnd, /* parent */
              5, /* id */
              0  /* user data */
              );
      child = TexuCreateWindow(
              "1.234",
              TEXU_EDIT_CLASS,
              TEXU_ES_AUTOHSCROLL|TEXU_ES_DECIMAL, /* style*/
              0, /* exstyle*/
              2, /* y */
              21, /* x */
              1,  /* height */
              10, /* width */
              wnd, /* parent */
              6, /* id */
              0  /* user data */
              );
      child = TexuCreateWindow(
              "1.234",
              TEXU_LISTBOX_CLASS,
              0, /* style*/
              0, /* exstyle*/
              3, /* y */
              0, /* x */
              5,  /* height */
              20, /* width */
              wnd, /* parent */
              7, /* id */
              0  /* user data */
              );
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
      
      child = TexuCreateWindow(
              "1.234",
              TEXU_LISTBOX_CLASS,
              TEXU_LBS_RADIOBOX, /* style*/
              0, /* exstyle*/
              3, /* y */
              21, /* x */
              5,  /* height */
              20, /* width */
              wnd, /* parent */
              8, /* id */
              0  /* user data */
              );
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
      
      child = TexuCreateWindow(
              "1.234",
              TEXU_LISTBOX_CLASS,
              TEXU_LBS_CHECKBOX, /* style*/
              0, /* exstyle*/
              3, /* y */
              42, /* x */
              5,  /* height */
              20, /* width */
              wnd, /* parent */
              9, /* id */
              0  /* user data */
              );
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
      texu_wnd_send_msg(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
      
      child = TexuCreateWindow(
              "F1 - Help",
              TEXU_LABEL_CLASS,
              TEXU_WS_CENTER, /* style*/
              0, /* exstyle*/
              23, /* y */
              0, /* x */
              1,  /* height */
              16, /* width */
              wnd, /* parent */
              10, /* id */
              0  /* user data */
              );
      texu_wnd_set_color(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
      
      child = TexuCreateWindow(
              "F3 - Exit",
              TEXU_LABEL_CLASS,
              TEXU_WS_CENTER, /* style*/
              0, /* exstyle*/
              23, /* y */
              40, /* x */
              1,  /* height */
              16, /* width */
              wnd, /* parent */
              13, /* id */
              0  /* user data */
              );
      texu_wnd_set_color(child, TEXU_CIO_COLOR_WHITE_RED, TEXU_CIO_COLOR_WHITE_RED);
      
      texu_wnd_add_keycmd(wnd, KEY_F(1), ID_HELP);
      texu_wnd_add_keycmd(wnd, KEY_F(2), ID_ADD);
      texu_wnd_add_keycmd(wnd, KEY_F(3), ID_DELETE);
      
      child = TexuCreateWindow(
              "This is the second window",
              TEXU_LABEL_CLASS,
              0, /* style*/
              0, /* exstyle*/
              24, /* y */
              0, /* x */
              1,  /* height */
              100, /* width */
              wnd, /* parent */
              IDC_STATUSBAR, /* id */
              0  /* user data */
              );
      texu_wnd_set_color(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
      
      return TEXU_OK;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}







void _MyWndProc3_OnExit(texu_wnd* wnd)
{
  texu_wnd* topwnd = 0;
  TexuPopWindow();
  TexuDestroyWindow(wnd);
  
  topwnd = TexuTopWindow();
  TexuShowWindow(topwnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(topwnd);
}

texu_i64
MyWndProc3(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_COMMAND:
    {
      switch (param1)
      {
        case ID_DELETE:
          _MyWndProc3_OnExit(wnd);
          break;
          
      }
      break;
    }

    case TEXU_WM_CREATE:
    {
      texu_wnd_header header;
      texu_wnd_subitem subitem;
      texu_wnd* child = 0;
      
      child = TexuCreateWindow(
              "Text:",
              TEXU_LISTCTRL_CLASS,
              0, /* style*/
              0, /* exstyle*/
              0, /* y */
              0, /* x */
              20,
              120,
              wnd, /* parent */
              1, /* id */
              0  /* user data */
              );
              
      memset(&header, 0, sizeof(header));
      header.caption = "Header 1";
      header.cols = 20;
      header.align = TEXU_ALIGN_LEFT;
      header.normcolor = TEXU_CIO_COLOR_BLUE_YELLOW;
      header.discolor  = TEXU_CIO_COLOR_BLUE_YELLOW;
      header.selcolor  = TEXU_CIO_COLOR_YELLOW_BLUE;
      header.editstyle = TEXU_ES_AUTOHSCROLL;
      header.decwidth  = 2;
      texu_wnd_send_msg(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);
      
      header.caption = "Header 2";
      texu_wnd_send_msg(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);
      
      header.caption = "Header 3";
      texu_wnd_send_msg(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);
      
      header.caption = "Header 4";
      texu_wnd_send_msg(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);
      
      header.caption = "Header 5";
      texu_wnd_send_msg(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);

      header.caption = "Header 6";
      texu_wnd_send_msg(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);

      texu_wnd_send_msg(child, TEXU_LCM_ADDITEM, (texu_i64)"\t1\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      texu_wnd_send_msg(child, TEXU_LCM_ADDITEM, (texu_i64)"\t2\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      texu_wnd_send_msg(child, TEXU_LCM_ADDITEM, (texu_i64)"\t3\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      texu_wnd_send_msg(child, TEXU_LCM_ADDITEM, (texu_i64)"\t4\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      texu_wnd_send_msg(child, TEXU_LCM_ADDITEM, (texu_i64)"\t5\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      texu_wnd_send_msg(child, TEXU_LCM_ADDITEM, (texu_i64)"\t6\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      texu_wnd_send_msg(child, TEXU_LCM_ADDITEM, (texu_i64)"\t7\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      texu_wnd_send_msg(child, TEXU_LCM_ADDITEM, (texu_i64)"\t8\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      texu_wnd_send_msg(child, TEXU_LCM_ADDITEM, (texu_i64)"\t9\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      texu_wnd_send_msg(child, TEXU_LCM_ADDITEM, (texu_i64)"\t10\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      
      subitem.idx = 0;
      subitem.col = 0;
      subitem.normcolor = TEXU_CIO_COLOR_GREEN_WHITE;
      subitem.discolor  = TEXU_CIO_COLOR_GREEN_WHITE;
      subitem.selcolor  = TEXU_CIO_COLOR_WHITE_GREEN;
      
      texu_wnd_send_msg(child, TEXU_LCM_SETITEM, TEXU_LCFM_COLOR, (texu_i64)&subitem);

      subitem.idx = 1;
      subitem.col = 1;
      subitem.normcolor = TEXU_CIO_COLOR_GREEN_MAGENTA;
      subitem.discolor  = TEXU_CIO_COLOR_GREEN_MAGENTA;
      subitem.selcolor  = TEXU_CIO_COLOR_MAGENTA_GREEN;
      
      texu_wnd_send_msg(child, TEXU_LCM_SETITEM, TEXU_LCFM_COLOR, (texu_i64)&subitem);
      

      subitem.idx = 2;
      subitem.col = 2;
      subitem.normcolor = TEXU_CIO_COLOR_RED_YELLOW;
      subitem.discolor  = TEXU_CIO_COLOR_RED_YELLOW;
      subitem.selcolor  = TEXU_CIO_COLOR_YELLOW_RED;
      
      texu_wnd_send_msg(child, TEXU_LCM_SETITEM, TEXU_LCFM_COLOR, (texu_i64)&subitem);
      
      child = TexuCreateWindow(
              "F3 - Exit",
              TEXU_LABEL_CLASS,
              TEXU_WS_CENTER, /* style*/
              0, /* exstyle*/
              23, /* y */
              40, /* x */
              1,  /* height */
              16, /* width */
              wnd, /* parent */
              13, /* id */
              0  /* user data */
              );
      texu_wnd_set_color(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
      
      texu_wnd_add_keycmd(wnd, KEY_F(1), ID_HELP);
      texu_wnd_add_keycmd(wnd, KEY_F(3), ID_DELETE);
      
      child = TexuCreateWindow(
              "This is the third window",
              TEXU_LABEL_CLASS,
              0, /* style*/
              0, /* exstyle*/
              24, /* y */
              0, /* x */
              1,  /* height */
              100, /* width */
              wnd, /* parent */
              IDC_STATUSBAR, /* id */
              0  /* user data */
              );
      texu_wnd_set_color(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
      return TEXU_OK;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}



int main2()
{
  /*texu_env* env = 0;*/
  texu_cio* cio = 0;
  texu_status rc = 0;
  texu_i32 ch = 0;
  texu_char text[80];

  rc = TexuStartup();
  if (rc != TEXU_OK)
  {
    printf("cannot initialized environment\n");
    return -1;
  }

  /*env = TexuGetEnv();*/
  cio = TexuGetDC();
  texu_cio_echo(cio, TEXU_FALSE); 

  texu_cio_putstr(cio, 0, 0, "press F1 to exit");
  texu_cio_gotoyx(cio, 1, 0);

  while (TEXU_TRUE)
  {
    ch = texu_cio_getch(cio);

    memset(text, 0, sizeof(text));
    sprintf(text, "ch=%c, (%d)", ch, ch);
    texu_cio_putstr(cio, 2, 0, text);

    if (ch == KEY_F(1))
    {
      texu_cio_putstr_attr(cio, 24, 0, "exit now", 
        COLOR_PAIR(TEXU_CIO_COLOR_YELLOW_BLUE)|A_BLINK);
      texu_cio_getch(cio);
      break;
    }
  }

  TexuShutdown();
  return 0;
}

