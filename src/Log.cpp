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

#include "Params.hpp"

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

namespace socialmedia_signer {

static void _prefix(ustr& out, const ustr& level, const ustr& msg)
{
  const ustr& cmd_name = Params::get_command_name();

  out = ustr::format("{}:{}: {}\n", level, cmd_name, msg);
}

} /* namespace socialmedia_signer  */

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
socialmedia_signer::Log::note(const ustr& msg)
{
  ustr out;
  _prefix(out, u8"note", msg);

  std::u8string buf;
  out.out_utf8(buf);

  std::clog << reinterpret_cast<const char*>(buf.data());
}

void
socialmedia_signer::Log::warn(const ustr& msg)
{
  ustr out;
  _prefix(out, u8"Warning", msg);

  std::u8string buf;
  out.out_utf8(buf);

  std::clog << reinterpret_cast<const char*>(buf.data());
}

void
socialmedia_signer::Log::error(const ustr& msg)
{
  ustr out;
  _prefix(out, u8"ERROR", msg);

  std::u8string buf;
  out.out_utf8(buf);

  std::clog << reinterpret_cast<const char*>(buf.data());
}

void
socialmedia_signer::Log::fatal(const ustr& msg, int exit_code)
{
  ustr out;
  _prefix(out, u8"FATAL", msg);

  std::u8string buf;
  out.out_utf8(buf);

  std::clog << reinterpret_cast<const char*>(buf.data());

  std::exit(exit_code);
}

/* ***************************************************************  */
