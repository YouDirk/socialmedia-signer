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


include ../.makefile.cache.mk

include ../makeinc/makefile.variables.mk

# ********************************************************************
# Targets

.PHONY: all
all: $(OUTPUT)

.PHONY: recompile
recompile: clean all

.PHONY: run
run: all
	@-rm -f $(MTRACEFILE)
	$(RUN_ENV) MALLOC_TRACE=$(MTRACEFILE) ./$(OUTPUT) $(ARGS)
ifneq (,$(HAS_MTRACE))
	$(MTRACE_OPT) $(OUTPUT) $(MTRACEFILE) || true
endif

.PHONY: debug
debug: all
ifeq (,$(GDB_OPT))
	$(call _CMD_TEST_RESNO_ERR,gdb,debugger)
else
	$(RUN_ENV) $(GDB_OPT) $(DEBUGGERFLAGS) --args $(OUTPUT) $(ARGS)
endif

# Set in GDB shell: 'set environment LD_LIBRARY_PATH=../lib'

.PHONY: debug-emacs
debug-emacs: all
ifeq (,$(GDB_OPT))
	$(call _CMD_TEST_RESNO_ERR,gdb,debugger)
else
	$(RUN_ENV) $(GDB_OPT) -i=mi $(DEBUGGERFLAGS) --args $(OUTPUT) \
	  $(ARGS)
endif

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
	-rm -f $(MAKE_CACHEFILE) $(OUTPUT)

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
ifeq (,$(AS_OPT))
	$(call _CMD_TEST_RESNO_ERR,g++,assembler)
else
	$(AS_OPT) -c $(ASFLAGS) -o $@ $<
endif

$(CTAGSFILE): $(TAGEDFILES)
ifeq (,$(CTAGS_OPT))
	$(call _CMD_TEST_RESNO_ERR,emacs-bin-common,CTAGS command)
else
	$(CTAGS_OPT) $(CTAGSFLAGS) -o $@ $^
endif

$(ETAGSFILE): $(TAGEDFILES)
ifeq (,$(ETAGS_OPT))
	$(call _CMD_TEST_RESNO_ERR,emacs-bin-common,ETAGS command)
else
	$(ETAGS_OPT) $(ETAGSFLAGS) -o $@ $^
endif

$(EBROWSEFILE): $(TAGEDFILES)
ifeq (,$(EBROWSE_OPT))
	$(call _CMD_TEST_RESNO_ERR,emacs-bin-common,EBROWSE command)
else
	$(EBROWSE_OPT) $(EBROWSEFLAGS) -o $@ $^
endif

$(OUTPUT): $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(addprefix -l,$(LIBS))

-include $(DEPFILES)

# --------------------------------------------------------------------

$(MAKE_CACHEFILE): $(MAKEFILEZ)
	echo > $@ && $(MAKE) -f ../makeinc/makefile.check.mk

# End of Targets
# ********************************************************************
