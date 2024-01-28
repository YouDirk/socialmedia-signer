# Socialmedia Signer, sign and verify social media posts.
# Copyright (C) 2024  Dirk Lehmann
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


OUTPUT := socialmedia-signer
OBJ := main

DEBUGFLAGS := -g
OPTFLAG := -O0
LD_PRELOADS := /usr/lib/x86_64-linux-gnu/libc_malloc_debug.so

INCLUDE_PATHS :=
LD_PATHS :=
LIBS :=

MTRACEFILE := mtrace.log
EBROWSEFILE := BROWSE
CTAGSFILE := tags
ETAGSFILE := TAGS
MAKEFILEZ := Makefile

CC := g++
AS := g++
LD := g++
DEBUGGER := gdb
MTRACE := mtrace
EBROWSE := ebrowse
CTAGS := ctags
ETAGS := etags

CEXT := cpp
HEXT := hpp
SEXT := S
CCEXT := cc
ASMEXT := asm
OEXT := o
DEPEXT := d
LOGEXT := log

OBJFILES := $(OBJ:=.$(OEXT))
DEPFILES := $(OBJ:=.$(DEPEXT))

FLAGS := $(DEBUGFLAGS) -Wall -Wextra -Wformat-security $(OPTFLAG)
CCFLAGS := $(FLAGS) -DDEBUG $(addprefix -I,$(INCLUDE_PATHS))
ASFLAGS := $(CCFLAGS)
LDFLAGS := $(FLAGS) $(addprefix -L,$(LD_PATHS))
DEBUGGERFLAGS := --quiet

NULLCHAR :=
RUN_ENV := \
  LD_PRELOAD=$$LD_PRELOAD:$(subst $(NULLCHAR) ,:,$(LD_PRELOADS))
  LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(subst $(NULLCHAR) ,:,$(LD_PATHS))

TAGEDFILES := $(wildcard *.$(CEXT) *.$(HEXT) *.$(SEXT))

CTAGSFLAGS :=
ETAGSFLAGS :=
EBROWSEFLAGS :=

MAKEDEP := $(CC) $(CCFLAGS) -M

.PHONY: all
all: $(OUTPUT)

.PHONY: recompile
recompile: clean all

.PHONY: run
run: all
	@-rm -f $(MTRACEFILE)
	$(RUN_ENV) MALLOC_TRACE=$(MTRACEFILE) ./$(OUTPUT) $(ARGS)
	test -f $(MTRACEFILE) \
	  && $(MTRACE) $(OUTPUT) $(MTRACEFILE) 2> /dev/null || true

.PHONY: debug
debug: all
	$(RUN_ENV) $(DEBUGGER) $(DEBUGGERFLAGS) $(OUTPUT)

# Set in GDB shell: 'set environment LD_LIBRARY_PATH=../lib'

.PHONY: debug-emacs
debug-emacs: all
	$(RUN_ENV) $(DEBUGGER) -i=mi $(DEBUGGERFLAGS) $(OUTPUT)

.PHONY: ctags
ctags: $(CTAGSFILE)

.PHONY: etags
etags: $(ETAGSFILE)

.PHONY: ebrowse
ebrowse: $(EBROWSEFILE)

.PHONY: all-tags
all-tags: ctags etags ebrowse

.PHONY: clean-deps
clean-deps:
	-rm -f *.$(DEPEXT)

.PHONY: clean-tags
clean-tags:
	-rm -f $(CTAGSFILE) $(ETAGSFILE) $(EBROWSEFILE)

.PHONY: clean
clean: clean-deps
	-rm -rf *.$(CCEXT) *.$(ASMEXT) *.$(OEXT) *.$(LOGEXT) *~ *.bak

.PHONY: clean-all
clean-all: clean clean-tags
	-rm -f $(OUTPUT)

%.$(DEPEXT): %.$(CEXT) $(MAKEFILEZ)
	@-$(MAKEDEP) -E -MQ $*.$(OEXT) -o $@ $<
%.$(DEPEXT): %.$(SEXT) $(MAKEFILEZ)
	@-$(MAKEDEP) -E -MQ $*.$(OEXT) -o $@ $<

%.$(CCEXT): %.$(CEXT) $(MAKEFILEZ)
	$(CC) -E $(CCFLAGS) -o $@ $<
%.$(ASMEXT): %.$(CEXT) $(MAKEFILEZ)
	$(CC) -S $(CCFLAGS) -o $@ $<
%.$(OEXT): %.$(CEXT) $(MAKEFILEZ)
	$(CC) -c $(CCFLAGS) -o $@ $<
%.$(OEXT): %.$(SEXT) $(MAKEFILEZ)
	$(AS) -c $(ASFLAGS) -o $@ $<

$(CTAGSFILE): $(TAGEDFILES)
	$(CTAGS) $(CTAGSFLAGS) -o $@ $^

$(ETAGSFILE): $(TAGEDFILES)
	$(ETAGS) $(ETAGSFLAGS) -o $@ $^

$(EBROWSEFILE): $(TAGEDFILES)
	$(EBROWSE) $(EBROWSEFLAGS) -o $@ $^

$(OUTPUT): $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(addprefix -l,$(LIBS))

-include $(DEPFILES)
