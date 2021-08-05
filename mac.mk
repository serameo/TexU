#
# File Name: makefile
# Author: Seree Rakwong
# Date: 06-MAY-2020
#

TESTUI  = test_texu
TESTLIB = test_texulib
LIBTEXU = libtexu.dylib
CC     = gcc
CFLAGS = -g -Wall
LFLAGS = -lm -lcurses -lpthread -lsqlite3

.PHONY: default all clean

default: $(TESTUI)
testlib: $(TESTLIB)
libtexu: $(LIBTEXU)
all: default

TEXU_SRC_DIR = ./src
TEXU_INC_DIR = ./include
TEXU_LIB_DIR = ./lib
TEXU_SOURCES = $(TEXU_SRC_DIR)/texulib.c \
               $(TEXU_SRC_DIR)/texutils.c \
               $(TEXU_SRC_DIR)/texucio.c \
               $(TEXU_SRC_DIR)/texui.c \
               $(TEXU_SRC_DIR)/texuproc.c \
               $(TEXU_SRC_DIR)/texuctrl.c \
               $(TEXU_SRC_DIR)/texumenu.c \
               $(TEXU_SRC_DIR)/texu.c


TEXU_OBJECTS  = $(TEXU_SOURCES:.c=.o)
TEXU_INCLUDES = -I$(TEXU_INC_DIR)
TEXU_LIBS     = -L$(TEXU_LIB_DIR) -ltexu

TEST_SRC_DIR = ./src
TEST_INC_DIR = ./include
TEST_SOURCES = $(TEST_SRC_DIR)/test_texu.c
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

TESTLIB_SOURCES = $(TEST_SRC_DIR)/test_texulib.c
TESTLIB_OBJECTS = $(TESTLIB_SOURCES:.c=.o)

DEFINES = -DXTERM_256COLOR

%.o: %.c
	$(CC) $(CFLAGS) $(TEXU_INCLUDES) $(DEFINES) -c $< -o $@


#$(LIBTEXU): $(TEXU_OBJECTS)
#	$(CC) $(CLIBFLAGS) $(TEXU_INCLUDES) $(DEFINES) -c $< -o $@

$(TESTUI): $(TEST_OBJECTS) $(TEXU_OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ $(TEXU_LIBS)

$(TESTLIB): $(TESTLIB_OBJECTS)
	$(CC) $(CLIBFLAGS) $(LFLAGS) -o $@ $^ $(TEXU_LIBS)

clean:
	rm -f src/*.o $(TESTUI)*


