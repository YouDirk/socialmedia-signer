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

#ifdef CONFIG_GUI
#  include "AppGui.hpp"
#else
#  include "App.hpp"
#endif

#include "common.hpp"

/* ***************************************************************  */

using namespace socialmedia_signer;

/**
 * Entry point for operating system using ISO C standard.
 */
int
main(int argc, const char** argv)
{
  int exit_code = EXIT_SUCCESS;
  App* app;

  MTRACE();
  try {

    Params::init(argc, argv);

    /* -----------------------------------------------------------  */

#ifdef CONFIG_GUI
    app = new AppGui();
#else
    app = new App();
#endif

    app->run();

    delete app;

  } catch (Success& s) {
    exit_code = EXIT_SUCCESS;
  } catch (Error& e) {
    Log::error(e.uwhat());
    exit_code = e.get_exit_code();
  } catch (std::runtime_error& e) {
    /* Last chance to catch possible runtime-errors.  */
    Log::error(reinterpret_cast<const char8_t*>(e.what()));
    exit_code = EXIT_FAILURE;
  }

  /* -----------------------------------------------------------**  */

  Params::release();

  MUNTRACE();

  return exit_code;
}

/* ***************************************************************  */
