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


#include "common.hpp"

/* ***************************************************************  */

socialmedia_signer::Error::Error(const ustr& reason, int exit_code)
  :std::runtime_error(""), reason(), _reason_buf(), exit_code(exit_code)
{
  this->set_reason(reason);
}

socialmedia_signer::Error::~Error()
{}

/* ***************************************************************  */

socialmedia_signer::Error::Error(int exit_code)
  :std::runtime_error(""), reason(), _reason_buf(), exit_code(exit_code)
{}

void
socialmedia_signer::Error::set_reason(const ustr& reason)
{
  this->reason = reason;
  this->reason.out_utf8(this->_reason_buf);
}

/* ***************************************************************  */

const char*
socialmedia_signer::Error::what() const noexcept
{
  return reinterpret_cast<const char*>(this->_reason_buf.data());
}

const socialmedia_signer::ustr&
socialmedia_signer::Error::uwhat() const noexcept
{
  return this->reason;
}

int
socialmedia_signer::Error::get_exit_code() const noexcept
{
  return this->exit_code;
}

/* ***************************************************************  */
