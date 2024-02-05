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

#include <vector>

#define IS_CHAR_ALPHANUM(ch)      ( ((ch) >= u8'a' && (ch) <= u8'z') \
                                 || ((ch) >= u8'A' && (ch) <= u8'Z') \
                                 || ((ch) >= u8'0' && (ch) <= u8'9') )

/* ***************************************************************  */

/* Using '_' for debugging purposes, should never be printed out.
 */
socialmedia_signer::ustr
socialmedia_signer::Params::command_name = u8"socialmedia-signer_";

socialmedia_signer::Params*
socialmedia_signer::Params::instance = nullptr;

/* ***************************************************************  */

socialmedia_signer::Params::Subargument::Subargument(
  const ustr name, const char32_t abbr, const ustr description,
  const ustr value_doc, const bool value_allowed,
  const bool value_emptyallowed)
  :name(name), abbr(abbr), description(description),
   value_doc(value_doc), value_allowed(value_allowed),
   value_emptyallowed(value_emptyallowed), set(false), set_value(u8"")
{}

socialmedia_signer::Params::Subcommand::Subcommand(
  const ustr name, const char32_t abbr, const ustr description,
  const ustr value_doc, const bool value_allowed,
  const bool value_emptyallowed,
  const char32_t* subargs_required, const char32_t* subargs_optional)
  :Subargument(name, abbr, description, value_doc, value_allowed,
                value_emptyallowed),
   subargs_required(subargs_required),
   subargs_optional(subargs_optional)
{}

/* ***************************************************************  */

socialmedia_signer::Params::Params(int argc, const char** argv)
  :subargs({
     Subargument(u8"text", u8't',
       u8"text message to sign or verify",
       u8"<message>", true, false),
     Subargument(u8"image", u8'i',
       u8"filename of an image to overlay the QR signature",
       u8"<filename>", true, false)
   }),
   subcmds({
     Subcommand(u8"sign", u8's',
       u8"post a signed message to <platform>",
       u8"<platform>", true, false,
       U"t",
       U"i"),
     Subcommand(u8"verify", u8'v',
       u8"verify a post with a QR signature at <url>",
       u8"<url>", true, false,
       U"",
       U""),

     Subcommand(u8"help", u8'?',
       u8"display this help and exit",
       u8"", false, true, U"", U""),
     Subcommand(u8"version", u8'V',
       u8"output version information and exit",
       u8"", false, true, U"", U"")
   }), subarg_map(), subcmd_map(), subcommand(nullptr)
{
  /* UARGV: modifiable storage for parsing  */
  std::vector<ustr> uargv;
  for (int i=0; i<argc; i++) uargv.push_back((const char8_t*) argv[i]);

  /* -----------------------------------------------------------------
   * Parse ARGV[0].
   */

  if (argc < 1)
    Log::fatal(u8"Operating system does not provide argv[0]!");

  this->parse_argv0(Params::command_name, uargv[0]);

  /* -----------------------------------------------------------------
   * Parse command-line parameters into PARSED_NAMES and PARSED_ABBRS.
   */

  std::map<ustr, ustr>     parsed_names;
  std::map<char32_t, ustr> parsed_abbrs;

  ustr* next;
  for (int i=1; i<argc; i++) {
    ustr emptystr = u8"";
    next = i < argc-1? &uargv[i+1]: &emptystr;

    if (!this->parse_argv(parsed_names, parsed_abbrs, *next, uargv[i]))
      std::exit(EXIT_FAILURE);
  }
  /* UARGV modified here, during parsing!  */

  /* -----------------------------------------------------------------
   * Check parsed parameters and build subcommands and subarguments.
   */

  if (!this->check_parameters(parsed_names, parsed_abbrs))
    std::exit(EXIT_FAILURE);

#ifdef DEBUG_PARAMS
  for (const auto& subarg_search: this->subarg_map) {
    Subargument* sarg = subarg_search.second;

    Log::debug(ustr::format("-{} --{: <7} = {: >7} '{}'", sarg->abbr,
      sarg->name, static_cast<ustr>(sarg->set? u8"SET": u8"not set"),
      sarg->set_value));
  }
  for (const auto& subcmd_search: this->subcmd_map) {
    Subcommand* scmd = subcmd_search.second;

    Log::debug(ustr::format("-{} --{: <7} = {: >7} '{}'", scmd->abbr,
      scmd->name, static_cast<ustr>(scmd->set? u8"SET": u8"not set"),
      scmd->set_value));
  }
  for (const auto& names: parsed_names) {
    Log::debug(ustr::format("*** names --{} = '{}'", names.first,
      names.second));
  }
  for (const auto& abbrs: parsed_abbrs) {
    Log::debug(ustr::format("*** abbrs -{} '{}'", abbrs.first,
      abbrs.second));
  }
#endif

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

  const Subcommand* subcmd = instance->get_subcommand();
  if (subcmd == nullptr) return;

  switch (subcmd->abbr) {
  case u8'V':
    instance->print_version();
    Common::set_exit_code(0);
    break;
  case u8'?':
    instance->print_help();
    Common::set_exit_code(0);
    break;
  default: break;
  }
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

const socialmedia_signer::ustr&
socialmedia_signer::Params::get_command_name()
{
  return Params::command_name;
}

/* ***************************************************************  */

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
  const ustr& command_name = Params::get_command_name();

  Log::println(COMMON_APP_NAME u8"\n\n" COMMON_APP_DESC u8"\n\nUsage:");

#ifdef CONFIG_GUI
  Log::println(ustr::format("  {} (for GUI)", command_name));
#endif

  for (const Subcommand& scmd: this->subcmds) {
    if (scmd.abbr == u8'?' || scmd.abbr == u8'V') continue;

    ustr out = ustr::format("  {} {}",
                            command_name, this->format(scmd, false));

    for (const char32_t* sarg_req_abbr = scmd.subargs_required;
         *sarg_req_abbr != U'\0'; sarg_req_abbr++) {
      const Subargument* sarg_req = this->get_subargument(*sarg_req_abbr);
      if (sarg_req == nullptr) {
        Log::fatal(ustr::format("help: subarg -{} not implemented"
          ", required for subcmd --{} !", *sarg_req_abbr, scmd.name));
      }
      out = ustr::format("{} {}",
                         out, this->format(*sarg_req, true));
    }
    for (const char32_t* sarg_opt_abbr = scmd.subargs_optional;
         *sarg_opt_abbr != U'\0'; sarg_opt_abbr++) {
      const Subargument* sarg_opt = this->get_subargument(*sarg_opt_abbr);
      if (sarg_opt == nullptr) {
        Log::fatal(ustr::format("help: subarg -{} not implemented"
          ", optional for subcmd --{} !", *sarg_opt_abbr, scmd.name));
      }
      out = ustr::format("{} {}",
                         out, this->format(*sarg_opt, true, true));
    }

    Log::println(out);
  }
  Log::println(u8"\nSubcommands:");

  for (const Subcommand& scmd: this->subcmds) {
    Log::println(
      ustr::format("  -{}, {: <19} {}",
        scmd.abbr, this->format(scmd, false), scmd.description));
  }
  Log::println(u8"\nSubarguments:");

  for (const Subargument& sarg: this->subargs) {
    Log::println(
      ustr::format("  -{}, {: <19} {}",
        sarg.abbr, this->format(sarg, false), sarg.description));
  }

  Log::println();
  this->print_version();
}

const socialmedia_signer::Params::Subcommand*
socialmedia_signer::Params::get_subcommand() const
{
  return this->subcommand;
}

const socialmedia_signer::Params::Subcommand*
socialmedia_signer::Params::get_subcommand(const char32_t scmd_abbr)
  const
{
  const auto& scmd_search = this->subcmd_map.find(scmd_abbr);

  if (scmd_search == subcmd_map.end()) return nullptr;

  return scmd_search->second;
}

const socialmedia_signer::Params::Subargument*
socialmedia_signer::Params::get_subargument(const char32_t scmd_abbr)
  const
{
  const auto& sarg_search = this->subarg_map.find(scmd_abbr);

  if (sarg_search == subarg_map.end()) return nullptr;

  return sarg_search->second;
}

/* ***************************************************************  */

const socialmedia_signer::ustr
socialmedia_signer::Params::format(const Subargument& subarg,
                                   bool abbr, bool optional) const
{
  const char32_t equals = abbr? u8' ': u8'=';

  ustr out = abbr
    ? ustr::format("-{}", subarg.abbr)
    : ustr::format("--{}", subarg.name);

  if (subarg.value_allowed && subarg.value_emptyallowed)
    out = ustr::format("{}[{}{}]", out, equals, subarg.value_doc);
  else if (!subarg.value_emptyallowed)
    out = ustr::format("{}{}{}", out, equals, subarg.value_doc);

  if (optional)
    out = ustr::format("[{}]", out);

  return out;
}

bool
socialmedia_signer::Params::check_parameters(
  std::map<ustr, ustr>& parsed_names,
  std::map<char32_t, ustr>& parsed_abbrs)
{
  if (!this->check_subaruments(&this->subargs, &this->subarg_map,
                               parsed_names, parsed_abbrs))
    return false;

  if (!this->check_subcommands(parsed_names, parsed_abbrs))
    return false;

  if (!parsed_names.empty()) {
    return this->print_error(ustr::format(
      "--{} unknown parameter name !", parsed_names.begin()->first));
  }
  if (!parsed_abbrs.empty()) {
    return this->print_error(ustr::format(
      "-{} unknown parameter !", parsed_abbrs.begin()->first));
  }

  return true;
}

bool
socialmedia_signer::Params::check_subcommands(
  std::map<ustr, ustr>& parsed_names,
  std::map<char32_t, ustr>& parsed_abbrs)
{
  if (!this->check_subaruments(
    reinterpret_cast<std::forward_list<Subargument>*>(&this->subcmds),
    reinterpret_cast<std::map<char32_t, Subargument*>*>(&this->subcmd_map),
    parsed_names, parsed_abbrs)) return false;

  for (const Subcommand& scmd: this->subcmds) {
    if (!scmd.set) continue;

    if (this->subcommand != nullptr) {
      return this->print_error(ustr::format(
        "subcommands --{} and --{} set, choose just one please !",
        this->subcommand->name, scmd.name));
    }

    for (const char32_t* sarg_req_abbr = scmd.subargs_required;
         *sarg_req_abbr != U'\0'; sarg_req_abbr++) {
      const auto& sarg_search = this->subarg_map.find(*sarg_req_abbr);
      if (sarg_search != subarg_map.end()
          && sarg_search->second->set) continue;

      return this->print_error(ustr::format(
        "subcommand --{} requires -{} to be set !",
        scmd.name, *sarg_req_abbr));
    }

    this->subcommand = &scmd;
  }

  for (const Subargument& sarg: this->subargs) {
    if (!sarg.set) continue;

    if (this->subcommand == nullptr) {
      return this->print_error(ustr::format(
        "subargument --{} without a subcommand !", sarg.name));
    }

    for (const char32_t* cur_req = this->subcommand->subargs_required;
         *cur_req != U'\0'; cur_req++)
      if (*cur_req == sarg.abbr) goto continue_outer;
    for (const char32_t* cur_opt = this->subcommand->subargs_optional;
         *cur_opt != U'\0'; cur_opt++)
      if (*cur_opt == sarg.abbr) goto continue_outer;

    return this->print_error(ustr::format(
      "--{} is not a subargument of subcommand --{} !",
      sarg.name, this->subcommand->name));

  continue_outer:;
  }

  return true;
}

bool
socialmedia_signer::Params::check_subaruments(
  std::forward_list<Subargument>* subargs,
  std::map<char32_t, Subargument*>* subarg_map,
  std::map<ustr, ustr>& parsed_names,
  std::map<char32_t, ustr>& parsed_abbrs) const
{
  for (Subargument& sarg: *subargs) {
    const auto& name_search = parsed_names.find(sarg.name);
    if (name_search != parsed_names.end()) {
      sarg.set = true;
      sarg.set_value = name_search->second;

      if (parsed_names.erase(sarg.name) == 0) {
        Log::fatal(ustr::format(
          "Could not erase --{} from parsed names !", sarg.name));
      }
    }

    const auto& abbr_search = parsed_abbrs.find(sarg.abbr);
    if (abbr_search != parsed_abbrs.end()) {
      if (sarg.set) {
        return this->print_error(ustr::format(
          "--{} and -{} given, but just one of them needed !", sarg.name,
          sarg.abbr, sarg.set_value));
      }

      sarg.set = true;
      sarg.set_value = abbr_search->second;

      if (parsed_abbrs.erase(sarg.abbr) == 0) {
        Log::fatal(ustr::format(
          "Could not erase -{} from parsed abbreviations !", sarg.abbr));
      }
    }

    if (sarg.set && !sarg.value_allowed && !sarg.set_value.empty()) {
      return this->print_error(ustr::format(
        "--{} value needs to be empty, but set to --{}={} !", sarg.name,
        sarg.name, sarg.set_value));
    }
    if (sarg.set && !sarg.value_emptyallowed && sarg.set_value.empty()) {
      return this->print_error(ustr::format(
        "--{} a value is required, example --{}={} !", sarg.name,
        sarg.name, sarg.value_doc));
    }

    const auto& [_, success] = subarg_map->insert({sarg.abbr, &sarg});
    if (!success) {
      Log::fatal(ustr::format(
        "Could not insert subargument --{}, -{}, '{}'!  Double insert?",
        sarg.name, sarg.abbr, sarg.description));
    }
  }

  return true;
}

/* ***************************************************************  */

void
socialmedia_signer::Params::parse_argv0(ustr& out, const ustr& argv0)
  const
{
  if (argv0.empty())
    Log::fatal(u8"argv[0] is empty!  Bad operating system?");

  int i_start;
  for (i_start=argv0.length(); i_start > 0; i_start--) {
    char32_t cur = argv0[i_start-1];
    if (cur == u8'/' || cur == u8'\\') break;
  }
  out = argv0.substr(i_start);

  /* {terminate}  */
}

bool
socialmedia_signer::Params::parse_argv(
  std::map<ustr, ustr>& parsed_names,
  std::map<char32_t, ustr>& parsed_abbrs, ustr& argv_next,
  const ustr& argv) const
{
  if (argv.empty()) return true;

  if (argv[0] != u8'-' || argv.length() < 2) {
    return this->print_error(ustr::format(
                                      "'{}' not a parameter !", argv));
  }

  if (argv[1] == u8'-') {
    return this->parse_name(parsed_names, argv.substr(1));
  }

  return this->parse_abbr(parsed_abbrs, argv_next, argv.substr(1));
}

/* ---------------------------------------------------------------  */

bool
socialmedia_signer::Params::parse_name(
  std::map<ustr, ustr>& parsed_names, const ustr& argv) const
{
  const std::size_t argv_len = argv.length();

  if (argv_len < 1 || argv[0] != u8'-') {
    return this->print_error(ustr::format(
                                "-{} not a parameter name  !", argv));
  }

  unsigned name_len;
  for (name_len=0; 1+name_len < argv_len; name_len++) {
    char32_t cur = argv[1+name_len];
    if (!(IS_CHAR_ALPHANUM(cur) || cur == u8'?')) break;
  }
  ustr param_name = argv.substr(1, name_len);
  const std::size_t param_len = param_name.length();

  if (param_len < 1) {
    return this->print_error(ustr::format(
                                 "-{} empty parameter name !", argv));
  }

  if (param_name.length() < 2) {
    return this->print_error(ustr::format(
      "--{} incomplete parameter name for -{} !  Did you mean -{} ?",
      param_name, argv, param_name));
  }

  return this->parse_value(parsed_names, param_name,
                           argv.substr(1+name_len));
}

bool
socialmedia_signer::Params::parse_value(
  std::map<ustr, ustr>& parsed_names, const ustr& param_name,
  const ustr& argv) const
{
  ustr value;

  if (argv.empty()) {
    value = u8"";
    /* {terminate}  */
  } else if (argv[0] != u8'=') {
    return this->print_error(ustr::format(
      "Garbage '{}' behind --{} !", argv, param_name));
  } else {
    value = argv.substr(1);
    /* {terminate}  */
  }

  const auto& [_, success] = parsed_names.insert({param_name, value});
  if (!success) {
    return this->print_error(ustr::format(
      "--{} double parameter name !", param_name));
  }

  return true;
}

/* ---------------------------------------------------------------  */

bool
socialmedia_signer::Params::parse_abbr(
  std::map<char32_t, ustr>& parsed_abbrs, ustr& argv_next,
  const ustr& argv) const
{
  if (argv.empty()) {
    return this->print_error(ustr::format(
      "'-' empty parameter abbreviation !"));
  }

  return this->parse_abbr_next(parsed_abbrs, argv_next, argv);
}

bool
socialmedia_signer::Params::parse_abbr_next(
  std::map<char32_t, ustr>& parsed_abbrs, ustr& argv_next,
  const ustr& argv) const
{
  char32_t abbr_name = argv[0];
  if (!(IS_CHAR_ALPHANUM(abbr_name) || abbr_name == u8'?')) {
    if (abbr_name == u8'=')
      return this->print_error(ustr::format(
        "-example{} not a parameter !  Did you mean"
        " --example{} or '-e {}' ?", argv, argv, argv.substr(1)));
    else
      return this->print_error(ustr::format(
        "-{} not a parameter abbreviation !", abbr_name));
  }

  /* Is last character ?  */
  if (argv.length() < 2)
    return this->parse_argv_next(parsed_abbrs, argv_next, abbr_name);

  const auto& [_, success] = parsed_abbrs.insert({abbr_name, u8""});
  if (!success) {
    return this->print_error(ustr::format(
      "-{} double parameter abbreviation !", abbr_name));
  }

  return this->parse_abbr_next(parsed_abbrs, argv_next, argv.substr(1));
}

bool
socialmedia_signer::Params::parse_argv_next(
  std::map<char32_t, ustr>& parsed_abbrs, ustr& argv_next,
  const char32_t abbr_name) const
{
  ustr value;

  if (argv_next.empty() || argv_next[0] == u8'-') {
    value = u8"";  /* 2x {terminate}  */
  } else {
    value = argv_next; /* {terminate}  */
    argv_next = u8"";  /* {clear}  */
  }

  const auto& [_, success] = parsed_abbrs.insert({abbr_name, value});
  if (!success) {
    return this->print_error(ustr::format(
      "-{} double parameter abbreviation !", abbr_name));
  }

  return true;
}

/* ---------------------------------------------------------------  */

bool
socialmedia_signer::Params::print_error(const ustr& msg) const
{
  const ustr& cmd_name = Params::get_command_name();

  ustr out = ustr::format(
    "command-line: {}  Try '{} --help' for full help.", msg, cmd_name);
  Log::error(out);

  return false;
}

/* ***************************************************************  */
