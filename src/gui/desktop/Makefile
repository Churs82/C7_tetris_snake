# Linux, Darwin or Windows_NT
OS?:=$(shell uname -s)
SHELL?:=/bin/sh
CC := g++
CPP :=g++
PWD?:=$(shell pwd)
DIR_INC=../../inc
FLAGS := -I $(DIR_INC) -std=c++17 -Wall -Wpedantic -fstack-protector-strong -g -fPIC  #-Werror
CFLAGS += -I/usr/include/x86_64-linux-gnu/qt6/QtCore -I/usr/include/x86_64-linux-gnu/qt6/ -I/usr/include/x86_64-linux-gnu/qt6/QtGui -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets
LFLAGS := -lQt6Core -lQt6Gui -lQt6Widgets
CFLAGS:=$(FLAGS) -c
DIR_BUILD:=../../out/
DIR_GAMES:=../../brick_game/
IF_NAME = $(notdir $(CURDIR))
GAMES?=$(notdir $(wildcard $(DIR_GAMES)*))
IF_SRC:=$(wildcard *.c) 
IF_OBJS:=$(IF_SRC:.c=.o)

OS_MAC_UX:=-lsubunit -lm
RM:=rm -rf

.phony: all, clean, games

all: games

clean:
	for game in $(GAMES); do $(RM) $(DIR_BUILD)$${game}_$(IF_NAME); done
	$(RM) $(IF_OBJS)

ifeq ($(OS),Darwin)
	$(eval OS_MAC_UX=-L/opt/homebrew/lib)
endif

games: $(GAMES)

$(IF_OBJS):
	$(CC) $(CFLAGS) $(patsubst %.o,%.c,$(notdir $@)) -o $@

$(GAMES): $(IF_OBJS) 
	$(CPP) $(FLAGS) -o $(DIR_BUILD)$@_$(IF_NAME) $(IF_OBJS) $(LFLAGS) -l$@ $(OS_MAC_UX) -L. -L$(DIR_BUILD)
	



