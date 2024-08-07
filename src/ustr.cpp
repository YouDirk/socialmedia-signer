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
#include <cuchar>
#include <cctype>

/* ***************************************************************  */

namespace socialmedia_signer {

  static const std::locale
  ustr_locale_utf8 = std::locale("de_DE.UTF-8");

  static const std::codecvt<char32_t, char8_t, std::mbstate_t>&
  ustr_cvt_utf8 = std::use_facet<std::codecvt<char32_t,
                  char8_t, std::mbstate_t>>(ustr_locale_utf8);

} /* namespace socialmedia_signer  */

/* ***************************************************************  */

void
socialmedia_signer::ustr::_cvt_in_utf8(const std::u8string& in)
{
  std::mbstate_t mb {};

  const char8_t* in_next;
  char32_t* out_next;

  std::u32string::resize(in.length(), U'\0');

  ustr_cvt_utf8.in(mb, &in[0], &in[in.length()], in_next,
                   &(*this)[0], &(*this)[this->length()], out_next);

  std::u32string::resize(out_next - &(*this)[0]);
}

void
socialmedia_signer::ustr::_cvt_out_utf8(std::u8string& out) const
{
  std::mbstate_t mb {};

  const char32_t* in_next;
  char8_t* out_next;

  out.resize(this->length() * ustr_cvt_utf8.max_length(), u8'\0');

  ustr_cvt_utf8.out(mb, &(*this)[0], &(*this)[this->length()], in_next,
                    &out[0], &out[out.length()], out_next);

  out.resize(out_next - &out[0]);
}

/* ***************************************************************  */

socialmedia_signer::ustr::ustr(): std::u32string() {}

socialmedia_signer::ustr::ustr(const ustr& msg): std::u32string(msg) {}

socialmedia_signer::ustr::ustr(const char8_t* msg): std::u32string()
{
  this->_cvt_in_utf8(msg);
}

socialmedia_signer::ustr::ustr(const std::u8string& msg): std::u32string()
{
  this->_cvt_in_utf8(msg);
}

socialmedia_signer::ustr::ustr(const char32_t* msg): std::u32string(msg) {}

socialmedia_signer::ustr::ustr(const std::u32string& msg): std::u32string(msg) {}

socialmedia_signer::ustr::ustr(const char32_t ch): std::u32string(&ch, 1) {}

/* ---------------------------------------------------------------  */

socialmedia_signer::ustr&
socialmedia_signer::ustr::operator=(const ustr& msg)
{
  std::u32string::assign(msg);
  return *this;
}

socialmedia_signer::ustr&
socialmedia_signer::ustr::operator=(const char8_t* msg)
{
  this->_cvt_in_utf8(msg);
  return *this;
}

socialmedia_signer::ustr&
socialmedia_signer::ustr::operator=(const std::u8string& msg)
{
  this->_cvt_in_utf8(msg);
  return *this;
}

socialmedia_signer::ustr&
socialmedia_signer::ustr::operator=(const char32_t* msg)
{
  std::u32string::assign(msg);
  return *this;
}

socialmedia_signer::ustr&
socialmedia_signer::ustr::operator=(const std::u32string& msg)
{
  std::u32string::assign(msg);
  return *this;
}

socialmedia_signer::ustr&
socialmedia_signer::ustr::operator=(const char32_t ch)
{
  std::u32string::assign(1, ch);
  return *this;
}

/* ---------------------------------------------------------------  */

void
socialmedia_signer::ustr::out_utf8(std::u8string& out) const
{
  this->_cvt_out_utf8(out);
}

socialmedia_signer::ustr::size_type
socialmedia_signer::ustr::find(const ustr& msg, size_type pos) const
{
  return std::u32string::find(msg, pos);
}

socialmedia_signer::ustr
socialmedia_signer::ustr::substr(size_type pos, size_type count) const
{
  return std::u32string::substr(pos, count);
}

int
socialmedia_signer::ustr::compare(const ustr& str) const
{
  return std::u32string::compare(str);
}

int
socialmedia_signer::ustr::compare(
  size_type pos1, size_type count1, const ustr& str) const
{
  return std::u32string::compare(pos1, count1, str);
}

int
socialmedia_signer::ustr::compare(size_type pos1, size_type count1, const ustr& str,
                       size_type pos2, size_type count2) const
{
  return std::u32string::compare(pos1, count1, str, pos2, count2);
}

void
socialmedia_signer::ustr::tolower()
{
  for (char32_t* cur=&this->front(); cur <= &this->back(); cur++)
    *cur = std::tolower(*cur);
}

void
socialmedia_signer::ustr::toupper()
{
  for (char32_t* cur=&this->front(); cur <= &this->back(); cur++)
    *cur = std::toupper(*cur);
}

/* ***************************************************************  */

socialmedia_signer::ustr
socialmedia_signer::operator+(const ustr& lhs, const char8_t* rhs)
{
  return lhs + (ustr) rhs;
}

socialmedia_signer::ustr
socialmedia_signer::operator+(const ustr& lhs, const std::u8string& rhs)
{
  return lhs + (ustr) rhs;
}

socialmedia_signer::ustr
socialmedia_signer::operator+(const ustr& lhs, const char32_t rhs)
{
  return lhs + (ustr) rhs;
}

socialmedia_signer::ustr
socialmedia_signer::operator+(const char8_t* lhs, const ustr& rhs)
{
  return (ustr) lhs + rhs;
}

socialmedia_signer::ustr
socialmedia_signer::operator+(const std::u8string& lhs, const ustr& rhs)
{
  return (ustr) lhs + rhs;
}

socialmedia_signer::ustr
socialmedia_signer::operator+(const char32_t lhs, const ustr& rhs)
{
  return (ustr) lhs + rhs;
}

/* ***************************************************************  */
