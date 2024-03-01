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


SUBDIRS := src

# ********************************************************************
# Feature check stuff.

_BLANK :=
define NL

$(_BLANK)
endef
ERRB  = $(NL)$(NL)  ERROR:
WARNB = $(NL)$(NL)  warning:

_CMD_TEST = \
  $(eval _CMD_TEST_BUF:=checking for $(2)... )$(\
  )$(shell which $(1) 2> /dev/null)
_HEADER_TEST = \
  $(eval _CMD_TEST_BUF:=checking for header $(1)... )$(\
  )$(shell ls /usr/include/$(1) 2> /dev/null)
_LIB_TEST = \
  $(eval _CMD_TEST_BUF:=checking for library lib$(1)... )$(\
  )$(shell $(LDCONFIG) -p | $(SED) -n \
             '/^[\t ]*lib$(1)\./{s~^[\t ]*\([^ ]\+\).*$$~\1~;p}' \
             2> /dev/null)

_CMD_TEST_RESULT = $(info $(_CMD_TEST_BUF)$(1))
_CMD_TEST_RESNO = $(info $(_CMD_TEST_BUF)no - Debian package $(1))
_CMD_TEST_RESNO_ERR = $(error $(ERRB) Run (Debian): $$> apt-get install \
  $(1)$(NL)$(NL) $(2) not found!  Install and try again)

# ********************************************************************
# Common variables

# No .MAKEFILE.CACHE.MK here, because: .MAKEFILE.CACHE.MK depends on
# $(MAKEFILEZ).
MAKEFILEZ := ../Makefile ../makefile.config.mk \
  ../makeinc/makefile.variables.mk ../makeinc/makefile.inc.mk \
  ../makeinc/makefile.check.mk \
  \
  $(patsubst %,../%/Makefile,$(SUBDIRS))

MAKE_CACHEFILE := ../.makefile.cache.mk

# ********************************************************************

# At least C++20 is required to enable the UTF-8 specific type char8_t.
CCSTDFLAG      := -std=c++20

DFLAGS         := -DCONFIG_VERSION='"$(CONFIG_VERSION)"'
DEBUGFLAGS     :=
OPTFLAG        := -O3
LD_PRELOADS    :=

INCLUDE_PATHS  :=
LD_PATHS       :=
LIBS           :=

ifneq (0,$(DEBUG))
  OPTFLAG        := -O0

  DFLAGS         += -DDEBUG
  DEBUGFLAGS     += -g

  ifneq (,$(MTRACE_OPT))
    LD_PRELOADS  += libc_malloc_debug.so

    HAS_MTRACE   := 1
    DFLAGS       += -DHAS_MTRACE
  endif
endif

ifneq (0,$(CONFIG_GUI))
  DFLAGS         += -DCONFIG_GUI
endif

# ********************************************************************

ifneq (0,$(CONFIG_STATIC_ALL))
  LIBS           += -static
endif

ifneq (0,$(CONFIG_STATIC_LIBGCC))
  LIBS           += -static-libgcc

  ifeq (0,$(CONFIG_STATIC_LIBSTDCPP))
    $(error $(ERRB) Dynamic LIBSTDC++ is depending dynamic LIBGCC! \
      But CONFIG_STATIC_LIBSTDCPP is set in makefile.config.mk)
  endif
endif

ifneq (0,$(CONFIG_STATIC_LIBSTDCPP))
  LIBS           += -l:libstdc++.a
endif

ifneq (0,$(CONFIG_STATIC_CRYPTO))
  LIBS           += -l:libssl.a -l:libcrypto.a
else
  LIBS           += -lssl -lcrypto
endif

# ********************************************************************

MTRACEFILE     := mtrace.log
EBROWSEFILE    := BROWSE
CTAGSFILE      := tags
ETAGSFILE      := TAGS

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
CCFLAGS := \
  $(FLAGS) $(CCSTDFLAG) $(DFLAGS) $(addprefix -I,$(INCLUDE_PATHS))
ASFLAGS := $(CCFLAGS)
LDFLAGS := $(FLAGS) $(addprefix -L,$(LD_PATHS))
DEBUGGERFLAGS  := --quiet

RUN_ENV        := \
  LD_PRELOAD=$$LD_PRELOAD:$(subst $(_BLANK) ,:,$(LD_PRELOADS)) \
  LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(subst $(_BLANK) ,:,$(LD_PATHS))

TAGEDFILES     := $(wildcard *.$(CEXT) *.$(HEXT) *.$(SEXT))

CTAGSFLAGS     :=
ETAGSFLAGS     :=
EBROWSEFLAGS   :=

MAKEDEP        := $(CC) $(CCFLAGS) -M

# End of Common variables
# ********************************************************************
