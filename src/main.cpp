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

#include "Common.hpp"

/* ***************************************************************  */

using namespace socialmedia_signer;

int
main(int argc, const char** argv)
{
  MTRACE(); {

  Params::init(argc, argv);

  /* -------------------------------------------------------------  */

  Params* params = Params::get();

  ustr cmd_name;
  params->get_command_name(cmd_name);

  if (Common::get_exit_code() < 0) {
#ifdef CONFIG_GUI
    Log::warn(u8"Not implemented -- GUI should run now.");
#else
    params->print_version();
    Log::println(ustr::format("\n  Usage: {} --help\n", cmd_name));
#endif
  }

  /* -------------------------------------------------------------  */

  Params::release();

  } MUNTRACE();

  int exit_code = Common::get_exit_code();
  return exit_code < 0? EXIT_SUCCESS: exit_code;
}

/* ***************************************************************  */
