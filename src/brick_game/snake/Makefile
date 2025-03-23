# Linux, Darwin or Windows_NT
OS=$(shell uname -s)
CC := gcc
FLAGS := -I ../../inc -std=c99 -Wall -Wpedantic -fstack-protector-strong -g #-Werror
CFLAGS:=$(FLAGS) -c
TSTFLAGS = -lcheck
SHELL = /bin/sh
DIR_BUILD=../../out/
DIR_TESTS=../../tests/
TEST_SRC=$(wildcard tests/*.c)
LIB_SRC=$(wildcard *.c) # https://runebook.dev/ru/docs/gnu_make/wildcard-function
TEST_OBJS=$(addprefix $(DIR_BUILD),$(notdir $(TEST_SRC:.c=.o)))
IF_OBJS=$(addprefix $(DIR_BUILD),$(notdir $(IF_SRC:.c=.o)))
LIB_OBJS:=$(notdir $(LIB_SRC:.c=.o))
LIB = libsnake.a
LIB_NAME = snake

OS_MAC_UX=-lsubunit -lm
RM:=-rm -rf

.phony: all, clean, dvi, test, gcov_report $(LIB) ext1_flag 

all:  clean $(LIB) test

clean:
	$(RM) *.o $(DIR_BUILD)*.info $(DIR_BUILD)*.gcov $(DIR_BUILD)*.gcda $(DIR_BUILD)*.gcno $(DIR_BUILD)$(LIB) $(DIR_BUILD)$(LIB_NAME)_test

test: test_nl
	$(DIR_BUILD)$(LIB_NAME)_test

gcov_report: clean ext1_flag $(LIB) test
	-lcov -t "$(LIB_NAME)_report" -o $(DIR_BUILD)$(LIB_NAME)_report.info -c -d $(DIR_BUILD)
	-geninfo $(DIR_BUILD) -o $(DIR_BUILD)$(LIB_NAME)_report.info
	-genhtml -o $(DIR_BUILD)report_${LIB_NAME} $(DIR_BUILD)$(LIB_NAME)_report.info
	-mkdir $(DIR_BUILD)report_${LIB_NAME}
	cd $(DIR_BUILD)
	-@python3 -m gcovr -r . --html --html-details -o $(DIR_BUILD)report_${LIB_NAME}/index.html --exclude test
ifeq ($(OS), Windows_NT) 
	start $(DIR_BUILD)report_${LIB_NAME}/index.html
else
	open $(DIR_BUILD)report_${LIB_NAME}/index.html
endif

$(LIB): $(DIR_BUILD)$(LIB)

$(DIR_BUILD)$(LIB): $(LIB_OBJS)
	ar r $@ $(LIB_OBJS)
	ranlib $@


$(LIB_OBJS): $(LIB_SRC)
	$(CC) $(CFLAGS) $(DIR_LIB)$(patsubst %.o,%.c,$(notdir $@)) $(GCOV) -o $@

$(TEST_OBJS): $(TEST_SRC)
	$(CC) $(CFLAGS) $(DIR_TESTS)$(patsubst %.o,%.c,$(notdir $@)) -o $(DIR_BUILD)$(notdir $@) 

lib: $(LIB)

test_nl: $(LIB) $(TEST_OBJS)

ifeq ($(OS),Darwin)
	$(eval OS_MAC_UX=-L/opt/homebrew/lib)
endif
	$(CC) $(FLAGS) $(GCOV) $(TEST_OBJS) -o $(DIR_BUILD)$(LIB_NAME)_test $(LFLAGS) -l$(LIB_NAME) $(TSTFLAGS) $(OS_MAC_UX) -L. -L$(DIR_BUILD)

ext1_flag:
#	add parametrs
	$(eval GCOV=-fprofile-arcs -ftest-coverage -coverage)

leak_check: clean test_nl
ifeq ($(OS),Darwin)
	leaks -atExit -- $(DIR_BUILD)$(LIB_NAME)_test
else
	-valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -s $(DIR_BUILD)$(LIB_NAME)_test
endif
	



