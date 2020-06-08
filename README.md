# TexU
Text based user interface library using ncurses library.

How to build:
$ make
gcc -g -Wall -m32 -I./include -DXTERM_256COLOR -c src/texulib.c -o src/texulib.o
gcc -g -Wall -m32 -I./include -DXTERM_256COLOR -c src/texutils.c -o src/texutils.o
gcc -g -Wall -m32 -I./include -DXTERM_256COLOR -c src/texucio.c -o src/texucio.o
gcc -g -Wall -m32 -I./include -DXTERM_256COLOR -c src/texui.c -o src/texui.o
gcc -g -Wall -m32 -I./include -DXTERM_256COLOR -c src/texuproc.c -o src/texuproc.o
gcc -g -Wall -m32 -I./include -DXTERM_256COLOR -c src/texuctrl.c -o src/texuctrl.o
gcc -g -Wall -m32 -I./include -DXTERM_256COLOR -c src/texumenu.c -o src/texumenu.o
gcc -g -Wall -m32 -I./include -DXTERM_256COLOR -c src/texu.c -o src/texu.o
gcc -g -Wall -m32 -I./include -DXTERM_256COLOR -c src/test_texu.c -o src/test_texu.o
gcc -g -Wall -m32 -lm -lcurses -lpthread -lsqlite3 -o test_texu src/texulib.o src/texutils.o src/texucio.o src/texui.o src/texuproc.o src/texuctrl.o src/texumenu.o src/texu.o src/test_texu.o
d
How to enable terminal supported 256 colors
$ TERM=xterm-256color

