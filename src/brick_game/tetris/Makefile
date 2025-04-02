# Linux, Darwin or Windows_NT
OS?:=$(shell uname -s)
SHELL?:=/bin/sh
CC := gcc
PWD?:=$(shell pwd)
DIR_BUILD?:=../../out/
DIR_INC=../../inc
FLAGS := -I $(DIR_INC) -std=c99 -Wall -Wpedantic -fstack-protector-strong -g #-Werror
CFLAGS:=$(FLAGS) -c
TSTFLAGS := -lcheck
LIB_NAME = $(notdir $(CURDIR))
LIB = lib$(LIB_NAME).a
DIR_BUILD?=../../out/
DIR_TESTS?=../../tests/
TEST_SRC:=$(wildcard $(DIR_TESTS)$(LIB_NAME)_*.c)
LIB_SRC:=$(wildcard *.c) # https://runebook.dev/ru/docs/gnu_make/wildcard-function
TEST_OBJS=$(addprefix $(DIR_BUILD),$(notdir $(TEST_SRC:.c=.o)))
LIB_OBJS:=$(addprefix $(DIR_BUILD),$(notdir $(LIB_SRC:.c=.o)))

OS_MAC_UX:=-lsubunit -lm
RM:=-rm -rf

.phony: all, clean, test, lib 

all: lib test

clean:
	$(RM) $(LIB_OBJS) $(DIR_BUILD)$(LIB_NAME).info $(DIR_BUILD)$(LIB_NAME).gcov $(DIR_BUILD)$(LIB_NAME).gcda $(DIR_BUILD)$(LIB_NAME).gcno $(DIR_BUILD)$(LIB_NAME)_test $(TEST_OBJS)$(DIR_BUILD)$(LIB)

$(LIB_NAME) $(LIB): $(DIR_BUILD)$(LIB)

$(DIR_BUILD)$(LIB): $(LIB_OBJS)
	ar r $@ $(LIB_OBJS)
	ranlib $@

$(LIB_OBJS): $(LIB_SRC)
	$(CC) $(CFLAGS) $(DIR_LIB)$(patsubst %.o,%.c,$(notdir $@)) $(GCOV) -o $@

$(TEST_OBJS): $(TEST_SRC)
	$(CC) $(CFLAGS) $(DIR_TESTS)$(patsubst %.o,%.c,$(notdir $@)) -o $(DIR_BUILD)$(notdir $@) 

lib: $(LIB)

test: test_nl
	$(DIR_BUILD)$(LIB_NAME)_test

test_nl: $(LIB) $(TEST_OBJS)

ifeq ($(OS),Darwin)
	$(eval OS_MAC_UX=-L/opt/homebrew/lib)
endif

	$(CC) $(FLAGS) $(GCOV) $(TEST_OBJS) -o $(DIR_BUILD)$(LIB_NAME)_test $(LFLAGS) -l$(LIB_NAME) $(TSTFLAGS) $(OS_MAC_UX) -L. -L$(DIR_BUILD)

leak_check: clean test_nl

ifeq ($(OS),Darwin)
	leaks -atExit -- $(DIR_BUILD)$(LIB_NAME)_test
else
	-valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -s $(DIR_BUILD)$(LIB_NAME)_test
endif
	



