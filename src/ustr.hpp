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

namespace socialmedia_signer {

/* ***************************************************************  */
/**
 * Fully Unicode+UTF-8 supported string.  For UTF-8 input in the
 * sources use the string literals:
 *
 *  1.    const char8_t*           u8"💗 😀 Hello World! 😀 💗"
 *  2. or std::u8string            u8"💗 😀 Hello World! 😀 💗"s
 *
 * The socialmedia_signer::ustr then internally is working with UTF-32
 * characters.  Therefore, the access to an character using
 * operator[]() has nearly no time to run.
 *
 * For output to GUI, console or file use this:
 *
 * ```c++
 *
 *   ustr str = u8"💗 😀 Hello World! 😀 💗"
 *
 *   // use it here ...
 *
 *   // output:
 *   std::u8string buf;
 *   msg.out_utf8(buf);   // here, BUF contains an UTF-8 encoded string
 *
 *   std::cout << reinterpret_cast<const char*>(buf.data());
 *
 * ```
 */
class ustr: public std::u32string
{
public:
  ustr();

  /* copy constructor  */
  ustr(const ustr& msg);

  ustr(const char8_t* msg);
  ustr(const std::u8string& msg);

  ustr(const char32_t* msg);
  ustr(const std::u32string& msg);
  ustr(const char32_t ch);

  /* copy assignment  */
  virtual ustr& operator=(const ustr& msg);

  virtual ustr& operator=(const char8_t* msg);
  virtual ustr& operator=(const std::u8string& msg);

  virtual ustr& operator=(const char32_t* msg);
  virtual ustr& operator=(const std::u32string& msg);
  virtual ustr& operator=(const char32_t ch);

  virtual void out_utf8(std::u8string& out) const;

  virtual size_type find(const ustr& msg, size_type pos=0) const
    noexcept;
  virtual ustr substr(size_type pos=0, size_type count=npos)
    const;

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

/* -------------------------------------------------------------------
 * Cool concatenation of unicode socialmedia_signer::ustr strings.
 */

  ustr operator+(const ustr& lhs, const char8_t* rhs);
  ustr operator+(const ustr& lhs, const std::u8string& rhs);
  ustr operator+(const ustr& lhs, const char32_t rhs);

  ustr operator+(const char8_t* lhs, const ustr& rhs);
  ustr operator+(const std::u8string& lhs, const ustr& rhs);
  ustr operator+(const char32_t lhs, const ustr& rhs);

} /* namespace socialmedia_signer  */

/* -------------------------------------------------------------------
 * std::formatter()´s for socialmedia_signer::format().
 */

template<>
struct std::formatter<socialmedia_signer::ustr, char>
  :public std::formatter<const char*, char>
{
  /* Just call it from the base class.
   */
  /*
  constexpr auto parse(std::format_parse_context& ctx)
  {
    return std::formatter<const char*, char>::parse(ctx);
  }
  */

  auto format(const socialmedia_signer::ustr& str,
              std::format_context& ctx) const
  {
    std::u8string out;
    str.out_utf8(out);

    return std::formatter<const char*, char>
      ::format(reinterpret_cast<const char*>(out.data()), ctx);
  }
};

template<size_t size>
struct std::formatter<char8_t[size], char>
  :public std::formatter<const char*, char>
{
  auto format(const char8_t* str, std::format_context& ctx) const
  {
    return std::formatter<const char*, char>
      ::format(reinterpret_cast<const char*>(str), ctx);
  }
};

template<>
struct std::formatter<char32_t, char>
  :public std::formatter<const char*, char>
{
  auto format(const char32_t str, std::format_context& ctx) const
  {
    socialmedia_signer::ustr in = str;

    std::u8string out;
    in.out_utf8(out);

    return std::formatter<const char*, char>
      ::format(reinterpret_cast<const char*>(out.data()), ctx);
  }
};

/* ***************************************************************  */

#endif /* USTR_HPP__  */
