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

#ifdef DEBUG
void socialmedia_signer::Log::debug(const u8str& msg)
{
  std::clog << "DEBUG: " << (const char*) msg.data() << "\n";
}
#endif

void socialmedia_signer::Log::info(const u8str& msg)
{
  std::clog << "info: " << (const char*) msg.data() << "\n";
}

void socialmedia_signer::Log::warn(const u8str& msg)
{
  std::clog << "Warning: " << (const char*) msg.data() << "\n";
}

void socialmedia_signer::Log::error(const u8str& msg)
{
  std::clog << "ERROR: " << (const char*) msg.data() << "\n";
}

void socialmedia_signer::Log::fatal(const u8str& msg, int exit_code)
{
  std::clog << "FATAL: " << (const char*) msg.data() << "\n";

  exit(exit_code);
}

/* ***************************************************************  */
