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

/* ***************************************************************  */

socialmedia_signer::App::App()
  :signed_data(nullptr)
{
}

socialmedia_signer::App::~App()
{
}

/* ---------------------------------------------------------------  */

const socialmedia_signer::SignedData&
socialmedia_signer::App::sign(const IPlatform& platform,
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
