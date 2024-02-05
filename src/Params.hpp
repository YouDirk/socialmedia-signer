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


#ifndef PARAMS_HPP__
#define PARAMS_HPP__

#include "Common.hpp"

#include <map>
#include <forward_list>

/* ***************************************************************  */

namespace socialmedia_signer {

class Params
{
public:
  /* Singleton class  */
  static void init(int argc, const char** argv);
  static void release();

  /* Get instance of singleton  */
  static Params* get();

  virtual void print_version() const;
  virtual void print_help() const;

  /* Needs to be static to make it possible that constructor Params()
   * can print output via class Log.
   */
  static void get_command_name(ustr& command_name);

protected:

  struct _Subargument {
    _Subargument(const ustr name, const char32_t abbr,
      const ustr description,const ustr value_doc,
      const bool value_allowed, const bool value_emptyallowed);

    const ustr name; const char32_t abbr;
    const ustr description;

    const ustr value_doc;
    const bool value_allowed;
    const bool value_emptyallowed;

    bool set;
    ustr set_value;
  };

  struct _Subcommand: public _Subargument {
    _Subcommand(const ustr name, const char32_t abbr,
      const ustr description, const ustr value_doc,
      const bool value_allowed, const bool value_emptyallowed,
      const std::forward_list<ustr> subargs_required,
      const std::forward_list<ustr> subargs_optional);

    /* Required arguments for this subcommand.  */
    const std::forward_list<ustr> subargs_required;
    const std::forward_list<ustr> subargs_optional;
  };

  /* -------------------------------------------------------------  */

  virtual bool check_parameters(std::map<ustr, ustr>& parsed_names,
    std::map<char32_t, ustr>& parsed_abbrs);
  virtual bool check_subcommands(std::map<ustr, ustr>& parsed_names,
    std::map<char32_t, ustr>& parsed_abbrs);
  virtual bool check_subaruments(
    std::forward_list<_Subargument>& subargs,
    std::map<ustr, _Subargument*>& subarg_map,
    std::map<ustr, ustr>& parsed_names,
    std::map<char32_t, ustr>& parsed_abbrs) const;

  /* -------------------------------------------------------------  */

  /* syntax to parse:
   *
   *   <argv>   -> '-' <name> | <abbr>
   *             | {terminate}
   *
   *   <name>   -> '-' [?a-zA-Z0-9]{2,} <value>
   *   <value>  -> '=' .* {terminate}
   *             | {terminate}
   *
   *   <abbr>   -> [?a-zA-Z0-9] <abbr+1>
   *
   *   <abbr+1> -> [?a-zA-Z0-9] <abbr+1>
   *             | {terminate} <argv+1>
   *
   *   <argv+1> -> [^-].* {terminate} {clear}
   *             | [-].* {terminate}
   *             | {terminate}
   *
   * description:
   *
   *   <xyz>        - expression xyz
   *   'a'          - char literal 'a'
   *   .[regex]+*   - word literal matching regular expression .[regex]+*
   *   {terminate}  - need to be parse End Of String for <argv>
   *                  means RETURN TRUE in implementation
   *   {clear}      - clear string which was parsed by rule to prevent
   *                  parsing <argv>/<argv+1> twice
   */

  virtual void parse_argv0(ustr& out, const ustr& argv0) const;
  virtual bool parse_argv(std::map<ustr, ustr>& parsed_names,
    std::map<char32_t, ustr>& parsed_abbrs, ustr& argv_next,
    const ustr& argv) const;

  virtual bool parse_name(std::map<ustr, ustr>& parsed_names,
    const ustr& argv) const;
  virtual bool parse_value(std::map<ustr, ustr>& parsed_names,
    const ustr& param_name, const ustr& argv) const;

  virtual bool parse_abbr(std::map<char32_t, ustr>& parsed_abbrs,
    ustr& argv_next, const ustr& argv) const;
  virtual bool parse_abbr_next(std::map<char32_t, ustr>& parsed_abbrs,
    ustr& argv_next, const ustr& argv) const;
  virtual bool parse_argv_next(std::map<char32_t, ustr>& parsed_abbrs,
    ustr& argv_next, const char32_t abbr_name) const;

  virtual bool print_error(const ustr& msg) const;

  /* -------------------------------------------------------------  */

private:
  explicit Params(int argc, const char** argv);
  virtual ~Params();

  static Params* instance;

  /* Needs to be static to make it possible that constructor Params()
   * can print output via class Log.
   */
  static ustr command_name;

  /* Memory location of subcommands and -arguments.  */
  std::forward_list<_Subargument> subargs;
  std::forward_list<_Subcommand>  subcmds;

  /* Used to lookup the lists above.  */
  std::map<ustr, _Subargument*> subarg_map;
  std::map<ustr, _Subcommand*>  subcmd_map;

  /* -------------------------------------------------------------  */
};

}

/* ***************************************************************  */

#endif /* PARAMS_HPP__  */
