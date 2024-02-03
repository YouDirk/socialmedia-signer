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


#ifndef LOG_HPP__
#define LOG_HPP__

#include "Common.hpp"

/* ***************************************************************  */

namespace socialmedia_signer {

class Log
{
public:
  /* Abstract class  */
  Log()           = delete;
  Log(Log& other) = delete;
  virtual ~Log()  = 0;

  /* -------------------------------------------------------------  */

  static void print(const ustr& msg);

  static void println(const ustr& msg);
  static void println();

  /* -------------------------------------------------------------  */

#ifdef DEBUG
  static void debug(const ustr& msg);
#else
  static void debug([[maybe_unused]] const ustr& msg) {};
#endif

  static void note(const ustr& msg);
  static void warn(const ustr& msg);
  static void error(const ustr& msg);

  static void fatal(const ustr& msg, int exit_code = 0xff);
};

}

/* ***************************************************************  */

#endif /* LOG_HPP__  */
