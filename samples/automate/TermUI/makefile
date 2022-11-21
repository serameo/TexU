
SRC_DIR=./src
INC_DIR=./include

SRCS=   $(SRC_DIR)/loginwnd.c \
        $(SRC_DIR)/autoui.c

OBJS=$(SRCS:.c=.o)

TARGET=autoui

LIBSQL=libsqlite3.so
LIBSQL_DIR=/usr
LIBSQL_INC=$(LIBSQL_DIR)/include
LIBSQL_LIB=$(LIBSQL_DIR)/lib
LIBSQL_LDFLAGS=-L$(LIBSQL_LIB) -lsqlite3

LIBTEXU=libtexu.a
LIBTEXU_DIR=./TexU
LIBTEXU_INC=$(LIBTEXU_DIR)/include
LIBTEXU_LIB=$(LIBTEXU_DIR)/lib
LIBTEXU_LDFLAGS=-ltexu -L$(LIBTEXU_LIB) 


DFLAGS= -I. \
        -I$(INC_DIR) \
        -I$(LIBTEXU_INC) \
        -I$(LIBSQL_INC) 

CFLAGS=-m32 -g
#CFLAGS=-m32
OPTIONS = 
LDFLAGS = -L../lib  -L/usr/local/lib -lpthread -lm -lcl -lz -lrt -lcurses $(LIBTEXU_LDFLAGS) $(LIBSQL_LDFLAGS) 

CC=gcc
BIN=Linux_3.10.0-1062.el7.x86_64
INSTALL_DIR=../lib
SOURCE_DIR=.
DEPENDFLAGS=-D__UNIX__ -D__LINUX__ -DUSE_TCL_AUTOMATION


DEFINES  = $(CFLAGS) $(OPTIONS) $(DFLAGS) $(LDFLAGS) $(DEPENDFLAGS)

.PHONY: default all clean

default: $(TARGET)

all: default

%.o: %.c
	$(CC) $(DEFINES) -c $< -o $@
    

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(DEFINES)
	#cp $(TARGET) $(BOS_EXE)
    
clean:
	rm -f *.o  $(TARGET) $(OBJS) core a.out

depend:
	makedepend -- $(DEPENDFLAGS) $(CFLAGS) -- $(SRCS) 
# DO NOT DELETE

