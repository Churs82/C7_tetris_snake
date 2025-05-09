# Linux, Darwin or Windows_NT
OS?:=$(shell uname -s)
SHELL?:=/bin/sh
CC := gcc
CPP :=g++
PWD?:=$(shell pwd)
DIR_INC=../../inc
FLAGS := -I $(DIR_INC) -std=c99 -Wall -Wpedantic -fstack-protector-strong -g #-Werror
LFLAGS := -lncurses
CFLAGS:=$(FLAGS) -c
DIR_BUILD:=../../out/
DIR_GAMES:=../../brick_game/
IF_NAME = $(notdir $(CURDIR))
GAMES?=$(notdir $(wildcard $(DIR_GAMES)*))
BINARIES:=$(addsuffix _$(IF_NAME), $(GAMES))
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

%.o: %.c %.h
	$(CC) $(CFLAGS) $(patsubst %.o,%.c,$(notdir $@)) -o $@

$(GAMES): 
	$(MAKE) $(DIR_BUILD)$@_$(IF_NAME)

$(addsuffix _$(IF_NAME), $(addprefix $(DIR_BUILD),$(GAMES))): $(IF_OBJS) 
	$(CPP) $(FLAGS) $(IF_OBJS) $(LFLAGS) -l$(notdir $(patsubst %_$(IF_NAME),%,$@)) $(OS_MAC_UX) -L. -L$(DIR_BUILD)  -o $@
	
	



