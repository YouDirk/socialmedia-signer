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


SUBDIR_MAKE  := src
SUBDIR_CLEAN := makeinc

.PHONY: all run debug clean clean-all
all run debug: $(OUTPUT)
	$(MAKE) -C $(SUBDIR_MAKE) $@

.PHONY: _clean clean
_clean:
	-rm -f .gitignore~ $(SUBDIR_CLEAN)/*~ $(SUBDIR_CLEAN)/*.bak \
	   *~ *.bak
clean: _clean
	$(MAKE) -C $(SUBDIR_MAKE) $@

.PHONY: clean-all
clean-all: _clean
	$(MAKE) -C $(SUBDIR_MAKE) $@
