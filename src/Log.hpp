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

#include "common.hpp"

namespace socialmedia_signer {

/* ***************************************************************  */

class Log {
public:
  /* Abstract class  */
  Log()           = delete;
  Log(Log& other) = delete;
  virtual ~Log()  = 0;

#ifdef DEBUG
  static void debug(const u8str& msg);
#else
  static void debug(const u8str& msg) {};
#endif

  static void info(const u8str& msg);
  static void warn(const u8str& msg);
  static void error(const u8str& msg);

  static void fatal(const u8str& msg, int exit_code = 0xff);
};

/* ***************************************************************  */

};

#endif /* LOG_HPP__  */
