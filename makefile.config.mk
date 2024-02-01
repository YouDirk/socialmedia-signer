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

# Debug build?  Possible values: [1, 0], default: 1
#
# Will be overridden by CI/CD.
DEBUG               := 1

# See 'Semantic Versioning 2.0.0' for more details.
#
# Will be overridden by CI/CD.
CONFIG_VERSION      := 0.1.0-dev

# --------------------------------------------------------------------

# Compile to GUI version or to command-line tool?
#
# Possible values: [1, 0], default: 1
CONFIG_GUI          := 0

# ********************************************************************
