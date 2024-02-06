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
#include "App.hpp"

#include "Common.hpp"

/* ***************************************************************  */

using namespace socialmedia_signer;

/**
 * Entry point for operating system using ISO C standard.
 */
int
main(int argc, const char** argv)
{
  MTRACE();
  try {

    Params::init(argc, argv);
    App* app = new App();

    /* -----------------------------------------------------------  */

    // TODO: Do something with `app` ...

    if (Common::get_exit_code() < 0) {

#ifdef CONFIG_GUI
      Log::warn(u8"Not implemented -- GUI should run now.");
#else
      const Params* params = Params::get();
      const ustr& cmd_name = params->get_command_name();

      params->print_version();
      Log::println(ustr::format("\n  Usage: {} --help\n", cmd_name));
#endif

    }

    /* -----------------------------------------------------------  */

    delete app;
    Params::release();

    /* Last chance to catch possible runtime-errors.  */
  } catch (std::runtime_error& e) {
    Log::error(reinterpret_cast<const char8_t*>(e.what()));
    Common::set_exit_code(EXIT_FAILURE);
  }
  MUNTRACE();

  int exit_code = Common::get_exit_code();
  return exit_code < 0? EXIT_SUCCESS: exit_code;
}

/* ***************************************************************  */
