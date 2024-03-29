/* Socialmedia Signer, sign and verify social media posts.
 * Copyright (C) 2024  Dirk Lehmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef COMMON_HPP__
#define COMMON_HPP__

/* ***************************************************************  */

#include "config.hpp"

/* ***************************************************************  */

#include <stdlib.h>

#ifdef HAS_MTRACE
#  include <mcheck.h>
#  define MTRACE()                 mtrace()
#  define MUNTRACE()               muntrace()
#else
#  define MTRACE()
#  define MUNTRACE()
#endif

/* ***************************************************************  */

#include "ustr.hpp"

#include "Log.hpp"
#include "Error.hpp"
#include "Success.hpp"

/* ***************************************************************  */

#define COMMON_APP_NAME            u8"Socialmedia Signer"
#define COMMON_APP_DESC \
  u8"Sign your social media posts and verify other posts."

#define COMMON_BUGTRACKING_URL \
  u8"https://github.com/YouDirk/socialmedia-signer/issues"

/* ***************************************************************  */

#endif /* COMMON_HPP__  */
