# Linux, Darwin or Windows_NT
OS=$(shell uname -s)
CC := gcc
FLAGS := -I inc -std=c99 -Wall -Wpedantic -fstack-protector-strong -g #-Werror
CFLAGS:=$(FLAGS) -c
LFLAGS = -lncurses
TSTFLAGS = -lcheck
PROG_NAME := tetris
SHELL = /bin/sh
DIR_BUILD=$(shell pwd)/out/
DIR_INSTALL=~/games/
DIR_TESTS=tests/
DIR_IF=gui/cli/
DIR_LIB=brick_game/tetris/
TEST_SRC=$(wildcard tests/*.c)
IF_SRC=$(wildcard gui/cli/*.c)
LIB_SRC=$(wildcard brick_game/tetris/*.c) # https://runebook.dev/ru/docs/gnu_make/wildcard-function
TEST_OBJS=$(addprefix $(DIR_BUILD),$(notdir $(TEST_SRC:.c=.o)))
IF_OBJS=$(addprefix $(DIR_BUILD),$(notdir $(IF_SRC:.c=.o)))
LIB_OBJS:=$(addprefix $(DIR_BUILD),$(notdir $(LIB_SRC:.c=.o)))
LIB = libtetris.a
LIB_NAME = tetris

OS_MAC_UX=-lsubunit -lm
RM:=-rm -rf

.phony: all, install, uninstall, clean, dvi, dist, test, gcov_report $(PROG_NAME) $(LIB) ext1_flag 

all: clang clean $(LIB) test $(PROG_NAME)

install: $(PROG_NAME) 
	-mkdir $(DIR_INSTALL)
	cp $(DIR_BUILD)$(PROG_NAME) $(DIR_INSTALL)

uninstall: 
	$(RM) $(DIR_INSTALL)$(PROG_NAME) $(DIR_INSTALL)tetris_hs.conf
	

clean:
	$(RM) $(DIR_BUILD)*.o $(DIR_BUILD)*.info $(DIR_BUILD)*.gcov $(DIR_BUILD)*.gcda $(DIR_BUILD)*.gcno $(DIR_BUILD)$(LIB) $(DIR_BUILD)$(PROG_NAME) $(DIR_BUILD)$(PROG_NAME)_test

dvi:

dist: $(PROG_NAME)
	echo tar-`sed \
	-e '/version_string/!d' \
	-e 's/[^0-9.]*\([0-9.]*\).*/\1/' \
	-e q version.c` > .fname
	-rm -rf `cat .fname`
	mkdir `cat .fname`
	ln $(DIR_BUILD)$(PROG_NAME) `cat .fname`
	tar chzf `cat .fname`.tar.gz `cat .fname`
	-rm -rf `cat .fname` .fname

test: test_nl
	$(DIR_BUILD)$(PROG_NAME)_test

gcov_report: clean ext1_flag $(LIB) test
	-lcov -t "$(LIB_NAME)_report" -o $(DIR_BUILD)$(LIB_NAME)_report.info -c -d $(DIR_BUILD)
	-geninfo $(DIR_BUILD) -o $(DIR_BUILD)$(LIB_NAME)_report.info
	-genhtml -o $(DIR_BUILD)report $(DIR_BUILD)$(LIB_NAME)_report.info
	-mkdir $(DIR_BUILD)report
	cd $(DIR_BUILD)
	-@python3 -m gcovr -r . --html --html-details -o $(DIR_BUILD)report/index.html --exclude test
ifeq ($(OS), Windows_NT) 
	start $(DIR_BUILD)report/index.html
else
	open $(DIR_BUILD)report/index.html
endif

$(LIB): $(DIR_BUILD)$(LIB)

$(PROG_NAME): $(DIR_BUILD)$(PROG_NAME)

$(DIR_BUILD)$(LIB): $(LIB_OBJS)
	ar r $@ $(LIB_OBJS)
	ranlib $@

$(DIR_BUILD)$(PROG_NAME): $(LIB) $(IF_OBJS)
	$(CC) $(FLAGS) $(GCOV) $(IF_OBJS) -l$(LIB_NAME) -o $(DIR_BUILD)$(PROG_NAME) $(LFLAGS) $(OS_MAC_UX) -L. -L$(DIR_BUILD)

$(IF_OBJS): $(IF_SRC)
	$(CC) $(CFLAGS) $(DIR_IF)$(patsubst %.o,%.c,$(notdir $@)) -o $(DIR_BUILD)$(notdir $@)

$(LIB_OBJS): $(LIB_SRC)
	$(CC) $(CFLAGS) $(DIR_LIB)$(patsubst %.o,%.c,$(notdir $@)) $(GCOV) -o $(DIR_BUILD)$(notdir $@)

$(TEST_OBJS): $(TEST_SRC)
	$(CC) $(CFLAGS) $(DIR_TESTS)$(patsubst %.o,%.c,$(notdir $@)) -o $(DIR_BUILD)$(notdir $@) 

lib: $(LIB)

test_nl: $(LIB) $(TEST_OBJS)

ifeq ($(OS),Darwin)
	$(eval OS_MAC_UX=-L/opt/homebrew/lib)
endif
	$(CC) $(FLAGS) $(GCOV) $(TEST_OBJS) -o $(DIR_BUILD)$(PROG_NAME)_test $(LFLAGS) -l$(LIB_NAME) $(TSTFLAGS) $(OS_MAC_UX) -L. -L$(DIR_BUILD)

ext1_flag:
#	add parametrs
	$(eval GCOV=-fprofile-arcs -ftest-coverage -coverage)

leak_check: clean test_nl
ifeq ($(OS),Darwin)
	leaks -atExit -- $(DIR_BUILD)$(LIB_NAME)_test
else
	-valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -s $(DIR_BUILD)$(LIB_NAME)_test
endif
	
	
control: leak_check clean 
	-cppcheck --enable=all --suppress=missingIncludeSystem --error-exitcode=1 *.c *.h tests/*.c tests/*.h *.c *.h dev/*.c dev/*.h
	-clang-format --Werror -n -style=file:../materials/linters/.clang-format tests/*.c tests/*.h *.c *.h dev/*.c dev/*.h
	-cd ../materials/build && sudo ./run.sh
	
clang:
#	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i -style=file:../materials/linters/.clang-format {} \;

check_format: 
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -n -style=file:../materials/linters/.clang-format {} \;
	cppcheck --enable=all --suppress=missingIncludeSystem .


