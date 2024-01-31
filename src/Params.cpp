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


#include "Params.hpp"

/* ***************************************************************  */

socialmedia_signer::Params::_ParamEntry::_ParamEntry(
  ustr name, char32_t abbr, std::vector<ustr> values,
  ustr description)
  :name(name), abbr(abbr), values(values), description(description),
   set(false), set_value(u8"")
{}

std::vector<socialmedia_signer::Params::_ParamEntry>
socialmedia_signer::Params::params = {
  _ParamEntry(u8"help", U'h',
              {},
              u8"display this help and exit"),
  _ParamEntry(u8"version", U'v',
              {},
              u8"output version information and exit")
};

socialmedia_signer::Params* socialmedia_signer::Params::instance \
  = nullptr;

/* ***************************************************************  */

socialmedia_signer::Params::Params(int argc, const char** argv)
{
  const char8_t** u8argv = (const char8_t**) argv;

  // TODO
}

socialmedia_signer::Params::~Params()
{
  // TODO
}

void
socialmedia_signer::Params::init(int argc, const char** argv)
{
  if (Params::instance != nullptr)
    Log::fatal(u8"Command line parameters double parsed!");

  Params::instance = new Params(argc, argv);
}

void
socialmedia_signer::Params::release()
{
  delete Params::instance;
}

socialmedia_signer::Params*
socialmedia_signer::Params::get()
{
  if (Params::instance == nullptr)
    Log::fatal(u8"Command line parameters not parsed!");

  return Params::instance;
}

void
socialmedia_signer::Params::print_version()
{
#ifdef CONFIG_VERSION
  Log::print(u8"Socialmedia Signer version " CONFIG_VERSION);
#else
#  error "Missing -DCONFIG_VERSION flag in C++ compiler call!"
#endif
#ifdef DEBUG
  Log::print(u8" +debug");
#endif
  Log::println(u8'\n');

  Log::println(u8"Report bugs to " CONFIG_BUGTRACKING_URL);
}

void
socialmedia_signer::Params::print_help()
{
  Log::println(u8"Socialmedia Signer\n");
  Log::println(
          u8"Sign your social media posts and verify other posts.\n");

  for(_ParamEntry entry: Params::params) {
    Log::println(
      ustr::format("  -{}, --{: <20} {}",
                   entry.abbr,
                   entry.name, entry.description));

#ifdef DEBUG
    Log::println(
      ustr::format(" {: >27} {} {}",
        u8"debug:", entry.set? u8"[X]": u8"[ ]",
        entry.set && !entry.set_value.empty()?
          (u8"=" + entry.set_value): u8'\0'));
#endif
  }

  Log::println();
  this->print_version();
}

/* ***************************************************************  */
