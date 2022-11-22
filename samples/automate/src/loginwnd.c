#include "autoui.h"
#include "texu.h"
#include <string.h>


texu_i32 _login_validate_user(texu_wnd *wnd, texu_char* sUser);
texu_i32 _login_validate_passwd(texu_wnd *wnd, texu_char* sPasswd);

texu_wnd_template2 templ_login2[] =
{
  { "User Id  :",               TEXU_LABEL_CLASS,       TEXU_WS_RIGHT,  0, 10, 27,  1,  10, IDC_LABEL4, 0 },
  { "",                         TEXU_EDIT_CLASS,        TEXU_ES_LEFT|TEXU_ES_UPPERCASE,
                                                                        0, 10, 38,  1,  16, IDC_USER,   /*10008*/
                                                                        _login_validate_user },
  { "Password :",               TEXU_LABEL_CLASS,       TEXU_ES_LEFT,   0, 12, 27,  1,  10, IDC_LABEL5, 0 },
  { "",                         TEXU_EDIT_CLASS,        TEXU_ES_LEFT|TEXU_ES_UPPERCASE|TEXU_ES_PASSWORD|
                                                        TEXU_ES_AUTOHSCROLL,
                                                                        0, 12, 38,  1,  16, IDC_PASSWD, /*10009*/
                                                                        _login_validate_passwd },

  { "",                         TEXU_LABEL_CLASS,       TEXU_WS_CENTER, 0, 20,  1,  1,  78, IDC_INFO,   /* 10011*/
                                                                        0 },
  /*last control is nil*/
  { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

texu_i32 _login_validate_user(texu_wnd *wnd, texu_char* sUser)
{
    texu_wnd *parent = TexuGetParent(wnd);
    return TEXU_OK;
}

texu_i32 _login_validate_passwd(texu_wnd *wnd, texu_char* sPasswd)
{
    texu_wnd *parent = TexuGetParent(wnd);
    texu_wnd *edit = TexuGetWindowItem(parent, IDC_USER);
    texu_char user[TEXU_MAX_WNDTEXT + 1];
    
    TexuGetWindowText(edit, user, TEXU_MAX_WNDTEXT);
    if (0 == strlen(sPasswd))
    {
        return TEXU_OK;
    }
    
    edit = TexuGetWindowItem(parent, IDC_INFO);
    if (0 == strcmp(user, "1000") && 0 == strcmp(sPasswd, "H1W0RLD"))
    {
        TexuSetWindowText(edit, "Login OK");
        return TEXU_OK;
    }
    else
    {
        TexuSetWindowText(edit, "Invalid user or password");
    }
    
    
    edit = TexuGetWindowItem(parent, IDC_USER);
    TexuSendMessage(edit, TEXU_WM_SETFOCUS, 0, 0);
    return TEXU_ERROR;
}

void Login_OnLogin(texu_wnd *wnd)
{
    texu_wnd *edit = TexuGetWindowItem(wnd, IDC_PASSWD);
    texu_char passwd[TEXU_MAX_WNDTEXT + 1];
    texu_i32 rc = 0;
    
    TexuGetWindowText(edit, passwd, TEXU_MAX_WNDTEXT);
    rc = _login_validate_passwd(edit, passwd);
}

void Login_OnPaint(texu_wnd *wnd, texu_cio *dc)
{

    static texu_rect rect  = {0,  0, 24, 79};    /*outer frame*/
    static texu_rect rect2 = {8, 25,  6, 31};    /*inner frame*/

    TexuDrawFrame(dc, "Sample Login App", &rect, TexuGetColor(dc, TEXU_CIO_COLOR_WHITE_BLUE));
    TexuDrawFrame(dc, "", &rect2, TexuGetColor(dc, TEXU_CIO_COLOR_WHITE_BLUE));
}

texu_i64 Login_WndProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    texu_wnd* edit = 0;
    switch (msg)
    {
        case TEXU_WM_IDLE:      /* 19 */
        {
            break;
        }
        case TEXU_WM_COMMAND:   /* 12 */
        {
            switch (param1)
            {
                case ID_LOGIN:  /* 10001 */
                {
                    Login_OnLogin(wnd);
                    break;
                }
            }
            break;
        }
        case TEXU_WM_PAINT:    /* 6 */
        {
            Login_OnPaint(wnd, (texu_cio *)param1);
            break;
        }
        case TEXU_WM_CREATE:    /* 7 */
        {
            texu_status rc = TexuCreateControls2(wnd, templ_login2, 50);
            if (rc != TEXU_OK)
            {
                return rc;
            }
            edit = TexuGetWindowItem(wnd, IDC_USER);
            TexuSendMessage(edit, TEXU_EM_LIMITTEXT, 4, 0);
            edit = TexuGetWindowItem(wnd, IDC_PASSWD);
            TexuSendMessage(edit, TEXU_EM_LIMITTEXT, 16, 0);
            return TEXU_OK;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

