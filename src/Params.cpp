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

#define IS_CHAR_ALPHANUM(ch)       (((ch) >= u8'a' && (ch) <= u8'z') \
                                 || ((ch) >= u8'A' && (ch) <= u8'Z') \
                                 || ((ch) >= u8'0' && (ch) <= u8'9'))

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
  :command_name()
{
  bool success;

  /* -----------------------------------------------------------------
   * Parse ARGV[0].
   */

  if (argc < 1)
    Log::fatal(u8"Operating system does not provide argv[0]!");

  success = this->parse_argv0(this->command_name,
                           reinterpret_cast<const char8_t*>(argv[0]));
  if (!success)
    Log::fatal(u8"Could not parse argv[0]!");

  /* -----------------------------------------------------------------
   * Parse command-line parameters into PARSED_NAME and PARSED_ABBR.
   */

  std::map<ustr, ustr> parsed_name, parsed_abbr;

  ustr next;
  for (int i=1; i<argc; i++) {
    next = i < argc - 1? reinterpret_cast<const char8_t*>(argv[i+1])
                       : u8"";

    success = this->parse_argv(parsed_name, parsed_abbr, next,
                           reinterpret_cast<const char8_t*>(argv[i]));
    if (!success)
      std::exit(EXIT_FAILURE);
  }

  // TODO ...
  for (auto [name, value]: parsed_name)
    Log::debug(ustr::format("--{}={}", name, value));
  for (auto [abbr, value]: parsed_abbr)
    Log::debug(ustr::format("-{} {}", abbr, value));

  /* -------------------------------------------------------------  */
}

socialmedia_signer::Params::~Params()
{
}

/* ***************************************************************  */

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
socialmedia_signer::Params::print_version() const
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
socialmedia_signer::Params::print_help() const
{
  Log::println(COMMON_APP_NAME u8"\n\n" COMMON_APP_DESC u8"\n");

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
socialmedia_signer::Params::get_command_name(ustr& command_name) const
{
  command_name = this->command_name;
}

/* ***************************************************************  */

bool
socialmedia_signer::Params::parse_argv0(ustr& out, const ustr& argv0)
  const
{
  if (argv0.empty()) {
    Log::error(u8"argv[0] is empty!  Bad operating system?");
    return false;
  }

  int i_start;
  for (i_start=argv0.length(); i_start > 0; i_start--) {
    char32_t cur = argv0[i_start-1];
    if (cur == u8'/' || cur == u8'\\') break;
  }
  out = argv0.substr(i_start);

  return true;
}

/* Syntax to parse:
 *
 * <argv>   -> '-' <name> | <abbr>
 *           | {terminate}
 *
 * <name>   -> '-' [a-zA-z0-9]+ <value>
 * <value>  -> '=' .* {terminate}
 *           | {terminate}
 *
 * <abbr>   -> [a-zA-z0-9] <abbr>
 *           | {terminate} <argv+1>
 *
 * <argv+1> -> [^-].* {terminate} {clear}
 *           | {terminate}
 *
 * Description:
 *
 *  <xyz>       - expession xyz
 *  'a'         - char literal 'a'
 *  .[regex]+*  - word literal matching regular expression .[regex]+*
 * {terminate}  - need to be parse End Of String for <argv>
 * {clear}      - clear string which was parsed by rule to prevent
 *                parsing <argv>/<argv+1> twice
 */

bool
socialmedia_signer::Params::parse_argv(std::map<ustr, ustr>& parsed_name,
  std::map<ustr, ustr>& parsed_abbr, ustr& argv_next, const ustr& argv)
  const
{
  if (argv.empty()) return true;

  if (argv[0] != u8'-' || argv.length() < 2) {
    return this->_print_parse_error(ustr::format(
                 "Not a valid parameter '{}'!", argv));
  }

  if (argv[1] == u8'-') {
    return this->parse_name(parsed_name, argv_next, argv.substr(1));
  }

  return this->parse_abbr(parsed_abbr, argv_next, argv.substr(1));
}

bool
socialmedia_signer::Params::parse_name(std::map<ustr, ustr>& parsed_name,
  ustr& argv_next, const ustr& argv) const
{
  if (argv.length() < 2) {
    return this->_print_parse_error(ustr::format(
                 "Empty parameter name '--' found!", argv));
  }

  if (argv.length() < 3) {
    return this->_print_parse_error(ustr::format(
      "Parameter name '-{}' is too short!  Do you mean '{}'"
      " instead?", argv, argv));
  }

  unsigned i_end;
  for (i_end=1; i_end < argv.length(); i_end++) {
    char32_t cur = argv[i_end];
    if (!IS_CHAR_ALPHANUM(cur)) break;
  }
  ustr param_name = argv.substr(1, i_end-1);

  const auto [_, success] = parsed_name.insert({param_name, u8""});
  if (!success) {
    return this->_print_parse_error(ustr::format(
      "Parameter name given twice '--{}'!", param_name));
  }

  return true;
}

bool
socialmedia_signer::Params::parse_abbr(std::map<ustr, ustr>& parsed_abbr,
  ustr& argv_next, const ustr& argv) const
{
  Log::debug(u8"******************* abbr");

  return true;
}

/* ***************************************************************  */

bool
socialmedia_signer::Params::_print_parse_error(const ustr& msg) const
{
  ustr cmd_name;
  this->get_command_name(cmd_name);

  ustr out = ustr::format("command-line: {}\n\n  Usage: {} --help\n",
                          msg, cmd_name);
  Log::error(out);

  return false;
}

/* ***************************************************************  */
