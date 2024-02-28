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


include ../makeinc/makefile.variables.mk

# ********************************************************************
# Linux commands, feature check

CC := $(call _CMD_TEST,g++,C++ compiler)
ifeq (,$(CC))
  $(shell rm -f $(MAKE_CACHEFILE))
  $(call _CMD_TEST_RESNO_ERR,g++,C++ compiler)
else
  $(call _CMD_TEST_RESULT,$(CC))
  $(shell echo 'CC = $(CC)' >> $(MAKE_CACHEFILE))
endif

LD := $(call _CMD_TEST,g++,linker)
ifeq (,$(LD))
  $(shell rm -f $(MAKE_CACHEFILE))
  $(call _CMD_TEST_RESNO_ERR,g++,linker)
else
  $(call _CMD_TEST_RESULT,$(LD))
  $(shell echo 'LD = $(LD)' >> $(MAKE_CACHEFILE))
endif

SED := $(call _CMD_TEST,sed,sed)
ifeq (,$(SED))
  $(shell rm -f $(MAKE_CACHEFILE))
  $(call _CMD_TEST_RESNO_ERR,sed,SED command)
else
  $(call _CMD_TEST_RESULT,$(SED))
  $(shell echo 'SED = $(SED)' >> $(MAKE_CACHEFILE))
endif

LDCONFIG := $(call _CMD_TEST,ldconfig,ldconfig)
ifeq (,$(LDCONFIG))
  $(shell rm -f $(MAKE_CACHEFILE))
  $(call _CMD_TEST_RESNO_ERR,libc-bin,LDCONFIG command)
else
  $(call _CMD_TEST_RESULT,$(LDCONFIG))
  $(shell echo 'LDCONFIG = $(LDCONFIG)' >> $(MAKE_CACHEFILE))
endif

LIBCRYPTO := $(call _LIB_TEST,libcrypto)
ifeq (,$(LIBCRYPTO))
  $(shell rm -f $(MAKE_CACHEFILE))
  $(call _CMD_TEST_RESNO_ERR,libssl3,LIBCRYPTO library)
else
  $(call _CMD_TEST_RESULT,$(LIBCRYPTO))
  $(shell echo 'LIBCRYPTO = $(LIBCRYPTO)' >> $(MAKE_CACHEFILE))
endif

HEADERCRYPTO := $(call _HEADER_TEST,openssl/crypto.h)
ifeq (,$(HEADERCRYPTO))
  $(shell rm -f $(MAKE_CACHEFILE))
  $(call _CMD_TEST_RESNO_ERR,libssl-dev,openssl/crypto.h)
else
  $(call _CMD_TEST_RESULT,$(HEADERCRYPTO))
  $(shell echo 'HEADERCRYPTO = $(HEADERCRYPTO)' >> $(MAKE_CACHEFILE))
endif

LIBSSL := $(call _LIB_TEST,libssl)
ifeq (,$(LIBSSL))
  $(shell rm -f $(MAKE_CACHEFILE))
  $(call _CMD_TEST_RESNO_ERR,libssl3,LIBSSL library)
else
  $(call _CMD_TEST_RESULT,$(LIBSSL))
  $(shell echo 'LIBSSL = $(LIBSSL)' >> $(MAKE_CACHEFILE))
endif

HEADERSSL := $(call _HEADER_TEST,openssl/ssl3.h)
ifeq (,$(HEADERSSL))
  $(shell rm -f $(MAKE_CACHEFILE))
  $(call _CMD_TEST_RESNO_ERR,libssl-dev,openssl/ssl3.h)
else
  $(call _CMD_TEST_RESULT,$(HEADERSSL))
  $(shell echo 'HEADERSSL = $(HEADERSSL)' >> $(MAKE_CACHEFILE))
endif

# --------------------------------------------------------------------
# optional features

AS_OPT := $(call _CMD_TEST,g++,(optional) assembler)
ifeq (,$(AS_OPT))
  $(call _CMD_TEST_RESNO,g++)
  $(shell echo 'AS_OPT =' >> $(MAKE_CACHEFILE))
else
  $(call _CMD_TEST_RESULT,$(AS_OPT))
  $(shell echo 'AS_OPT = $(AS_OPT)' >> $(MAKE_CACHEFILE))
endif

GDB_OPT := $(call _CMD_TEST,gdb,(optional) debugger)
ifeq (,$(GDB_OPT))
  $(call _CMD_TEST_RESNO,gdb)
  $(shell echo 'GDB_OPT =' >> $(MAKE_CACHEFILE))
else
  $(call _CMD_TEST_RESULT,$(GDB_OPT))
  $(shell echo 'GDB_OPT = $(GDB_OPT)' >> $(MAKE_CACHEFILE))
endif

MTRACE_OPT := $(call _CMD_TEST,mtrace,(optional) mtrace memory leaks)
ifeq (,$(MTRACE_OPT))
  $(call _CMD_TEST_RESNO,libc-devtools)
  $(shell echo 'MTRACE_OPT =' >> $(MAKE_CACHEFILE))
else
  $(call _CMD_TEST_RESULT,$(MTRACE_OPT))
  $(shell echo 'MTRACE_OPT = $(MTRACE_OPT)' >> $(MAKE_CACHEFILE))
endif

CTAGS_OPT := $(call _CMD_TEST,ctags,(optional) ctags)
ifeq (,$(CTAGS_OPT))
  $(call _CMD_TEST_RESNO,emacs-bin-common)
  $(shell echo 'CTAGS_OPT =' >> $(MAKE_CACHEFILE))
else
  $(call _CMD_TEST_RESULT,$(CTAGS_OPT))
  $(shell echo 'CTAGS_OPT = $(CTAGS_OPT)' >> $(MAKE_CACHEFILE))
endif

ETAGS_OPT := $(call _CMD_TEST,etags,(optional) etags)
ifeq (,$(ETAGS_OPT))
  $(call _CMD_TEST_RESNO,emacs-bin-common)
  $(shell echo 'ETAGS_OPT =' >> $(MAKE_CACHEFILE))
else
  $(call _CMD_TEST_RESULT,$(ETAGS_OPT))
  $(shell echo 'ETAGS_OPT = $(ETAGS_OPT)' >> $(MAKE_CACHEFILE))
endif

EBROWSE_OPT := $(call _CMD_TEST,ebrowse,(optional) ebrowse)
ifeq (,$(EBROWSE_OPT))
  $(call _CMD_TEST_RESNO,emacs-bin-common)
  $(shell echo 'EBROWSE_OPT =' >> $(MAKE_CACHEFILE))
else
  $(call _CMD_TEST_RESULT,$(EBROWSE_OPT))
  $(shell echo 'EBROWSE_OPT = $(EBROWSE_OPT)' >> $(MAKE_CACHEFILE))
endif

# End of Linux commands, feature check
# ********************************************************************

.PHONY: _cache
_cache:
