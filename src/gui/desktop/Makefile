# Linux, Darwin or Windows_NT
OS?:=$(shell uname -s)
SHELL?:=/bin/sh
CC = g++
PWD?:=$(shell pwd)
DIR_INC=../../inc
FLAGS := -std=c++17 -Wall -Wpedantic -fstack-protector-strong -g -fPIC  #-Werror
FLAGS += -I${DIR_INC}
FLAGS += -I/usr/include/x86_64-linux-gnu/qt6/
LFLAGS := -lQt6Core -lQt6Gui -lQt6Widgets
DIR_BUILD:=../../out/
DIR_GAMES:=../../brick_game/
IF_NAME = $(notdir $(CURDIR))
GAMES?=$(notdir $(wildcard $(DIR_GAMES)*))
IF_SRC:=$(wildcard *.cpp) 
IF_OBJS:=$(IF_SRC:.cpp=.o)

OS_MAC_UX:=
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
	$(CC) $(FLAGS) -c $(patsubst %.o,%.cpp,$(notdir $@)) -o $@

$(GAMES): 
	$(MAKE) $(DIR_BUILD)$@_$(IF_NAME)

$(addsuffix _$(IF_NAME), $(addprefix $(DIR_BUILD),$(GAMES))): $(IF_OBJS) 
	$(CC) $(FLAGS) $(IF_OBJS) $(LFLAGS) -l$(notdir $(patsubst %_$(IF_NAME),%,$@)) $(OS_MAC_UX) -L. -L$(DIR_BUILD)  -o $@
	



