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


#include "SignedData.hpp"

/* ***************************************************************  */

socialmedia_signer::SignedData::SignedData(
  const std::u8string& signed_msg, const Image* signature)
  :message(signed_msg), signature(signature)
{
}

socialmedia_signer::SignedData::~SignedData()
{
  delete this->signature;
}

/* ***************************************************************  */

void
socialmedia_signer::SignedData::sign() noexcept(false)
{
  // TODO: Call Crypto layer
}

void
socialmedia_signer::SignedData::verify() const noexcept(false)
{
  // TODO: Call Crypto layer
}

/* ***************************************************************  */
