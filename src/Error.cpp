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


#include "Error.hpp"

/* ***************************************************************  */

socialmedia_signer::Error::Error(const ustr& reason)
  :std::runtime_error(""), reason(reason)
{
  this->reason.out_utf8(const_cast<std::u8string&>(this->reason_buf));
}

const char*
socialmedia_signer::Error::what() const noexcept
{
  return reinterpret_cast<const char*>(this->reason_buf.data());
}

const socialmedia_signer::ustr&
socialmedia_signer::Error::uwhat() const noexcept
{
  return this->reason;
}

/* ***************************************************************  */
