#ifndef _AUTOUI_H_
#define _AUTOUI_H_

#include "texu.h"


#define LOGINWND_CLASS          "LoginWnd_Class"


#define ID_LOGIN                (TEXU_WM_USER +  1)
#define ID_DISCONNECT           (TEXU_WM_USER +  2)
#define ID_DISCONNECTALL        (TEXU_WM_USER +  3)
#define ID_EXIT                 (TEXU_WM_USER +  4)
#define ID_HELP                 (TEXU_WM_USER +  5)
#define IDC_PAGECTRL1           (TEXU_WM_USER +  6)
#define IDC_PAGE1               (TEXU_WM_USER +  7)
#define IDC_USER                (TEXU_WM_USER +  8)
#define IDC_PASSWD              (TEXU_WM_USER +  9)
#define IDC_TOTALOUTPACKS       (TEXU_WM_USER + 10)
#define IDC_INFO                (TEXU_WM_USER + 11)

#define IDC_LABEL1              (TEXU_WM_USER + 101)
#define IDC_LABEL2              (TEXU_WM_USER + 102)
#define IDC_LABEL3              (TEXU_WM_USER + 103)
#define IDC_LABEL4              (TEXU_WM_USER + 104)
#define IDC_LABEL5              (TEXU_WM_USER + 105)
#define IDC_LABEL6              (TEXU_WM_USER + 106)
#define IDC_LABEL7              (TEXU_WM_USER + 107)
#define IDC_LABEL8              (TEXU_WM_USER + 108)
#define IDC_LABEL9              (TEXU_WM_USER + 109)
#define IDC_LABEL10             (TEXU_WM_USER + 110)



texu_i64 Login_WndProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);



#endif /*_AUTOUI_H_*/
