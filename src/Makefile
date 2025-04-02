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
        	cp $(DIR_BUILD)$$game$$interface $(DIR_INSTALL); \
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
	for lib in $(notdir $(LIBS)); do \
		lcov -t "$$lib_report" -o $(DIR_BUILD)$$lib_report.info -c -d $(DIR_BUILD); \
		geninfo $(DIR_BUILD) -o $(DIR_BUILD)$$lib_report.info; \
		genhtml -o $(DIR_BUILD)report $(DIR_BUILD)$$lib_report.info; \
	done
	@echo "-----------------------------Trying GCOV----------------------------"
	cd $(DIR_BUILD)
	-@python3  -m gcovr --verbose -r $(DIR_BUILD) --html --html-details -o $(DIR_BUILD)report/index.html --exclude test
	$(OPEN_CMD) $(DIR_BUILD)report/index.html

libs:  
	for lib in $(GAMES); do \
		$(MAKE) -C $(DIR_GAMES)$$lib lib; \
	done

games:
	for interface in $(INTERFACES); do \
		$(MAKE) -C $(DIR_IF)$$interface $(GAMES); \
	done 

test: libs
	for lib in $(GAMES); do \
		$(MAKE) -C $(DIR_GAMES)$$lib test; \
	done

ext1_flag:
	$(eval export GCOV:=-fprofile-arcs -ftest-coverage -coverage)
	
clang:
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;

check_format: 
	find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -n {} \;
	cppcheck --enable=all --suppress=missingIncludeSystem .
