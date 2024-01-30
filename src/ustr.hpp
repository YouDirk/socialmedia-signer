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
#include <format>

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

  template<typename... Args> inline static ustr
    format(const std::format_string<Args...> fmt, Args&&... args)
  {
    ustr result = (char8_t*)
      std::vformat(fmt.get(), std::make_format_args(args...)).data();

    return result;
  }

private:
  const std::codecvt<char32_t, char8_t, std::mbstate_t>& cvt_utf8;

  void _cvt_in_utf8(const std::u8string& in);
  void _cvt_out_utf8(std::u8string& out) const;
};

/* ---------------------------------------------------------------  */

ustr operator+(const ustr& lhs, const char8_t* rhs);
ustr operator+(const ustr& lhs, const std::u8string& rhs);
ustr operator+(const char8_t* lhs, const ustr& rhs);
ustr operator+(const std::u8string& lhs, const ustr& rhs);

  // TODO: Formatter for USTR as argument in FORMAT.
  /*
template<> struct std::formatter<ustr, char> {
  template<class ParseContext> constexpr ParseContext::iterator
    parse(ParseContext& ctx);

  template<class FmtContext> FmtContext::iterator
    format(QuotableString s, FmtContext& ctx) const;
};
  */

};

/* ***************************************************************  */

#endif /* USTR_HPP__  */
