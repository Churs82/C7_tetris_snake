# Linux, Darwin or Windows_NT
OS?:=$(shell uname -s)
SHELL?:=/bin/sh
CC := gcc
PWD?:=$(shell pwd)
DIR_BUILD?:=../../out/
DIR_INC=../../inc
FLAGS := -I $(DIR_INC) -std=c99 -Wall -Wpedantic -fstack-protector-strong -g #-Werror
LFLAGS := -lncurses
CFLAGS:=$(FLAGS) -c
DIR_BUILD?=../../out/
DIR_GAMES?=../../brick_game/
IF_NAME = $(notdir $(CURDIR))
GAMES?=$(notdir $(wildcard $(DIR_GAMES)*))
IF_SRC:=$(wildcard *.c) 
IF_OBJS:=$(IF_SRC:.c=.o)

OS_MAC_UX:=-lsubunit -lm
RM:=-rm -rf

.phony: all, clean, games

all: games

clean:
	for game in $(GAMES); do \
        $(RM) $(DIR_BUILD)$${game}_$(IF_NAME); \
    done
	$(RM) $(IF_OBJS)

ifeq ($(OS),Darwin)
	$(eval OS_MAC_UX=-L/opt/homebrew/lib)
endif

games: $(GAMES)

$(IF_OBJS):
	$(CC) $(CFLAGS) $(patsubst %.o,%.c,$(notdir $@)) -o $@

$(GAMES): $(IF_OBJS) 
	$(CC) $(FLAGS) -o $(DIR_BUILD)$@_$(IF_NAME) $(IF_OBJS) $(LFLAGS) -l$@ $(OS_MAC_UX) -L. -L$(DIR_BUILD)
	



