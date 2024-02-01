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
socialmedia_signer::Params::param_rules = {
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
  bool success;

  if (argc < 1)
    Log::fatal(u8"Operating system does not provide argv[0]!");

  success = this->parse_argv0(this->command_name,
                           reinterpret_cast<const char8_t*>(argv[0]));
  if (!success)
    Log::fatal(u8"Could not parse argv[0]!");

  /* --------------------  */

  for (int i=1; i<argc; i++) {
    Log::debug(ustr::format("TODO: parse '{}'", argv[i]));
  }

  /* --------------------  */
}

socialmedia_signer::Params::~Params()
{
}

void
socialmedia_signer::Params::init(int argc, const char** argv)
{
  if (Params::instance != nullptr)
    Log::fatal(u8"Command line parameters double parsed!");

  Params::instance = new Params(argc, argv);

  // TODO: Apply --help or --version if they was applied by user via
  //       command line and exit with success.  Otherwise regular
  //       control flow.
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
  Log::print(COMMON_APP_NAME);

#ifdef CONFIG_GUI
  Log::print(u8" (GUI)");
#else
  Log::print(u8" (command-line)");
#endif

#ifdef CONFIG_VERSION
  Log::print(u8" version " CONFIG_VERSION);
#else
#  error "Missing -DCONFIG_VERSION flag in C++ compiler call!"
#endif

#ifdef DEBUG
  Log::print(u8" +debug");
#endif

  Log::println(u8"\n\nReport bugs to " COMMON_BUGTRACKING_URL);
}

void
socialmedia_signer::Params::print_help()
{
  Log::println(u8"Socialmedia Signer\n");
  Log::println(
          u8"Sign your social media posts and verify other posts.\n");

  for(_ParamEntry entry: Params::param_rules) {
    Log::println(
      ustr::format("  -{}, --{: <20} {}",
                   entry.abbr,
                   entry.name, entry.description));

    // TODO: Iterate through entry.values

#ifdef DEBUG
    Log::println(
      ustr::format(" {: >27} {} {}",
        u8"debug:", entry.set? u8"[X]": u8"[ ]",
        entry.set && !entry.set_value.empty()?
          (u8'=' + entry.set_value): u8'\0'));
#endif
  }

  Log::println();
  this->print_version();
}

/* ***************************************************************  */

void
socialmedia_signer::Params::get_command_name(ustr& command_name)
{
  command_name = this->command_name;
}

/* ***************************************************************  */

bool
socialmedia_signer::Params::parse_argv0(ustr& out, const ustr& argv0)
  const
{
  if (argv0.empty()) {
    Log::error(u8"argv[0] is empty!");
    return false;
  }

  int i_start = argv0.length();
  for(std::reverse_iterator it = argv0.rbegin();
      it != argv0.rend() || i_start == 0; it++, i_start--) {
    if (*it == u8'/' || *it == u8'\\') break;
  }

  out = argv0.substr(i_start, argv0.length());

  return true;
}

/* ***************************************************************  */
