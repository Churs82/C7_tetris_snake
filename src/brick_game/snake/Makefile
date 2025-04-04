# Linux, Darwin or Windows_NT
OS?:=$(shell uname -s)
SHELL?:=/bin/sh
CC := g++
PWD?:=$(shell pwd)
DIR_BUILD?:=../../out/
DIR_INC=../../inc
FLAGS := -I $(DIR_INC) -std=c++17 -Wall -Wpedantic -fstack-protector-strong -g #-Werror
CFLAGS:=$(FLAGS) -c
TSTFLAGS := -lcheck
LIB_NAME = $(notdir $(CURDIR))
LIB = lib$(LIB_NAME).a
DIR_BUILD?=../../out/
DIR_TESTS?=../../tests/
TEST_SRC:=$(wildcard $(DIR_TESTS)$(LIB_NAME)_*.c)
LIB_SRC:=$(wildcard *.cpp) # https://runebook.dev/ru/docs/gnu_make/wildcard-function
TEST_OBJS=$(TEST_SRC:.c=.o)
LIB_OBJS:=$(LIB_SRC:.cpp=.o)

OS_MAC_UX:=-lsubunit -lm
RM:=-rm -rf

.phony: all, clean, test, lib 

all: lib test

clean:
	$(RM) $(LIB_OBJS) *.info *.gcov *.gcda *.gcno $(DIR_BUILD)$(LIB_NAME)_test $(TEST_OBJS) $(DIR_BUILD)$(LIB)

$(LIB_NAME) $(LIB): $(DIR_BUILD)$(LIB)

$(DIR_BUILD)$(LIB): $(LIB_OBJS)
	ar r $@ $(LIB_OBJS)
	ranlib $@

$(LIB_OBJS): $(LIB_SRC)
	$(CC) $(CFLAGS) $(DIR_LIB)$(patsubst %.o,%.cpp,$(notdir $@)) $(COV_FLAGS) -o $@

$(TEST_OBJS): $(TEST_SRC)
	$(CC) $(CFLAGS) $(DIR_TESTS)$(patsubst %.o,%.cpp,$(notdir $@)) -o $@

lib: $(LIB)

test: test_nl
	$(DIR_BUILD)$(LIB_NAME)_test

test_nl: $(LIB) $(TEST_OBJS)

ifeq ($(OS),Darwin)
	$(eval export OS_MAC_UX:=-L/opt/homebrew/lib)
endif

	$(CC) $(FLAGS) $(COV_FLAGS) $(TEST_OBJS) -o $(DIR_BUILD)$(LIB_NAME)_test $(LFLAGS) -l$(LIB_NAME) $(TSTFLAGS) $(OS_MAC_UX) -L. -L$(DIR_BUILD)

leak_check: clean test_nl

ifeq ($(OS),Darwin)
	leaks -atExit -- $(DIR_BUILD)$(LIB_NAME)_test
else
	-valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -s $(DIR_BUILD)$(LIB_NAME)_test
endif
	



