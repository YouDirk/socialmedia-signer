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


#ifndef USTR_HPP__
#define USTR_HPP__

#include <string>
#include <codecvt>

using namespace std::string_literals;

/* ***************************************************************  */

namespace socialmedia_signer {

class ustr: public std::u32string
{
public:
  ustr(const char8_t* msg);
  ustr(const std::u8string& msg);

  ustr(const char32_t* msg);
  ustr(const std::u32string& msg);
  ustr(const char32_t msg);

  void out_utf8(std::u8string& out) const;

private:
  const std::codecvt<char32_t, char8_t, std::mbstate_t>& cvt_utf8;

  void _cvt_in_utf8(const std::u8string& in);
  void _cvt_out_utf8(std::u8string& out) const;
};

};

/* ***************************************************************  */

#endif /* USTR_HPP__  */
