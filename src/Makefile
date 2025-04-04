# Linux, Darwin or Windows_NT
OS:=$(shell uname -s)
SHELL:=/bin/sh
PWD:=$(shell pwd)
export DIR_BUILD:=$(PWD)/out/
export DIR_GAMES:=$(PWD)/brick_game/
export DIR_IF:=$(PWD)/gui/

DIR_INSTALL:=~/games/
GAMES:=$(notdir $(wildcard $(DIR_GAMES)*))
LIBS:=$(addprefix lib, $(GAMES))
INTERFACES:=$(notdir $(wildcard $(DIR_IF)*))

OS_MAC_UX:=-lsubunit -lm
RM:=-rm -rf

ifeq ($(OS),Darwin)
OPEN_CMD := open
endif

ifeq ($(OS),Linux)
OPEN_CMD := xdg-open
ADD_LDFLAGS = $(OS_MAC_UX)
endif

.phony: all, install, uninstall, clean, dvi, dist, test, gcov_report, clang 

all: clang clean libs test games

install: games 
	-mkdir $(DIR_INSTALL)
	for game in $(GAMES); do \
		for interface in $(INTERFACES); do \
        	cp $(DIR_BUILD)$${game}_$$interface $(DIR_INSTALL); \
		done \
    done

uninstall:
	for game in $(GAMES); do \
		for interface in $(INTERFACES); do \
        	$(RM) $(DIR_INSTALL)$$game$$interface $(DIR_INSTALL)$$game_hs.conf; \
		done \
    done

clean:
	for game in $(GAMES); do \
		$(MAKE) -C $(DIR_GAMES)$$game clean; \
	done 
	$(RM) $(DIR_BUILD)*
	@echo "***===Cleaned===***"

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

gcov_report: clean ext1_flag test
	-mkdir $(DIR_BUILD)report
	for lib in $(GAMES); do \
		lcov -t "lib$${lib}_report" -o $(DIR_BUILD)lib$${lib}_report.info -c -d $(DIR_GAMES)$$lib; \
		geninfo $(DIR_GAMES)$$lib -o $(DIR_BUILD)lib$${lib}_report.info; \
		genhtml -o $(DIR_BUILD)report $(DIR_BUILD)lib$${lib}_report.info; \
		echo "-----------------------------Trying GCOV----------------------------"; \
		python3 -m gcovr --verbose -r $(DIR_GAMES)$$lib --html --html-details -o $(DIR_BUILD)report/index.html --exclude test; \
	done

	$(OPEN_CMD) $(DIR_BUILD)report/index.html

libs:  $(LIBS)

$(LIBS): 
	$(MAKE) -C $(DIR_GAMES)$(patsubst lib%,%,$@) lib; \

games: $(GAMES)

$(GAMES): 
	$(MAKE) -C $(DIR_GAMES)$@ lib
	for interface in $(INTERFACES); do \
		$(MAKE) -C $(DIR_IF)$$interface $@; \
	done 

test: libs
	for lib in $(GAMES); do \
		$(MAKE) -C $(DIR_GAMES)$$lib test; \
	done

ext1_flag:
	$(eval export COV_FLAGS:=-ftest-coverage -coverage)
	
clang:
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;

check_format: 
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -n {} \;
	cppcheck --enable=all --suppress=missingIncludeSystem .
