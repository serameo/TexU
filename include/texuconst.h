/*
#
# File Name: texuconst.h
# Author: Seree Rakwong
# Date: 06-MAY-2020
#
*/

#ifndef _TEXUCONST_H_
#define _TEXUCONST_H_

/*
# TexU definitions and macros
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

/* status */
#define TEXU_NOMEM                     (-2)
#define TEXU_ERROR                     (-1)
#define TEXU_OK                        (0)
#define TEXU_NOTFOUND                  (3)
#define TEXU_NOTREGISTERED_CLASS       (4)
#define TEXU_DUPLICATED_ID             (5)

/* state */
#define TEXU_CONTINUE                  (0)
#define TEXU_BREAK                     (-1)

/* boolean */
#define TEXU_FALSE                     (0)
#define TEXU_TRUE                      (1)

/* compare */
#define TEXU_CMP_LT                    (-1)                /*less than*/
#define TEXU_CMP_EQ                    (0)                 /*equal to*/
#define TEXU_CMP_GT                    (1)                 /*greater than*/

#define TEXU_MAX_WNDTEXT               256

#define TEXU_WM_USER                   10000

#define TEXU_WM_FIRST                  0
#define TEXU_WM_CHAR                   (TEXU_WM_FIRST +  1)
#define TEXU_WM_SETFOCUS               (TEXU_WM_FIRST +  2)
#define TEXU_WM_KILLFOCUS              (TEXU_WM_FIRST +  3)
#define TEXU_WM_SETTEXT                (TEXU_WM_FIRST +  4)
#define TEXU_WM_GETTEXT                (TEXU_WM_FIRST +  5)
#define TEXU_WM_PAINT                  (TEXU_WM_FIRST +  6)
#define TEXU_WM_CREATE                 (TEXU_WM_FIRST +  7)
#define TEXU_WM_DESTROY                (TEXU_WM_FIRST +  8)
#define TEXU_WM_ENABLE                 (TEXU_WM_FIRST +  9)
#define TEXU_WM_SHOW                   (TEXU_WM_FIRST + 10)
#define TEXU_WM_ERASEBG                (TEXU_WM_FIRST + 11)
#define TEXU_WM_COMMAND                (TEXU_WM_FIRST + 12)
#define TEXU_WM_NOTIFY                 (TEXU_WM_FIRST + 13)


/* EDIT */
#define TEXU_EM_FIRST                  100
#define TEXU_EM_LIMITTEXT              (TEXU_EM_FIRST +  1)
#define TEXU_EM_SETPASSWDCHAR          (TEXU_EM_FIRST +  2)
#define TEXU_EM_SETDECWIDTH            (TEXU_EM_FIRST +  3)
#define TEXU_EM_SHOWPASSWDCHAR         (TEXU_EM_FIRST +  4)
#define TEXU_EM_SETVALIDSTRING         (TEXU_EM_FIRST +  5)
#define TEXU_EM_SETVALIDMINMAX         (TEXU_EM_FIRST +  6)

/* edit notification */
#define TEXU_EN_FIRST                  100
#define TEXU_EN_SETFOCUS               (TEXU_EN_FIRST +  1)
#define TEXU_EN_KILLFOCUS              (TEXU_EN_FIRST +  2)



/* LISTBOX */
#define TEXU_LBM_FIRST                 200
#define TEXU_LBM_ADDITEM               (TEXU_LBM_FIRST +  1)
#define TEXU_LBM_DELETEITEM            (TEXU_LBM_FIRST +  2)
#define TEXU_LBM_GETCURSEL             (TEXU_LBM_FIRST +  3)
#define TEXU_LBM_SETCURSEL             (TEXU_LBM_FIRST +  4)
#define TEXU_LBM_DELETEALLITEMS        (TEXU_LBM_FIRST +  5)
#define TEXU_LBM_GETITEMCOUNT          (TEXU_LBM_FIRST +  6)
#define TEXU_LBM_GETITEMDATA           (TEXU_LBM_FIRST +  7)
#define TEXU_LBM_SETITEMDATA           (TEXU_LBM_FIRST +  8)
#define TEXU_LBM_SETITEMTEXT           (TEXU_LBM_FIRST +  9)
#define TEXU_LBM_GETITEMTEXT           (TEXU_LBM_FIRST + 10)
#define TEXU_LBM_COUNTITEMCHECKED      (TEXU_LBM_FIRST + 11)
#define TEXU_LBM_GETITEMCHECKED        (TEXU_LBM_FIRST + 12)
#define TEXU_LBM_SETITEMCHECKED        (TEXU_LBM_FIRST + 13)
#define TEXU_LBM_ENABLEITEM            (TEXU_LBM_FIRST + 14)

/* listbox notification */
#define TEXU_LBN_FIRST                 200
#define TEXU_LBN_SETFOCUS              (TEXU_LBN_FIRST +  1)
#define TEXU_LBN_KILLFOCUS             (TEXU_LBN_FIRST +  2)
#define TEXU_LBN_DELETEITEM            (TEXU_LBN_FIRST +  3)
#define TEXU_LBN_SELCHANGED            (TEXU_LBN_FIRST +  4)



/* listctrl notification */
#define TEXU_LCM_FIRST                 300
#define TEXU_LCM_SETEDITABLECOLS       (TEXU_LCM_FIRST +  1)
#define TEXU_LCM_GETITEMSPERPAGE       (TEXU_LCM_FIRST +  2)
#define TEXU_LCM_GETCURPAGE            (TEXU_LCM_FIRST +  3)
#define TEXU_LCM_SETCURPAGE            (TEXU_LCM_FIRST +  4)
#define TEXU_LCM_GETEDITBOX            (TEXU_LCM_FIRST +  5)
#define TEXU_LCM_SETCOLWIDTH           (TEXU_LCM_FIRST +  6)
#define TEXU_LCM_SETCURROW             (TEXU_LCM_FIRST +  7)
#define TEXU_LCM_GETCURROW             (TEXU_LCM_FIRST +  8)
#define TEXU_LCM_GETEDITSTYLE          (TEXU_LCM_FIRST +  9)
#define TEXU_LCM_SETEDITSTYLE          (TEXU_LCM_FIRST + 10)
#define TEXU_LCM_INVALIDATEITEM        (TEXU_LCM_FIRST + 11)
#define TEXU_LCM_GETITEMCOUNT          (TEXU_LCM_FIRST + 12)
#define TEXU_LCM_GETITEM               (TEXU_LCM_FIRST + 13)
#define TEXU_LCM_SETITEM               (TEXU_LCM_FIRST + 14)
#define TEXU_LCM_DELETEALLITEMS        (TEXU_LCM_FIRST + 15)
#define TEXU_LCM_DELETEITEM            (TEXU_LCM_FIRST + 16)
#define TEXU_LCM_ADDITEM               (TEXU_LCM_FIRST + 17)
#define TEXU_LCM_DELETEALLCOLUMNS      (TEXU_LCM_FIRST + 18)
#define TEXU_LCM_DELETECOLUMN          (TEXU_LCM_FIRST + 19)
#define TEXU_LCM_ADDCOLUMN             (TEXU_LCM_FIRST + 20)



#define TEXU_LCN_FIRST                 300
#define TEXU_LCN_SETFOCUS              (TEXU_LCN_FIRST +  1)
#define TEXU_LCN_KILLFOCUS             (TEXU_LCN_FIRST +  2)
#define TEXU_LCN_DELETEITEM            (TEXU_LCN_FIRST +  3)
#define TEXU_LCN_SELCHANGED            (TEXU_LCN_FIRST +  4)
#define TEXU_LCN_BEGINMOVING           (TEXU_LCN_FIRST +  5)
#define TEXU_LCN_ENDMOVING             (TEXU_LCN_FIRST +  6)
#define TEXU_LCN_ENDINSERTROWOK        (TEXU_LCN_FIRST +  7)
#define TEXU_LCN_ENDINSERTROWCANCEL    (TEXU_LCN_FIRST +  8)
#define TEXU_LCN_BEGININSERTROW        (TEXU_LCN_FIRST +  9)
#define TEXU_LCN_BEGINEDITROW          (TEXU_LCN_FIRST + 10)
#define TEXU_LCN_ENDEDITROWCANCEL      (TEXU_LCN_FIRST + 11)
#define TEXU_LCN_ENDEDITROWOK          (TEXU_LCN_FIRST + 12)
#define TEXU_LCN_BEGINEDIT             (TEXU_LCN_FIRST + 13)
#define TEXU_LCN_ENDEDITOK             (TEXU_LCN_FIRST + 14)
#define TEXU_LCN_ENDEDITCANCEL         (TEXU_LCN_FIRST + 15)






#define TEXU_KEY_NEXTWND               10
#define TEXU_KEY_PREVWND               KEY_BTAB
#define TEXU_KEY_ESCAPE                27


#define TEXU_ENV_MAX_FRAMES            4096

#define TEXU_DESKTOP_CLASS             "TexuDesktopClass"
#define TEXU_LABEL_CLASS               "TexuLabelClass"
#define TEXU_EDIT_CLASS                "TexuEditClass"
#define TEXU_LISTBOX_CLASS             "TexuListBoxClass"
#define TEXU_COMBOBOX_CLASS            "TexuComboBoxClass"
#define TEXU_LISTCTRL_CLASS            "TexuListCtrlClass"
#define TEXU_TREECTRL_CLASS            "TexuTreeCtrlClass"


/* window styles */
#define TEXU_WS_LEFT                   0x00000001          /* shared controls style */
#define TEXU_WS_CENTER                 0x00000002
#define TEXU_WS_RIGHT                  0x00000004


#define TEXU_ALIGN_LEFT                TEXU_WS_LEFT
#define TEXU_ALIGN_CENTER              TEXU_WS_CENTER
#define TEXU_ALIGN_RIGHT               TEXU_WS_RIGHT


/* EDIT */
#define TEXU_ES_LEFT                   TEXU_WS_LEFT
#define TEXU_ES_CENTER                 TEXU_WS_CENTER
#define TEXU_ES_RIGHT                  TEXU_WS_RIGHT
#define TEXU_ES_AUTOHSCROLL            0x00010000
#define TEXU_ES_APPENDMODE             0x00020000
#define TEXU_ES_PASSWORD               0x00040000
#define TEXU_ES_NUMBER                 0x00080000
#define TEXU_ES_DECIMAL                0x00100000
#define TEXU_ES_AUTODECIMALCOMMA       0x00200000
#define TEXU_ES_UPPERCASE              0x00400000
#define TEXU_ES_LOWERCASE              0x00800000
#define TEXU_ES_A2Z                    0x01000000


/* LISTBOX */
#define TEXU_LBS_CHECKBOX              0x00010000
#define TEXU_LBS_RADIOBOX              0x00020000

#define TEXU_LB_UNCHECKED              0
#define TEXU_LB_CHECKED                1

#define TEXU_LB_OK                     0
#define TEXU_LB_ERROR                  -1



/* LIST CTRL */
#define TEXU_LCS_EDITABLE             0x00010000
#define TEXU_LCS_NOHEADER             0x00020000
#define TEXU_LCS_NOBORDER             0x00040000
#define TEXU_LCS_NOSELECTION          0x00080000
#define TEXU_LCS_LINEEDIT             0x00100000

#define TEXU_LC_ENDEDITOK             0x00000000
#define TEXU_LC_ENDEDITCANCEL         0x00000001

#define TEXU_LCT_EDITING              0x00000001
#define TEXU_LCT_VIEW                 0x00000002
#define TEXU_LCT_MOVINGCURSOR         0x00000003
#define TEXU_LCT_INSERTING            0x00000004
#define TEXU_LCT_ENDINSERTING         0x00000005
#define TEXU_LCT_BEGINEDITING         0x00000006
#define TEXU_LCT_ENDMOVING            0x00000007
#define TEXU_LCT_ENDEDITING           0x00000008


#define TEXU_LCFM_TEXT                0x0001
#define TEXU_LCFM_DATA                0x0002
#define TEXU_LCFM_COLOR               0x0004


#endif /*_TEXUCONST_H_*/
