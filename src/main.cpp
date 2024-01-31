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

#include "common.hpp"

/* ***************************************************************  */

using namespace socialmedia_signer;

int
main(int argc, const char** argv)
{
  MTRACE(); {

  Params::init(argc, argv);

  /* -------------------------------------------------------------  */

  // TODO ...
  Log::debug(u8"Hello 💗 ääää€ 💗 World!"s + u8"😀 Hello 😀");

  // TODO: Own u8str class which are including conversions
  const ustr test = u8"😀1😀3€56";
  Log::debug((char8_t*) std::to_string(test.length()).data());
  Log::debug(test[2] + (ustr) test[4]);

  Log::debug(ustr::format("Hello {} {:0>6} {:.^50}",
                          5, 5.7, test + u8"💗äöüß💗"));
  Log::debug(test.substr(2, 3));
  Log::debug(ustr::format("First € found at {}.", test.find(u8"€")));
  Log::debug(ustr::format("Second {} found at {}.",
                          test[0], test.find(test[0], 1)));

  Params::get();

  /* -------------------------------------------------------------  */

  Params::release();

  } MUNTRACE();
  return EXIT_SUCCESS;
}

/* ***************************************************************  */
