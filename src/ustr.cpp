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


#include "ustr.hpp"

#include <locale>

static std::locale ustr_locale_utf8 = std::locale("de_DE.UTF-8");

/* ***************************************************************  */

void
socialmedia_signer::ustr::_cvt_in_utf8(const std::u8string& in)
{
  std::mbstate_t mb {};

  const char8_t* in_next;
  char32_t* out_next;

  this->resize(in.length(), U'\0');

  this->cvt_utf8.in(mb, &in[0], &in[in.length()], in_next,
                    &(*this)[0], &(*this)[this->length()], out_next);

  this->resize(out_next - &(*this)[0]);
}

void
socialmedia_signer::ustr::_cvt_out_utf8(std::u8string& out) const
{
  std::mbstate_t mb {};

  const char32_t* in_next;
  char8_t* out_next;

  out.resize(this->length() * this->cvt_utf8.max_length(), U'\0');

  this->cvt_utf8.out(mb, &(*this)[0], &(*this)[this->length()], in_next,
                     &out[0], &out[out.length()], out_next);

  out.resize(out_next - &out[0]);
}

/* ***************************************************************  */

socialmedia_signer::ustr::ustr(const char8_t* msg)
  :std::u32string(),
   cvt_utf8(std::use_facet<std::codecvt<char32_t,
                           char8_t, std::mbstate_t>>(ustr_locale_utf8))
{
  this->_cvt_in_utf8(msg);
}

socialmedia_signer::ustr::ustr(const std::u8string& msg)
  :std::u32string(),
   cvt_utf8(std::use_facet<std::codecvt<char32_t,
                           char8_t, std::mbstate_t>>(ustr_locale_utf8))
{
  this->_cvt_in_utf8(msg);
}

socialmedia_signer::ustr::ustr(const char32_t* msg)
  :std::u32string(msg),
   cvt_utf8(std::use_facet<std::codecvt<char32_t,
                           char8_t, std::mbstate_t>>(ustr_locale_utf8))
{}

socialmedia_signer::ustr::ustr(const std::u32string& msg)
  :std::u32string(msg),
   cvt_utf8(std::use_facet<std::codecvt<char32_t,
                           char8_t, std::mbstate_t>>(ustr_locale_utf8))
{}

socialmedia_signer::ustr::ustr(const char32_t msg)
  :std::u32string(&msg, 1),
   cvt_utf8(std::use_facet<std::codecvt<char32_t,
                           char8_t, std::mbstate_t>>(ustr_locale_utf8))
{}

void
socialmedia_signer::ustr::out_utf8(std::u8string& out) const
{
  this->_cvt_out_utf8(out);
}

/* ***************************************************************  */