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

# Debug build?  Will be overridden by CI/CD.
#
# Possible values: [1, 0], default: 1
DEBUG                            := 1

# See 'Semantic Versioning 2.0.0' for more details.  Will be
# overridden by CI/CD.
#
CONFIG_VERSION                   := 0.1.0-dev

# --------------------------------------------------------------------

# Compile to GUI version or to command-line tool?  Will be overridden
# by CI/CD.
#
# Possible values: [1, 0], default: 1
CONFIG_GUI                       := 0

# ********************************************************************
# To check which libraries will be loaded dynamically during runtime,
# after `$> make -j`, try:
#
#   $> ldd src/socialmedia-signer

# Link LIBCRYPTO and LIBSSL static into executable.
#
# Possible values: [1, 0], default: 0
CONFIG_STATIC_CRYPTO             := 0

# Link LIBSTDC++ static into executable.
#
# Possible values: [1, 0], default: 1
CONFIG_STATIC_LIBSTDCPP          := 1

# Link LIBGCC static into executable.
#
# Possible values: [1, 0], default: 1
CONFIG_STATIC_LIBGCC             := 1

# (Not recommended) Link a NON-DYNAMIC EXECUTABLE.
#
# At least the static call to GETADDRINFO() and GETHOSTBYNAME()
# enforces to load the dynamic (G)LIBC during runtime using the (not
# statically linked) library call LIBDL::DLOPEN().
#
# Therefore, it results in a very big executable and during runtime
# the dynamic (G)LIBC load will be enforced.
#
# Possible values: [1, 0], default: 0
CONFIG_STATIC_ALL                := 0

# ********************************************************************
