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

#include "Platforms.hpp"
#include "Params.hpp"

/* ***************************************************************  */

socialmedia_signer::App::App()
  :signed_data(nullptr)
{
}

socialmedia_signer::App::~App()
{
  delete this->signed_data;
}

/* ---------------------------------------------------------------  */

void
socialmedia_signer::App::run() noexcept(false)
{
  const Params* params = Params::get();
  const Params::Subcommand* scmd = params->get_subcommand();

  Platforms* platforms = Platforms::get();

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

    /* Regularly control-flow:  run GUI if compiled.  */
    return;
  }

  /* Subcommand via command-line.  Apply it and terminate with
   * Success().
   */
  const Params::Subargument& sarg_message = params->get_subargument(U'm');
  const Params::Subargument& sarg_image = params->get_subargument(U'i');
  Platform* platform = nullptr;
  const Image* image = nullptr;
  switch (scmd->abbr) {
  case U's':
    platform = platforms->get_by_id(scmd->set_value);

    if (platform == nullptr) {
      throw Params::CmdErr(ustr::format(
        "--sign social media platform '{}' not supported!",
        scmd->set_value, Params::get_command_name()));
    }

    image = sarg_image.set
      ? new Image(sarg_image.set_value): new Image();

    this->sign(*platform, sarg_message.set_value, image);

    break;
  case U'v':
    this->verify(scmd->set_value);
    break;
  default: break;
  }

  /* Throw to make sure that AppGui::run() will not execute GUI.
   * Regularly control-flow above `if (scmd == nullptr)`.
   */
  throw Success();
}

/* ---------------------------------------------------------------  */

void
socialmedia_signer::App::sign(const Platform& platform,
  const ustr& message, const Image* image) noexcept(false)
{
  std::u8string message_utf8;
  message.out_utf8(message_utf8);

  this->set_signed_data(new SignedData(message_utf8, image));
  this->signed_data->sign();

  // TODO: post this->signed_data to Platform& platform;

  Log::debug(ustr::format("SIGN: {}, message='{}', image={}",
             platform.get_name(), message, image->to_string()));
}

void
socialmedia_signer::App::verify(const ustr& url) noexcept(false)
{
  Log::debug(ustr::format("VERIFY: post_url={}", url));

  // TODO: find Platform* by <url>
  // TODO: download post from platform and set this->signed_data

  if (this->signed_data == nullptr)
    throw Error(u8"App::verify(): Not implemented!");

  this->signed_data->verify();
}

/* ***************************************************************  */

void
socialmedia_signer::App::set_signed_data(SignedData* new_signed_data)
{
  delete this->signed_data;
  this->signed_data = new_signed_data;
}

/* ***************************************************************  */
