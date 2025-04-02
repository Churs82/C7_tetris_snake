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
LIBS=$(addprefix $(DIR_BUILD),$(notdir $(wildcard gui/*)))
GAMES=$(addprefix $(DIR_BUILD),$(notdir $(wildcard brick_game/*)))

OS_MAC_UX=-lsubunit -lm
RM:=-rm -rf

ifeq ($(OS),Darwin)
OPEN_CMD = open
endif

ifeq ($(OS),Linux)
OPEN_CMD = xdg-open
ADD_LDFLAGS = $(OS_MAC_UX)
endif

.phony: all, install, uninstall, clean, dvi, dist, test, gcov_report, clang 

all: clang clean libs test games

install: games 
	-mkdir $(DIR_INSTALL)
	for game in $(GAMES); do \
        cp $$game $(DIR_INSTALL); \
    done

uninstall:
	for game in $(addprefix $(DIR_INSTALL),$(notdir $(GAMES))); do \
        $(RM) $$game $$game_hs.conf; \
    done 

clean:
	$(RM) $(DIR_BUILD)*.o $(DIR_BUILD)*.info $(DIR_BUILD)*.gcov $(DIR_BUILD)*.gcda $(DIR_BUILD)*.gcno
	$(MAKE) -C brick_game/tetris clean
	$(MAKE) -C brick_game/snake clean
	$(RM) $(DIR_BUILD)*

# open custom documentation
dvi: doxygen_doc 
	$(OPEN_CMD) documentation/Documentation.html

# generate doxygen documentation from source code comments
doxygen_doc: documentation/Doxyfile
	doxygen documentation/Doxyfile

# open doxygen documentation
doxygen_open: doxygen_doc
	$(OPEN_CMD) documentation/html/index.html

dist: $(GAMES)
	echo tar-`sed \
	-e '/version_string/!d' \
	-e 's/[^0-9.]*\([0-9.]*\).*/\1/' \
	-e q version.c` > .fname
	-rm -rf `cat .fname`
	mkdir `cat .fname`
	for game in $(GAMES); do \
		ln $$game `cat .fname`; \
		tar chzf `cat .fname`.tar.gz `cat .fname`; \
		-rm -rf `cat .fname` .fname; \
	done

gcov_report: clean ext1_flag libs test
	for lib in $(notdir $(LIBS)); do \
		-lcov -t "$$lib_report" -o $(DIR_BUILD)$$lib_report.info -c -d $(DIR_BUILD); \
		-geninfo $(DIR_BUILD) -o $(DIR_BUILD)$$lib_report.info; \
		-genhtml -o $(DIR_BUILD)report $(DIR_BUILD)$$lib_report.info; \
		-mkdir $(DIR_BUILD)report; \
	done
	cd $(DIR_BUILD)
	-@python3 -m gcovr -r . --html --html-details -o $(DIR_BUILD)report/index.html --exclude test
	$(OPEN_CMD) $(DIR_BUILD)report/index.html

libs: $(LIBS)

$(LIBS):
	$(MAKE) -C $@ lib

$(GAMES):
	$(MAKE) -C $@ $(LIBS)

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
