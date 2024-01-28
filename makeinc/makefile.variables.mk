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


# ********************************************************************
# Common variables

SUBDIRS := src

# No .MAKEFILE.CACHE.MK here, because: .MAKEFILE.CACHE.MK depends on
# $(MAKEFILEZ).
MAKEFILEZ := ../Makefile ../makefile.config.mk \
  ../makeinc/makefile.variables.mk ../makeinc/makefile.inc.mk \
  ../makeinc/makefile.check.mk \
  \
  $(patsubst %,../%/Makefile,$(SUBDIRS))

# ********************************************************************

DEBUGFLAGS     := -g
OPTFLAG        := -O0
LD_PRELOADS    := /usr/lib/x86_64-linux-gnu/libc_malloc_debug.so

INCLUDE_PATHS  :=
LD_PATHS       :=
LIBS           :=

MTRACEFILE     := mtrace.log
EBROWSEFILE    := BROWSE
CTAGSFILE      := tags
ETAGSFILE      := TAGS

CC := g++
AS := g++
LD := g++
DEBUGGER := gdb
MTRACE := mtrace
EBROWSE := ebrowse
CTAGS := ctags
ETAGS := etags

CEXT           := cpp
HEXT           := hpp
SEXT           := S
CCEXT          := cc
ASMEXT         := asm
OEXT           := o
DEPEXT         := d
LOGEXT         := log

OBJFILES       := $(OBJ:=.$(OEXT))
DEPFILES       := $(OBJ:=.$(DEPEXT))

FLAGS := $(DEBUGFLAGS) -Wall -Wextra -Wformat-security $(OPTFLAG)
CCFLAGS := $(FLAGS) -DDEBUG $(addprefix -I,$(INCLUDE_PATHS))
ASFLAGS := $(CCFLAGS)
LDFLAGS := $(FLAGS) $(addprefix -L,$(LD_PATHS))
DEBUGGERFLAGS  := --quiet

NULLCHAR       :=
RUN_ENV        := \
  LD_PRELOAD=$$LD_PRELOAD:$(subst $(NULLCHAR) ,:,$(LD_PRELOADS)) \
  LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(subst $(NULLCHAR) ,:,$(LD_PATHS))

TAGEDFILES     := $(wildcard *.$(CEXT) *.$(HEXT) *.$(SEXT))

CTAGSFLAGS     :=
ETAGSFLAGS     :=
EBROWSEFLAGS   :=

MAKEDEP        := $(CC) $(CCFLAGS) -M

# End of Common variables
# ********************************************************************
