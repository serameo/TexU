# TexU
Text based user interface library using ncurses library. To see, how to create a simple application under src/test_texu.c.

To create your own controls, see src/texuproc.c and src/texuctrl.c

These files are included many text objects by the following:
- Window
- Label (TEXU_LABEL_CLASS)
- Edit (TEXU_EDIT_CLASS)
- List box (radio, check, or simple list box) (TEXU_LISTBOX_CLASS)
- Combo box (TEXU_COMBOBOX_CLASS)
- Status bar (TEXU_STATUSBAR_CLASS)
- List control (TEXU_LISTCTRL_CLASS)
- Tree control (TEXU_TREECTRL_CLASS)
- Up/down control (TEXU_UPDOWNCTRL_CLASS)
- Progress bar control (TEXU_PROGRESSBAR_CLASS)
- Menu (TEXU_MENU_CLASS / TEXU_MENUWND_CLASS)
- Page control (TEXU_PAGECTRL_CLASS)
- Simple console I/O (e.g. writing/reading a character, drawing a line, drawing a rectangle etc.)

All text objects are implemented by using a simple library (TexU_List, TexU_Stack, TexU_Queue, TexU_Tree, etc.) See also include/texulib.h. Also, some utilities such as texu_dblog object is using sqlite library to handle a database log file.

This library concept is the same as Win32 API concept. All window classes are required to register into the TexU environment. And then, you could implement your message by your procedure.

How to build:
- $ make

How to enable terminal supported 256 colors
- $ TERM=xterm-256color


<p align="center">
  <img src="https://github.com/serameo/TexU/blob/master/samples/TexU_sample06.png" width="640" title="Sample1">
</p>
<p align="center">
  <img src="https://github.com/serameo/TexU/blob/master/samples/TexU_sample07.png" width="640" title="Sample2">
</p>
