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


#include "Log.hpp"

#include <iostream>

/* ***************************************************************  */

void
socialmedia_signer::Log::print(const ustr& msg)
{
  std::u8string buf;
  msg.out_utf8(buf);

  std::cout << reinterpret_cast<const char*>(buf.data());
}

void
socialmedia_signer::Log::println(const ustr& msg)
{
  std::u8string buf;
  msg.out_utf8(buf);

  std::cout << reinterpret_cast<const char*>(buf.data()) << "\n";
}

void
socialmedia_signer::Log::println()
{
  std::cout << "\n";
}

/* ***************************************************************  */

#ifdef DEBUG
void
socialmedia_signer::Log::debug(const ustr& msg)
{
  std::u8string buf;
  msg.out_utf8(buf);

  std::clog << "DEBUG: "
            << reinterpret_cast<const char*>(buf.data()) << "\n";
}
#endif

void
socialmedia_signer::Log::info(const ustr& msg)
{
  std::u8string buf;
  msg.out_utf8(buf);

  std::clog << "info: "
            << reinterpret_cast<const char*>(buf.data()) << "\n";
}

void
socialmedia_signer::Log::warn(const ustr& msg)
{
  std::u8string buf;
  msg.out_utf8(buf);

  std::clog << "Warning: "
            << reinterpret_cast<const char*>(buf.data()) << "\n";
}

void
socialmedia_signer::Log::error(const ustr& msg)
{
  std::u8string buf;
  msg.out_utf8(buf);

  std::clog << "ERROR: "
            << reinterpret_cast<const char*>(buf.data()) << "\n";
}

void
socialmedia_signer::Log::fatal(const ustr& msg, int exit_code)
{
  std::u8string buf;
  msg.out_utf8(buf);

  std::clog << "FATAL: "
            << reinterpret_cast<const char*>(buf.data()) << "\n";

  std::exit(exit_code);
}

/* ***************************************************************  */
