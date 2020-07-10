#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texu.h"

#define MyWndClass  "MyWndClass"
#define MyWndClass2 "MyWndClass2"
#define MyWndClass3 "MyWndClass3"
#define MyWndClass4 "MyWndClass4"
#define MyWndClass5 "MyWndClass5"
#define MyWndClass5_Page1 "MyWndClass5_Page1"
#define MyWndClass5_Page2 "MyWndClass5_Page2"

#define ID_HELP             (TEXU_WM_USER + 1)
#define ID_ADD              (TEXU_WM_USER + 2)
#define ID_DELETE           (TEXU_WM_USER + 3)
#define ID_FIND             (TEXU_WM_USER + 4)
#define ID_MSGBOX           (TEXU_WM_USER + 5)
#define IDC_STATUSBAR       12
#define IDC_UPDOWN          15
#define IDC_PROGRESSBAR     16
#define ID_MAINMENU         1000
/*
#define IDC_LIST1           7
#define IDC_LIST2           8
#define IDC_LIST3           9
*/

texu_i64 MyWndProc(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64 MyWndProc2(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64 MyWndProc3(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64 MyWndProc4(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64 MyWndProc5(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64 MyWndProc5_Page1(texu_wnd*, texu_ui32, texu_i64, texu_i64);
texu_i64 MyWndProc5_Page2(texu_wnd*, texu_ui32, texu_i64, texu_i64);
/*
struct texu_wnd_template
{
  const texu_char* text;
  const texu_char* clsname;
  texu_ui32  style;
  texu_ui32  exstyle;
  texu_i32   y;
  texu_i32   x;
  texu_i32   h;
  texu_i32   w;
  texu_i32   id;
};
*/

#define IDC_LABEL1        200
#define IDC_EDIT1         201
#define IDC_UPDOWN1       202
#define IDC_LABEL2        203
#define IDC_EDIT2         204
#define IDC_PROGRESSBAR1  205
#define IDC_LABEL3        206
#define IDC_EDIT3         207
#define IDC_LIST1         208
#define IDC_LIST2         209
#define IDC_LIST3         210
#define IDC_BUTTON1       211
#define IDC_BUTTON2       212
#define IDC_BUTTON3       213
#define IDC_BUTTON4       214
#define IDC_STATUSBAR1    215
#define IDC_PAGECTRL1     216
#define IDC_PAGE1         217
#define IDC_PAGE2         218
#define IDC_PAGE3         219
#define IDC_PAGE4         220
#define IDC_COMBO1        221

texu_wnd_template templ5[] =
{
  { "Label1:",        TEXU_LABEL_CLASS,       TEXU_WS_RIGHT, 0, 1, 0, 1, 20, IDC_LABEL1 },
  { "Edit1",          TEXU_EDIT_CLASS,        TEXU_ES_AUTOHSCROLL|TEXU_ES_A2Z, 0, 1, 21, 1, 10, IDC_EDIT1 },
  { "0",              TEXU_UPDOWNCTRL_CLASS,  0, 0, 1, 42, 1, 10, IDC_UPDOWN1 },
  { "Label2:",        TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 2, 1, 1, 20, IDC_LABEL2 },
  { "Edit2",          TEXU_EDIT_CLASS,        TEXU_ES_AUTOHSCROLL|TEXU_ES_NUMBER, 0, 2, 21, 1, 10, IDC_EDIT2 },
  { "",               TEXU_PROGRESSBAR_CLASS, 0, 0, 2, 42, 1, 30, IDC_PROGRESSBAR1 },
  { "Label3:",        TEXU_LABEL_CLASS,       TEXU_WS_LEFT, 0, 3, 0, 1, 20, IDC_LABEL3 },
  { "Edit3",          TEXU_EDIT_CLASS,        TEXU_ES_AUTOHSCROLL|TEXU_ES_DECIMAL, 0, 3, 21, 1, 10, IDC_EDIT3 },
  { "",               TEXU_LISTBOX_CLASS,     0, 0, 4, 0, 5, 20, IDC_LIST1 },
  { "",               TEXU_LISTBOX_CLASS,     TEXU_LBS_RADIOBOX, 0, 4, 21, 5, 20, IDC_LIST2 },
  { "",               TEXU_LISTBOX_CLASS,     TEXU_LBS_CHECKBOX, 0, 4, 42, 5, 20, IDC_LIST3 },
  { "F1 - Help",      TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 0, 1, 16, IDC_BUTTON1 },
  { "F2 - Add",       TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 20, 1, 16, IDC_BUTTON2 },
  { "F3 - Exit",      TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 40, 1, 16, IDC_BUTTON3 },
  { "Alt+X - Exit",   TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 60, 1, 16, IDC_BUTTON4 },
  { "",               TEXU_STATUSBAR_CLASS,   0, 0, 24, 0, 1, 100, IDC_STATUSBAR1 },
  /*last control is nil*/
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

texu_wnd_template templ5_p1[] =
{
  { "Label2:",        TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 2, 1, 1, 20, IDC_LABEL2 },
  { "Edit2",          TEXU_EDIT_CLASS,        TEXU_ES_AUTOHSCROLL|TEXU_ES_NUMBER, 0, 2, 21, 1, 10, IDC_EDIT2 },
  { "",               TEXU_PROGRESSBAR_CLASS, 0, 0, 2, 42, 1, 30, IDC_PROGRESSBAR1 },
  { "Label3:",        TEXU_LABEL_CLASS,       TEXU_WS_LEFT, 0, 3, 0, 1, 20, IDC_LABEL3 },
  { "Edit3",          TEXU_EDIT_CLASS,        TEXU_ES_AUTOHSCROLL|TEXU_ES_DECIMAL, 0, 3, 21, 1, 10, IDC_EDIT3 },
  { "",               TEXU_LISTBOX_CLASS,     0, 0, 4, 0, 5, 20, IDC_LIST1 },
  { "",               TEXU_LISTBOX_CLASS,     TEXU_LBS_RADIOBOX, 0, 4, 21, 5, 20, IDC_LIST2 },
  { "",               TEXU_LISTBOX_CLASS,     TEXU_LBS_CHECKBOX, 0, 4, 42, 5, 20, IDC_LIST3 },
  { "F1 - Help",      TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 0, 1, 16, IDC_BUTTON1 },
  { "F2 - Add",       TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 20, 1, 16, IDC_BUTTON2 },
  { "F3 - Exit",      TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 40, 1, 16, IDC_BUTTON3 },
  { "Alt+X - Exit",   TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 60, 1, 16, IDC_BUTTON4 },
  { "",               TEXU_STATUSBAR_CLASS,   0, 0, 24, 0, 1, 100, IDC_STATUSBAR1 },
  /*last control is nil*/
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

texu_wnd_template templ5_p2[] =
{
  { "Label1:",        TEXU_LABEL_CLASS,       TEXU_WS_RIGHT, 0, 1, 0, 1, 20, IDC_LABEL1 },
  { "Edit1",          TEXU_EDIT_CLASS,        TEXU_ES_AUTOHSCROLL|TEXU_ES_A2Z, 0, 1, 21, 1, 10, IDC_EDIT1 },
  { "0",              TEXU_UPDOWNCTRL_CLASS,  0, 0, 1, 42, 1, 10, IDC_UPDOWN1 },
  { "Label2:",        TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 2, 1, 1, 20, IDC_LABEL2 },
  { "Edit2",          TEXU_EDIT_CLASS,        TEXU_ES_AUTOHSCROLL|TEXU_ES_NUMBER, 0, 2, 21, 1, 10, IDC_EDIT2 },
  { "",               TEXU_PROGRESSBAR_CLASS, 0, 0, 2, 42, 1, 30, IDC_PROGRESSBAR1 },
  { "Label3:",        TEXU_LABEL_CLASS,       TEXU_WS_LEFT, 0, 3, 0, 1, 20, IDC_LABEL3 },
  { "Edit3",          TEXU_EDIT_CLASS,        TEXU_ES_AUTOHSCROLL|TEXU_ES_DECIMAL, 0, 3, 21, 1, 10, IDC_EDIT3 },
  { "F1 - Help",      TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 0, 1, 16, IDC_BUTTON1 },
  { "F2 - Add",       TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 20, 1, 16, IDC_BUTTON2 },
  { "F3 - Exit",      TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 40, 1, 16, IDC_BUTTON3 },
  { "Alt+X - Exit",   TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 23, 60, 1, 16, IDC_BUTTON4 },
  { "",               TEXU_STATUSBAR_CLASS,   0, 0, 24, 0, 1, 100, IDC_STATUSBAR1 },
  /*last control is nil*/
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};



texu_i32
SetMyThemeColor(texu_i32 sysidx)
{
  switch (sysidx)
  {
    /*
    case TEXU_COLOR_WINDOW:                   return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_DESKTOP:                  return TEXU_CIO_COLOR_WHITE_BLACK;  
    case TEXU_COLOR_DIALOG:                   return TEXU_CIO_COLOR_BLACK_WHITE;
    case TEXU_COLOR_TITLE_WINDOW:             return TEXU_CIO_COLOR_WHITE_BLUE;
    case TEXU_COLOR_BUTTON_OK:                return TEXU_CIO_COLOR_BLACK_GREEN;
    case TEXU_COLOR_BUTTON_YES:               return TEXU_CIO_COLOR_WHITE_BLUE;
    case TEXU_COLOR_BUTTON_NO:                return TEXU_CIO_COLOR_BLACK_RED;
    case TEXU_COLOR_BUTTON_CANCEL:            return TEXU_CIO_COLOR_BLACK_YELLOW;
    */
    case TEXU_COLOR_MENU:                     return TEXU_CIO_COLOR_RED_WHITE;
    case TEXU_COLOR_MENU_DISABLED:            return TEXU_CIO_COLOR_BLACK_WHITE;
    case TEXU_COLOR_MENU_SELECTED:            return TEXU_CIO_COLOR_RED_CYAN;
    case TEXU_COLOR_MENUITEM:                 return TEXU_CIO_COLOR_RED_WHITE;
    case TEXU_COLOR_MENUITEM_DISABLED:        return TEXU_CIO_COLOR_BLACK_WHITE;
    case TEXU_COLOR_MENUITEM_SELECTED:        return TEXU_CIO_COLOR_WHITE_RED;
    /*
    case TEXU_COLOR_LABEL:                    return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_LABEL_DISABLED:           return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_STATUSBAR:                return TEXU_CIO_COLOR_WHITE_BLUE;
    case TEXU_COLOR_EDIT:                     return TEXU_CIO_COLOR_CYAN_BLACK;
    case TEXU_COLOR_EDIT_DISABLED:            return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_EDIT_SELECTED:            return TEXU_CIO_COLOR_BLACK_CYAN;
    case TEXU_COLOR_LISTBOX:                  return TEXU_CIO_COLOR_CYAN_BLACK;
    case TEXU_COLOR_LISTBOX_DISABLED:         return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_LISTBOX_SELECTED:         return TEXU_CIO_COLOR_BLUE_CYAN;
    case TEXU_COLOR_COMBOBOX:                 return TEXU_CIO_COLOR_BLUE_WHITE;
    case TEXU_COLOR_COMBOBOX_DISABLED:        return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_COMBOBOX_SELECTED:        return TEXU_CIO_COLOR_WHITE_BLUE;
    case TEXU_COLOR_LISTCTRL:                 return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_LISTCTRL_DISABLED:        return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_LISTCTRL_SELECTED:        return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_LISTCTRL_ITEM:            return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_LISTCTRL_ITEM_DISABLED:   return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_LISTCTRL_ITEM_SELECTED:   return TEXU_CIO_COLOR_BLUE_CYAN;
    case TEXU_COLOR_TREECTRL:                 return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_TREECTRL_DISABLED:        return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_TREECTRL_SELECTED:        return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_TREECTRL_ITEM:            return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_TREECTRL_ITEM_DISABLED:   return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_TREECTRL_ITEM_SELECTED:   return TEXU_CIO_COLOR_BLUE_CYAN;
    case TEXU_COLOR_UPDOWNCTRL:               return TEXU_CIO_COLOR_BLUE_WHITE;
    case TEXU_COLOR_UPDOWNCTRL_DISABLED:      return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_UPDOWNCTRL_SELECTED:      return TEXU_CIO_COLOR_BLUE_CYAN;
    case TEXU_COLOR_PROGRESSBAR:              return TEXU_CIO_COLOR_CYAN_BLACK;
    case TEXU_COLOR_PROGRESSBAR_DISABLED:     return TEXU_CIO_COLOR_WHITE_BLACK;
    case TEXU_COLOR_PROGRESSBAR_SELECTED:     return TEXU_CIO_COLOR_BLUE_CYAN;
    case TEXU_COLOR_DEFAULT:                  return TEXU_CIO_COLOR_WHITE_BLACK;
    */
  }
  return -1;
}


int main()
{
  texu_status rc = 0;
  texu_wnd* wnd = 0;

  rc = TexuStartup(25, 100);/*25 lines x 100 characters per line*/
  if (rc != TEXU_OK)
  {
    printf("cannot initialized environment\n");
    return -1;
  }
  
  TexuSetThemeColor(SetMyThemeColor);

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
  rc = TexuRegisterClass(
         MyWndClass4,
         MyWndProc4
         );

  rc = TexuRegisterClass(
         MyWndClass5,
         MyWndProc5
         );
  rc = TexuRegisterClass(
         MyWndClass5_Page1,
         MyWndProc5_Page1
         );
  rc = TexuRegisterClass(
         MyWndClass5_Page2,
         MyWndProc5_Page2
         );

  wnd = TexuCreateWindow(
          "Test TexU App",
          MyWndClass5,
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
  
  TexuShowWindow(wnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(wnd);
  
  TexuRun();

  TexuShutdown();
  return 0;
}


void _MyWndProc_OnHelp(texu_wnd* wnd)
{
  texu_wnd* ctl = TexuGetWindowItem(wnd, IDC_STATUSBAR);
  texu_wnd* newwnd = 0;

  TexuSetWindowText(ctl, "Pressed F1 to help");
  
  newwnd = TexuCreateWindow(
              "Test TexU App - window 2",
              MyWndClass5,
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
  
  TexuShowWindow(newwnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(newwnd);
}

void
_MyWndProc_OnMsgBox(texu_wnd* wnd)
{
  TexuMessageBox(
    "Hello world",
    "Do you want to update user?",
    wnd,
    1001, /* id */
    TEXU_MBS_OKCANCEL,
    0  /* user data */
    );
}

void _MyWndProc_OnAdd_test(texu_wnd* wnd)
{
  texu_wnd* ctl = TexuGetWindowItem(wnd, IDC_STATUSBAR);
  texu_wnd* newwnd = 0;
  
  TexuSetWindowText(ctl, "Pressed F2 to add");
  
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
  texu_char text[TEXU_MAX_WNDTEXT+1];
  texu_wnd* status = TexuGetWindowItem(wnd, IDC_STATUSBAR);
  texu_wnd* lb = TexuGetWindowItem(wnd, IDC_LIST1);
  texu_wnd* udctl = TexuGetWindowItem(wnd, IDC_UPDOWN);
  texu_wnd* pgb = TexuGetWindowItem(wnd, IDC_PROGRESSBAR);
  texu_i32 val = 0;
  texu_msgbox_notify* msg = 0;
  texu_menuitem_notify* menuitem = 0;
  
  memset(text, 0, sizeof(text));
  if (notify->id == IDC_LIST1)
  {
    TexuSendMessage(lb, TEXU_LBM_GETITEMTEXT, (texu_i64)lbntf->index, (texu_i64)text);
    TexuSetWindowText(status, text);
  }
  
  if (notify->id == IDC_UPDOWN)
  {
    val = TexuSendMessage(udctl, TEXU_UDCM_GETINT, 0, 0);
    /*TexuSaveCursorPosition(udctl);*/
    TexuSendMessage(pgb, TEXU_PGBM_SETPOS, val, 0);
    /*TexuRestoreCursorPosition(udctl);*/
    
    TexuSendMessage(pgb, TEXU_WM_GETTEXT, (texu_i64)text, TEXU_MAX_WNDTEXT);
    TexuSendMessage(status, TEXU_SBM_SETTEXT, 1, (texu_i64)text);
  }
  
  if (notify->code == TEXU_MBN_ENDDIALOG)
  {
    msg = (texu_msgbox_notify*)notify;
    val = msg->id;
    
    switch (val)
    {
      case TEXU_IDOK:
        strcpy(text, "PRESSED OK");
        break;
      case TEXU_IDCANCEL:
        strcpy(text, "PRESSED CANCEL");
        break;
      case TEXU_IDYES:
        strcpy(text, "PRESSED YES");
        break;
      case TEXU_IDNO:
        strcpy(text, "PRESSED NO");
        break;
    }
    TexuSetWindowText(status, text);
  }
  
  if (notify->code == TEXU_MNN_ITEMCHANGED)
  {
    menuitem = (texu_menuitem_notify*)notify;
    strcpy(text, menuitem->info);
    TexuSetWindowText(status, text);
  }
}

void
_MyWndProc_OnPaint(texu_wnd* wnd, texu_cio* dc)
{
  texu_rect rect = { 9, 0, 4, 99 };
  texu_i32 widths[4] = { 10, 20, 30, 39 };

  texu_rect rect2 = { 14, 0, 10, 99 };
  texu_i32 heights[2] = { 4, 4 };
  
  TexuDrawHRects(dc, &rect, widths, 4, 
    TexuGetColor(dc, TEXU_CIO_COLOR_WHITE_BLUE));
    
  TexuDrawVRects(dc, &rect2, heights, 2, 
    TexuGetColor(dc, TEXU_CIO_COLOR_WHITE_YELLOW));
}

texu_status _MyWndProc_OnCreate1(texu_wnd* wnd)
{
  /* to create a simple control */
  TexuCreateControls(wnd, templ5, 50);
  return TEXU_OK;
}


texu_menu_template mainmenu1[] = 
{
  {/*File*/
    " File ", TEXU_TRUE, "File", 4,
    {
      { " New          ", ID_ADD,     TEXU_TRUE,  "New window" },
      { " Open         ", 2,          TEXU_TRUE,  "Open window" },
      { "--------------", 0,          TEXU_FALSE, "" },
      { " Exit   Alt+X ", ID_DELETE,  TEXU_TRUE,  "Exit a program" }
    }
  },
  {/*Edit*/
    " Edit ", TEXU_TRUE, "Edit", 3,
    {
      { " Cut          ", 11,         TEXU_TRUE,  "Cut selection" },
      { " Copy         ", 12,         TEXU_TRUE,  "Copy selection" },
      { " Paste        ", 13,         TEXU_TRUE,  "Paste selection copied" }
    }
  },
  {/*Search*/
    " Search ", TEXU_FALSE, "Search", 2, 
    {
      { " Find         ", 21,         TEXU_TRUE,  "Find text" },
      { " Find Next    ", 22,         TEXU_TRUE,  "Find next text" }
    }
  },
  {/*View*/
    " View ", TEXU_TRUE, "View", 3,
    {
      { " Help      F1 ", ID_HELP,    TEXU_TRUE , "" },
      { "--------------", 0,          TEXU_FALSE, "" },
      { " About        ", ID_MSGBOX,  TEXU_TRUE , "About test_texu demo" }
    }
  }
};

texu_status _MyWndProc_OnCreate(texu_wnd* wnd)
{
  texu_wnd* child = 0;
  texu_i32 starty = 1;
  texu_menu* menu = 0;
  texu_popup_menu *item = 0;
  
  menu = TexuCreateMenus(wnd, ID_MAINMENU, mainmenu1, 4);
#if 0
  menu = TexuCreateMenu(wnd, ID_MAINMENU);
  item = TexuAddPopupMenu(menu, " File ", TEXU_TRUE, "File");
  TexuAddPopupMenuItem(menu, item, " New          ", ID_ADD,     TEXU_TRUE,  "New window");
  TexuAddPopupMenuItem(menu, item, " Open         ", 2,          TEXU_TRUE,  "Open window");
  TexuAddPopupMenuItem(menu, item, "--------------", -1,         TEXU_FALSE, "");
  TexuAddPopupMenuItem(menu, item, " Exit   Alt+X ", ID_DELETE,  TEXU_TRUE,  "Exit a program");

  item = TexuAddPopupMenu(menu, " Edit ", TEXU_TRUE, "Edit");
  TexuAddPopupMenuItem(menu, item, " Cut          ", 11,         TEXU_TRUE,  "Cut selection");
  TexuAddPopupMenuItem(menu, item, " Copy         ", 12,         TEXU_TRUE,  "Copy selection");
  TexuAddPopupMenuItem(menu, item, " Paste        ", 13,         TEXU_TRUE,  "Paste selection copied");
  
  item = TexuAddPopupMenu(menu, " Search ", TEXU_TRUE, "Search");
  TexuAddPopupMenuItem(menu, item, " Find         ", 21,         TEXU_TRUE,  "Find text");
  TexuAddPopupMenuItem(menu, item, " Find Next    ", 22,         TEXU_TRUE,  "Find next text");
  TexuEnablePopupMenu(menu, 2, TEXU_FALSE);
  
  item = TexuAddPopupMenu(menu, " View ", TEXU_TRUE, "View");
  TexuAddPopupMenuItem(menu, item, " Help      F1 ", ID_HELP,    TEXU_TRUE , "");
  TexuAddPopupMenuItem(menu, item, "--------------", -1,         TEXU_FALSE, "");
  TexuAddPopupMenuItem(menu, item, " About        ", ID_MSGBOX,  TEXU_TRUE , "About test_texu demo");
#endif
  child = TexuCreateWindow(
          "Text:",
          TEXU_LABEL_CLASS,
          TEXU_WS_RIGHT, /* style*/
          0, /* exstyle*/
          starty, /* y */
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
          starty, /* y */
          21, /* x */
          1,
          10,
          wnd, /* parent */
          2, /* id */
          0  /* user data */
          );
  TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

  child = TexuCreateWindow(
          "Text",
          TEXU_UPDOWNCTRL_CLASS,
          0, /* style*/
          0, /* exstyle*/
          starty, /* y */
          42, /* x */
          1,
          10,
          wnd, /* parent */
          IDC_UPDOWN, /* id */
          0  /* user data */
          );
  TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
  
  child = TexuCreateWindow(
          "Number:",
          TEXU_LABEL_CLASS,
          TEXU_WS_CENTER, /* style*/
          0, /* exstyle*/
          starty+1, /* y */
          0, /* x */
          1,
          20,
          wnd, /* parent */
          3, /* id */
          0  /* user data */
          );
  child = TexuCreateWindow(
          "1234567890",
          TEXU_EDIT_CLASS,
          TEXU_ES_AUTOHSCROLL|TEXU_ES_NUMBER, /* style*/
          0, /* exstyle*/
          starty+1, /* y */
          21, /* x */
          1,
          10,
          wnd, /* parent */
          4, /* id */
          0  /* user data */
          );
          

  child = TexuCreateWindow(
          "",
          TEXU_PROGRESSBAR_CLASS,
          0, /* style*/
          0, /* exstyle*/
          starty+1, /* y */
          42, /* x */
          1,
          30,
          wnd, /* parent */
          IDC_PROGRESSBAR, /* id */
          0  /* user data */
          );
  TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
  TexuSendMessage(child, TEXU_PGBM_SETMAX, 255, 0);


  child = TexuCreateWindow(
          "Decimal:",
          TEXU_LABEL_CLASS,
          0, /* style*/
          0, /* exstyle*/
          starty+2, /* y */
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
          starty+2, /* y */
          21, /* x */
          1,  /* height */
          10, /* width */
          wnd, /* parent */
          6, /* id */
          0  /* user data */
          );

  child = TexuCreateWindow(
          "combo1",
          TEXU_COMBOBOX_CLASS,
          0, /* style*/
          0, /* exstyle*/
          starty+2, /* y */
          42, /* x */
          1,  /* height */
          20, /* width */
          wnd, /* parent */
          IDC_COMBO1, /* id */
          0  /* user data */
          );
  TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_i64)"combo1", 0);
  TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_i64)"combo2", 0);
  TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_i64)"combo3", 0);
  TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_i64)"combo4", 0);
  /*
  TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_i64)"combo5", 0);
  TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_i64)"combo6", 0);
  TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_i64)"combo7", 0);
  TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_i64)"combo8", 0);
  TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_i64)"combo9", 0);
  TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_i64)"comboA", 0);
  */
  child = (texu_wnd*)TexuSendMessage(child, TEXU_CBM_GETLISTBOX, 0, 0);
  TexuSetColor(child, TEXU_CIO_COLOR_BLUE_WHITE, TEXU_CIO_COLOR_BLUE_WHITE);
  TexuSendMessage(child, TEXU_LBM_SETSELCOLOR, TEXU_CIO_COLOR_BLUE_CYAN, 0);

  child = TexuCreateWindow(
          "",
          TEXU_LISTBOX_CLASS,
          0, /* style*/
          0, /* exstyle*/
          starty+3, /* y */
          0, /* x */
          5,  /* height */
          20, /* width */
          wnd, /* parent */
          IDC_LIST1, /* id */
          0  /* user data */
          );
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);

  TexuSetColor(child, TEXU_CIO_COLOR_BLUE_WHITE, TEXU_CIO_COLOR_BLACK_WHITE);
  TexuSendMessage(child, TEXU_LBM_SETSELCOLOR, TEXU_CIO_COLOR_BLUE_CYAN, 0);
  
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)2, (texu_i64)TEXU_FALSE);
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)3, (texu_i64)TEXU_FALSE);
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)5, (texu_i64)TEXU_FALSE);
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)7, (texu_i64)TEXU_FALSE);
  
  child = TexuCreateWindow(
          "1.234",
          TEXU_LISTBOX_CLASS,
          TEXU_LBS_RADIOBOX, /* style*/
          0, /* exstyle*/
          starty+3, /* y */
          21, /* x */
          5,  /* height */
          20, /* width */
          wnd, /* parent */
          8, /* id */
          0  /* user data */
          );
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
  
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)2, (texu_i64)TEXU_FALSE);
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)3, (texu_i64)TEXU_FALSE);
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)5, (texu_i64)TEXU_FALSE);
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)7, (texu_i64)TEXU_FALSE);
  
  child = TexuCreateWindow(
          "1.234",
          TEXU_LISTBOX_CLASS,
          TEXU_LBS_CHECKBOX, /* style*/
          0, /* exstyle*/
          starty+3, /* y */
          42, /* x */
          5,  /* height */
          20, /* width */
          wnd, /* parent */
          9, /* id */
          0  /* user data */
          );
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
  TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
  
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)2, (texu_i64)TEXU_FALSE);
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)3, (texu_i64)TEXU_FALSE);
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)5, (texu_i64)TEXU_FALSE);
  TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_i64)7, (texu_i64)TEXU_FALSE);
  
  child = TexuCreateWindow(
          "F1 - Help",
          TEXU_LABEL_CLASS,
          TEXU_WS_CENTER, /* style*/
          0, /* exstyle*/
          starty+22, /* y */
          0, /* x */
          1,  /* height */
          16, /* width */
          wnd, /* parent */
          10, /* id */
          0  /* user data */
          );
  TexuSetColor(child, TEXU_CIO_COLOR_YELLOW_BLUE, TEXU_CIO_COLOR_YELLOW_BLUE);
  child = TexuCreateWindow(
          "F2 - Add",
          TEXU_LABEL_CLASS,
          TEXU_WS_CENTER, /* style*/
          0, /* exstyle*/
          starty+22, /* y */
          20, /* x */
          1,  /* height */
          16, /* width */
          wnd, /* parent */
          11, /* id */
          0  /* user data */
          );
  TexuSetColor(child, TEXU_CIO_COLOR_YELLOW_BLUE, TEXU_CIO_COLOR_YELLOW_BLUE);
  
  
  child = TexuCreateWindow(
          "F3 - Exit",
          TEXU_LABEL_CLASS,
          TEXU_WS_CENTER, /* style*/
          0, /* exstyle*/
          starty+22, /* y */
          40, /* x */
          1,  /* height */
          16, /* width */
          wnd, /* parent */
          13, /* id */
          0  /* user data */
          );
  TexuSetColor(child, TEXU_CIO_COLOR_WHITE_RED, TEXU_CIO_COLOR_WHITE_RED);
  
  
  child = TexuCreateWindow(
          "Alt + X - Exit",
          TEXU_LABEL_CLASS,
          TEXU_WS_CENTER, /* style*/
          0, /* exstyle*/
          starty+22, /* y */
          60, /* x */
          1,  /* height */
          16, /* width */
          wnd, /* parent */
          14, /* id */
          0  /* user data */
          );
  TexuSetColor(child, TEXU_CIO_COLOR_WHITE_RED, TEXU_CIO_COLOR_WHITE_RED);
  
  TexuAddHotKey(wnd, 'X',      ID_DELETE, 1);
  TexuAddHotKey(wnd, 'x',      ID_DELETE, 1);
  TexuAddHotKey(wnd, KEY_F(1), ID_HELP,   0);
  TexuAddHotKey(wnd, KEY_F(2), ID_ADD,    0);
  TexuAddHotKey(wnd, KEY_F(3), ID_DELETE, 0);
  TexuAddHotKey(wnd, KEY_F(4), ID_MSGBOX, 0);

  child = TexuCreateWindow(
          "",
          TEXU_STATUSBAR_CLASS,
          0, /* style*/
          0, /* exstyle*/
          starty+23, /* y */
          0, /* x */
          1,  /* height */
          100, /* width */
          wnd, /* parent */
          IDC_STATUSBAR, /* id */
          0  /* user data */
          );
  TexuSendMessage(child, TEXU_SBM_SETCOLOR, 0, TEXU_CIO_COLOR_BLUE_YELLOW);
  
  TexuSendMessage(child, TEXU_SBM_ADDPART, (texu_i64)"Part 2", 30);
  TexuSendMessage(child, TEXU_SBM_SETCOLOR, 1, TEXU_CIO_COLOR_WHITE_BLUE);
  TexuSendMessage(child, TEXU_SBM_SETALIGN, 1, TEXU_ALIGN_CENTER);
  
  TexuSendMessage(child, TEXU_SBM_ADDPART, (texu_i64)"Part 3", 20);
  TexuSendMessage(child, TEXU_SBM_SETCOLOR, 2, TEXU_CIO_COLOR_WHITE_RED);
  TexuSendMessage(child, TEXU_SBM_SETALIGN, 2, TEXU_ALIGN_RIGHT);
  
  return TEXU_OK;
}

void _MyWndProc_OnCommand(texu_wnd* wnd, texu_i32 id)
{
  texu_wnd* status = TexuGetWindowItem(wnd, IDC_STATUSBAR);
  texu_char text[TEXU_MAX_WNDTEXT+1];
  
  memset(text, 0, sizeof(text));
  sprintf(text, "enter id: %d from menu", id);
  TexuSetWindowText(status, text);
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
          /*_MyWndProc_OnAdd(wnd);*/
          break;
        case ID_MSGBOX:
          _MyWndProc_OnMsgBox(wnd);
          break;
        default:
          _MyWndProc_OnCommand(wnd, param1);
          break;
      }
      break;
    }

    case TEXU_WM_CREATE:
    {
      _MyWndProc_OnCreate(wnd);
      return TEXU_OK;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}



void _MyWndProc2_OnAdd_test(texu_wnd* wnd)
{
  texu_wnd* ctl = TexuGetWindowItem(wnd, IDC_STATUSBAR);
  texu_wnd* newwnd = 0;
  
  TexuSetWindowText(ctl, "Pressed F2 to add");
  
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
  
  TexuShowWindow(newwnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(newwnd);
}

void _MyWndProc2_OnHelp(texu_wnd* wnd)
{
  texu_wnd* ctl = TexuGetWindowItem(wnd, IDC_STATUSBAR);
  TexuSetWindowText(ctl, "Pressed F12 to exit");
}

void _MyWndProc2_OnExit(texu_wnd* wnd)
{
  /*TexuDestroyWindow(wnd);*/
}


void
_MyWndProc2_OnPaint(texu_wnd* wnd, texu_cio* dc)
{
  texu_rect rect = { 9, 0, 4, 99 };
  texu_cio_draw_frame(dc, "Hello World", &rect,
    TexuGetColor(dc, TEXU_CIO_COLOR_WHITE_BLUE));
}

texu_i64
MyWndProc2(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_PAINT:
      _MyWndProc2_OnPaint(wnd, (texu_cio*)param1);
      return 0;
      
    case TEXU_WM_COMMAND:
    {
      switch (param1)
      {
        case ID_DELETE:
          _MyWndProc2_OnExit(wnd);
          break;
          
        case ID_ADD:
          /*_MyWndProc2_OnAdd(wnd);*/
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
      TexuEnableWindow(child, TEXU_FALSE);
      
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
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
      
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
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
      
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
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 1", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 2", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 3", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 4", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 5", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 6", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 7", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 8", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 9", 0);
      TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_i64)"Item 10", 0);
      
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
      TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
      
      
      child = TexuCreateWindow(
              "F2 - List Demo",
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
      TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
      
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
      TexuSetColor(child, TEXU_CIO_COLOR_WHITE_RED, TEXU_CIO_COLOR_WHITE_RED);
      
      TexuAddHotKey(wnd, KEY_F(1), ID_HELP, 0);
      TexuAddHotKey(wnd, KEY_F(2), ID_ADD, 0);
      TexuAddHotKey(wnd, KEY_F(3), ID_DELETE, 0);
      
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
      TexuSetColor(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
      
      return TEXU_OK;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}






void _MyWndProc3_OnAdd_test(texu_wnd* wnd)
{
  texu_wnd* ctl = TexuGetWindowItem(wnd, IDC_STATUSBAR);
  texu_wnd* newwnd = 0;
  
  TexuSetWindowText(ctl, "Pressed F2 to add");
  
  newwnd = TexuCreateWindow(
          "Test TexU App - window 4",
          MyWndClass4,
          0, /* style*/
          0, /* exstyle*/
          0, /* y */
          0, /* x */
          TexuGetMaxY(),
          TexuGetMaxX(),
          0, /* parent */
          4, /* id */
          0  /* user data */
          );
  
  TexuShowWindow(newwnd, TEXU_WS_SHOW);
  TexuInvalidateWindow(newwnd);
}


void _MyWndProc3_OnExit(texu_wnd* wnd)
{
  /*TexuDestroyWindow(wnd);*/
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
          
        case ID_ADD:
          /*_MyWndProc3_OnAdd(wnd);*/
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
              100,
              wnd, /* parent */
              1, /* id */
              0  /* user data */
              );
              
      memset(&header, 0, sizeof(header));
      header.caption = "Header 1";
      header.cols = 20;
      header.align = TEXU_ALIGN_RIGHT;
      header.normcolor = TEXU_CIO_COLOR_BLUE_YELLOW;
      header.discolor  = TEXU_CIO_COLOR_BLUE_YELLOW;
      header.selcolor  = TEXU_CIO_COLOR_YELLOW_BLUE;
      header.editstyle = TEXU_ES_AUTOHSCROLL;
      header.decwidth  = 2;
      TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);
      
      header.align = TEXU_ALIGN_CENTER;
      header.caption = "Header 2";
      TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);
      
      header.align = TEXU_ALIGN_LEFT;
      header.caption = "Header 3";
      TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);
      
      header.caption = "Header 4";
      TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);
      
      header.caption = "Header 5";
      TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);

      header.caption = "Header 6";
      TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_i64)&header, 0);

      TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_i64)"\t 1\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_i64)"\t 2\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_i64)"\t 3\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_i64)"\t 4\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_i64)"\t 5\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_i64)"\t 6\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_i64)"\t 7\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_i64)"\t 8\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_i64)"\t 9\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_i64)"\t10\tHello world\tSawasdee\tAloha\tMeo\tMatt", 6);
      
      subitem.idx = 0;
      subitem.col = 0;
      subitem.normcolor = TEXU_CIO_COLOR_GREEN_WHITE;
      subitem.discolor  = TEXU_CIO_COLOR_GREEN_WHITE;
      subitem.selcolor  = TEXU_CIO_COLOR_WHITE_GREEN;
      
      TexuSendMessage(child, TEXU_LCM_SETITEM, TEXU_LCFM_COLOR, (texu_i64)&subitem);

      subitem.idx = 1;
      subitem.col = 1;
      subitem.normcolor = TEXU_CIO_COLOR_GREEN_MAGENTA;
      subitem.discolor  = TEXU_CIO_COLOR_GREEN_MAGENTA;
      subitem.selcolor  = TEXU_CIO_COLOR_MAGENTA_GREEN;
      
      TexuSendMessage(child, TEXU_LCM_SETITEM, TEXU_LCFM_COLOR, (texu_i64)&subitem);
      

      subitem.idx = 2;
      subitem.col = 2;
      subitem.normcolor = TEXU_CIO_COLOR_RED_YELLOW;
      subitem.discolor  = TEXU_CIO_COLOR_RED_YELLOW;
      subitem.selcolor  = TEXU_CIO_COLOR_YELLOW_RED;
      
      TexuSendMessage(child, TEXU_LCM_SETITEM, TEXU_LCFM_COLOR, (texu_i64)&subitem);
      
      
      child = TexuCreateWindow(
              "F2 - Tree Demo",
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
      TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
      
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
      TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
      
      TexuAddHotKey(wnd, KEY_F(1), ID_HELP, 0);
      TexuAddHotKey(wnd, KEY_F(2), ID_ADD, 0);
      TexuAddHotKey(wnd, KEY_F(3), ID_DELETE, 0);
      
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
      TexuSetColor(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
      return TEXU_OK;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}








void _MyWndProc4_OnExit(texu_wnd* wnd)
{
/*  TexuDestroyWindow(wnd);*/
}

texu_i64
MyWndProc4(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  texu_env* env = texu_wnd_get_env(wnd);
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
      texu_treewnd_item data;
      texu_tree_item* item;
      texu_tree_item* parent;
      texu_wnd* child = 0;
      
      child = TexuCreateWindow(
              "Text:",
              TEXU_TREECTRL_CLASS,
              TEXU_TCS_SHOWNODE, /* style*/
              0, /* exstyle*/
              0, /* y */
              0, /* x */
              20,
              120,
              wnd, /* parent */
              1, /* id */
              0  /* user data */
              );
              
      memset(&data, 0, sizeof(data));
      data.normcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_ITEM);
      data.discolor  = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_ITEM_DISABLED);
      data.selcolor  = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_ITEM_SELECTED);
      strcpy(data.itemtext, "Item 1");
      item = (texu_tree_item*)TexuSendMessage(child, TEXU_TCM_INSERTITEM, 0, (texu_i64)&data);
      
      strcpy(data.itemtext, "Item 1 - 1");
      TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_i64)item, (texu_i64)&data);

      strcpy(data.itemtext, "Item 1 - 2");
      parent = item;
      item = (texu_tree_item*)TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_i64)parent, (texu_i64)&data);

      strcpy(data.itemtext, "Item 1 - 3");
      TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_i64)item, (texu_i64)&data);

      strcpy(data.itemtext, "Item 1 - 4");
      TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_i64)item, (texu_i64)&data);
      
      strcpy(data.itemtext, "Item 2");
      item = (texu_tree_item*)TexuSendMessage(child, TEXU_TCM_INSERTITEM, 0, (texu_i64)&data);
      
      strcpy(data.itemtext, "Item 2 - 1");
      TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_i64)item, (texu_i64)&data);

      strcpy(data.itemtext, "Item 2 - 2");
      parent = item;
      item = (texu_tree_item*)TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_i64)parent, (texu_i64)&data);

      strcpy(data.itemtext, "Item 2 - 3");
      TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_i64)item, (texu_i64)&data);

      strcpy(data.itemtext, "Item 2 - 4");
      TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_i64)item, (texu_i64)&data);
      
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
      TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
      
      TexuAddHotKey(wnd, KEY_F(1), ID_HELP, 0);
      TexuAddHotKey(wnd, KEY_F(3), ID_DELETE, 0);
      
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
      TexuSetColor(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
      return TEXU_OK;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}


void _MyWndProc5_OnHelp(texu_wnd* wnd)
{
}

void _MyWndProc5_OnExit(texu_wnd* wnd)
{
  TexuExit();
}

texu_i64
MyWndProc5(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_COMMAND:
    {
      switch (param1)
      {
        case ID_HELP:
          _MyWndProc5_OnHelp(wnd);
          break;
        case ID_DELETE:
          _MyWndProc5_OnExit(wnd);
          break;
          
      }
      break;
    }
    case TEXU_WM_CREATE:
    {
      texu_wnd* child = TexuCreateWindow(
              "",
              TEXU_PAGECTRL_CLASS,
              0, /* style*/
              0, /* exstyle*/
              0, /* y */
              0, /* x */
              25,  /* height */
              100, /* width */
              wnd, /* parent */
              IDC_PAGECTRL1, /* id */
              0  /* user data */
              );
      /*
      TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_i64)MyWndClass5_Page1, IDC_PAGE1);
      TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_i64)MyWndClass5_Page2, IDC_PAGE2);
      */
      TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_i64)MyWndClass, IDC_PAGE1);
      TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_i64)MyWndClass2, IDC_PAGE2);
      TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_i64)MyWndClass3, IDC_PAGE3);
      TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_i64)MyWndClass4, IDC_PAGE4);

      TexuAddHotKey(wnd, KEY_F(1), ID_HELP, 0);
      TexuAddHotKey(wnd, KEY_F(3), ID_DELETE, 0);
      TexuAddHotKey(wnd, 'x', ID_DELETE, 1);
      /*TexuAddHotKey(wnd, 'X', ID_DELETE, 1);*/
      return TEXU_OK;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}

texu_i64
MyWndProc5_Page1(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_CREATE:
    {
      TexuCreateControls(wnd, templ5_p1, 50);
      return TEXU_OK;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}

texu_i64
MyWndProc5_Page2(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch(msg)
  {
    case TEXU_WM_CREATE:
    {
      TexuCreateControls(wnd, templ5_p2, 50);
      return TEXU_OK;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}



#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main2()
{
  /*texu_env* env = 0;*/
  texu_cio* cio = 0;
  texu_status rc = 0;
  texu_i32 ch = 0;
  texu_char text[80];
  texu_char* key;

  const char* params = "\0x4";

  rc = TexuStartup(25, 100);/*25 lines x 100 characters per line*/
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

  ioctl(0, TIOCSTI, params);
  while (TEXU_TRUE)
  {
    ch = texu_cio_getch(cio);

    memset(text, 0, sizeof(text));
    texu_cio_putstr(cio, 2, 0, "                                ");
    sprintf(text, "ch=%c, (%d)", ch, ch);
    texu_cio_putstr(cio, 2, 0, text);
    key = keyname(ch);
    texu_cio_putstr(cio, 3, 0, "                                ");
    texu_cio_putstr(cio, 3, 0, key);

    if (ch == KEY_F(1))
    {
      texu_cio_putstr_attr(cio, 24, 0, "exit now", 
        TexuGetColor(cio, TEXU_CIO_COLOR_YELLOW_BLUE)|A_BLINK);
      texu_cio_getch(cio);
      break;
    }
  }

  TexuShutdown();
  return 0;
}
