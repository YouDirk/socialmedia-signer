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


#include "App.hpp"

#include "Params.hpp"

/* ***************************************************************  */

socialmedia_signer::App::App()
  :signed_data(nullptr)
{
}

socialmedia_signer::App::~App()
{
}

/* ---------------------------------------------------------------  */

void
socialmedia_signer::App::run() const noexcept(false)
{
  const Params* params = Params::get();
  const Params::Subcommand* scmd = params->get_subcommand();

  /* Subcommand via command-line?
   *
   * If no subcommand then regularly control-flow.
   */
  if (scmd == nullptr) {
#ifndef CONFIG_GUI
    const ustr& cmd_name = params->get_command_name();

    params->print_version();
    Log::println(ustr::format("\n  Usage: {} --help\n", cmd_name));
#endif

    return;
  }

  /* Subcommand via command-line.  Apply it and terminate with
   * Success().
   */
  switch (scmd->abbr) {
  case U's':        /* --sign  */
    Log::debug(u8"--sign");
    break;
  case U'v':        /* --sign  */
    Log::debug(u8"--verify");
    break;
  default: break;
  }

  throw Success();
}

/* ---------------------------------------------------------------  */

const socialmedia_signer::SignedData&
socialmedia_signer::App::sign(const Platform& platform,
  const ustr& message, const Image* image) const noexcept(false)
{
  if (this->signed_data == nullptr) throw Error(u8"Not implemented!");

  return *this->signed_data;
}

const socialmedia_signer::SignedData&
socialmedia_signer::App::verify(const ustr& url) const noexcept(false)
{
  if (this->signed_data == nullptr) throw Error(u8"Not implemented!");

  return *this->signed_data;
}

/* ***************************************************************  */
