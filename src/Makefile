# Linux, Darwin or Windows_NT
OS=$(shell uname -s)
CC := gcc
FLAGS := -I inc -std=c99 -Wall -Wpedantic -fstack-protector-strong -g #-Werror
CFLAGS:=$(FLAGS) -c
LFLAGS = -lncurses
TSTFLAGS = -lcheck
SHELL = /bin/sh
DIR_BUILD=$(shell pwd)/out/
DIR_INSTALL=~/games/
DIR_TESTS=tests/
DIR_IF=gui/cli/
TEST_SRC=$(wildcard tests/*.c)
IF_SRC=$(wildcard gui/cli/*.c)
TEST_OBJS=$(addprefix $(DIR_BUILD),$(notdir $(TEST_SRC:.c=.o)))
IF_OBJS=$(addprefix $(DIR_BUILD),$(notdir $(IF_SRC:.c=.o)))

OS_MAC_UX=-lsubunit -lm
RM:=-rm -rf

.phony: all, install, uninstall, clean, dvi, dist, test, gcov_report $(PROG_NAME) ext1_flag 

all: clang clean libs test tetris snake

install: $(PROG_NAME) 
	-mkdir $(DIR_INSTALL)
	cp $(DIR_BUILD)$(PROG_NAME) $(DIR_INSTALL)

uninstall: 
	$(RM) $(DIR_INSTALL)$(PROG_NAME) $(DIR_INSTALL)*_hs.conf

clean:
	$(RM) $(DIR_BUILD)*.o $(DIR_BUILD)*.info $(DIR_BUILD)*.gcov $(DIR_BUILD)*.gcda $(DIR_BUILD)*.gcno
	$(MAKE) -C brick_game/tetris clean
	$(MAKE) -C brick_game/snake clean
	$(RM) $(DIR_BUILD)*

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

games: tetris snake

snake: $(eval PROG_NAME=snake) $(eval MACROS=SNAKE) lib_$(PROG_NAME) $(DIR_BUILD)$(PROG_NAME)


tetris: $(eval PROG_NAME=tetris) $(eval MACROS=TETRIS) lib_$(PROG_NAME) $(DIR_BUILD)$(PROG_NAME)


$(DIR_BUILD)$(PROG_NAME): $(LIB) $(IF_OBJS)
	$(CC) $(FLAGS) $(GCOV) $(IF_OBJS) -D$(MACROS) -o $(DIR_BUILD)$(PROG_NAME) -l$(PROG_NAME) $(LFLAGS) $(OS_MAC_UX) -L. -L$(DIR_BUILD)

$(IF_OBJS): $(IF_SRC)
	$(CC) $(CFLAGS) $(DIR_IF)$(patsubst %.o,%.c,$(notdir $@)) -o $(DIR_BUILD)$(notdir $@)

$(TEST_OBJS): $(TEST_SRC)
	$(CC) $(CFLAGS) $(DIR_TESTS)$(patsubst %.o,%.c,$(notdir $@)) -o $(DIR_BUILD)$(notdir $@) 

libs:  lib_tetris lib_snake

lib_tetris:
	$(MAKE) -C brick_game/tetris lib

lib_snake:
	$(MAKE) -C brick_game/snake lib

test: test_tetris test_snake

test_tetris:
	$(MAKE) -C brick_game/tetris test

test_snake:
	$(MAKE) -C brick_game/snake test

ext1_flag:
#	add parametrs
	$(eval GCOV=-fprofile-arcs -ftest-coverage -coverage)
	
clang:
#	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;

check_format: 
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -n {} \;
	cppcheck --enable=all --suppress=missingIncludeSystem .
