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


#include "Common.hpp"

/* ***************************************************************  */

int
socialmedia_signer::Common::exit_code = -1;

/* ***************************************************************  */

void
socialmedia_signer::Common::set_exit_code(int exit_code)
{
  Common::exit_code = exit_code;
}

int
socialmedia_signer::Common::get_exit_code()
{
  return Common::exit_code;
}

/* ***************************************************************  */
