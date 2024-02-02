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

#define IS_CHAR_ALPHANUM(ch)      ( ((ch) >= u8'a' && (ch) <= u8'z') \
                                 || ((ch) >= u8'A' && (ch) <= u8'Z') \
                                 || ((ch) >= u8'0' && (ch) <= u8'9') )

/* ***************************************************************  */

/* Using '_' for debugging purpoeses, should never be printed out.
 */
socialmedia_signer::ustr socialmedia_signer::Params::command_name
  = u8"socialmedia-signer_";

socialmedia_signer::Params::_ParamEntry::_ParamEntry(
  ustr name, char32_t abbr, std::vector<ustr> values,
  ustr description)
  :name(name), abbr(abbr), values(values), description(description),
   set(false), set_value(u8"")
{}

std::vector<socialmedia_signer::Params::_ParamEntry>
socialmedia_signer::Params::param_rules = {
  _ParamEntry(u8"help", U'?',
              {},
              u8"display this help and exit"),
  _ParamEntry(u8"version", U'V',
              {},
              u8"output version information and exit")
};

socialmedia_signer::Params* socialmedia_signer::Params::instance \
  = nullptr;

/* ***************************************************************  */

socialmedia_signer::Params::Params(int argc, const char** argv)
{
  bool success;

  /* UARGV: modifiable storage for parsing  */
  std::vector<ustr> uargv;
  for (int i=0; i<argc; i++) uargv.push_back((const char8_t*) argv[i]);

  /* -----------------------------------------------------------------
   * Parse ARGV[0].
   */

  if (argc < 1)
    Log::fatal(u8"Operating system does not provide argv[0]!");

  success = this->parse_argv0(Params::command_name, uargv[0]);
  if (!success)
    Log::fatal(u8"Could not parse argv[0]!");

  /* -----------------------------------------------------------------
   * Parse command-line parameters into PARSED_NAME and PARSED_ABBR.
   */

  std::map<ustr, ustr>     parsed_name;
  std::map<char32_t, ustr> parsed_abbr;

  ustr* next;
  for (int i=1; i<argc; i++) {
    ustr emptystr = u8"";
    next = i < argc-1? &uargv[i+1]: &emptystr;

    success = this->parse_argv(parsed_name, parsed_abbr, *next, uargv[i]);
    if (!success)
      std::exit(EXIT_FAILURE);
  }
  /* UARGV modified here, during parsing!  */

  // TODO ...
  for (auto [name, value]: parsed_name)
    Log::debug(ustr::format("Name {} = '{}'",
                            name, value.empty()? u8"<empty>": value));
  for (auto [abbr, value]: parsed_abbr)
    Log::debug(ustr::format("Abbr {} = '{}'",
                            abbr, value.empty()? u8"<empty>": value));

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
socialmedia_signer::Params::get_command_name(ustr& command_name)
{
  command_name = Params::command_name;
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

  return true;  /* {terminate}  */
}

bool
socialmedia_signer::Params::parse_argv(std::map<ustr, ustr>& parsed_name,
  std::map<char32_t, ustr>& parsed_abbr, ustr& argv_next,
  const ustr& argv) const
{
  if (argv.empty()) return true;

  if (argv[0] != u8'-' || argv.length() < 2) {
    return this->print_parse_error(ustr::format(
                                      "Not a parameter '{}'!", argv));
  }

  if (argv[1] == u8'-') {
    return this->parse_name(parsed_name, argv.substr(1));
  }

  return this->parse_abbr(parsed_abbr, argv_next, argv.substr(1));
}

/* ---------------------------------------------------------------  */

bool
socialmedia_signer::Params::parse_name(std::map<ustr, ustr>& parsed_name,
  const ustr& argv) const
{
  const std::size_t argv_len = argv.length();

  if (argv_len < 1 || argv[0] != u8'-') {
    return this->print_parse_error(ustr::format(
                                 "Not a parameter name -{} !", argv));
  }

  unsigned name_len;
  for (name_len=0; 1+name_len < argv_len; name_len++) {
    char32_t cur = argv[1+name_len];
    if (!(IS_CHAR_ALPHANUM(cur) || cur == u8'?')) break;
  }
  ustr param_name = argv.substr(1, name_len);
  const std::size_t param_len = param_name.length();

  if (param_len < 1) {
    return this->print_parse_error(ustr::format(
                                      "Empty parameter name '--' !"));
  }

  if (param_name.length() < 2) {
    return this->print_parse_error(ustr::format(
      "Incomplete parameter name --{} !  Did you mean -{} ?",
      param_name, param_name));
  }

  return this->parse_value(parsed_name, param_name,
                           argv.substr(1+name_len));
}

bool
socialmedia_signer::Params::parse_value(std::map<ustr, ustr>& parsed_name,
  const ustr& param_name, const ustr& argv) const
{
  ustr value;

  if (argv.empty()) {
    value = u8"";
    /* {terminate}  */
  } else if (argv[0] != u8'=') {
    return this->print_parse_error(ustr::format(
      "Garbage '{}' behind --{} !", argv, param_name));
  } else {
    value = argv.substr(1);
    /* {terminate}  */
  }

  const auto [_, success] = parsed_name.insert({param_name, value});
  if (!success) {
    return this->print_parse_error(ustr::format(
      "Double parameter name --{} !", param_name));
  }

  return true;
}

/* ---------------------------------------------------------------  */

bool
socialmedia_signer::Params::parse_abbr(
  std::map<char32_t, ustr>& parsed_abbr, ustr& argv_next,
  const ustr& argv) const
{
  if (argv.empty()) {
    return this->print_parse_error(ustr::format(
      "Empty parameter abbrevation '-' !"));
  }

  return this->parse_abbr_next(parsed_abbr, argv_next, argv);
}

bool
socialmedia_signer::Params::parse_abbr_next(
  std::map<char32_t, ustr>& parsed_abbr, ustr& argv_next,
  const ustr& argv) const
{
  char32_t abbr_name = argv[0];
  if (!(IS_CHAR_ALPHANUM(abbr_name) || abbr_name == u8'?')) {
    return this->print_parse_error(ustr::format(
      "Not a parameter abbrevation -{} !", abbr_name));
  }

  /* Is last character ?  */
  if (argv.length() < 2)
    return this->parse_argv_next(parsed_abbr, argv_next, abbr_name);

  const auto [_, success] = parsed_abbr.insert({abbr_name, u8""});
  if (!success) {
    return this->print_parse_error(ustr::format(
      "Double parameter abbrevation -{} !", abbr_name));
  }

  return this->parse_abbr_next(parsed_abbr, argv_next, argv.substr(1));
}

bool
socialmedia_signer::Params::parse_argv_next(
  std::map<char32_t, ustr>& parsed_abbr, ustr& argv_next,
  const char32_t abbr_name) const
{
  ustr value;

  if (argv_next.empty() || argv_next[0] == u8'-') {
    value = u8"";  /* 2x {terminate}  */
  } else {
    value = argv_next; /* {terminate}  */
    argv_next = u8"";  /* {clear}  */
  }

  const auto [_, success] = parsed_abbr.insert({abbr_name, value});
  if (!success) {
    return this->print_parse_error(ustr::format(
      "Double parameter abbrevation -{} !", abbr_name));
  }

  return true;
}

/* ---------------------------------------------------------------  */

bool
socialmedia_signer::Params::print_parse_error(const ustr& msg) const
{
  ustr cmd_name;
  Params::get_command_name(cmd_name);

  ustr out = ustr::format(
    "command-line: {}  Try '{} --help' for full help.", msg, cmd_name);
  Log::error(out);

  return false;
}

/* ***************************************************************  */
