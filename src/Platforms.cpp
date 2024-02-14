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


#include "Platforms.hpp"

#include "PlatformThreads.hpp"
#include "PlatformXCom.hpp"

/* ***************************************************************  */

socialmedia_signer::Platforms::iterator::iterator(
  const std::map<const ustr, Platform&>::iterator& other)
  :std::map<const ustr, Platform&>::iterator(other)
{
}

socialmedia_signer::Platform&
socialmedia_signer::Platforms::iterator::operator*()
{
  std::pair<const ustr, Platform&>& super_pair
    = std::map<const ustr, Platform&>::iterator::operator*();

  return super_pair.second;
}

socialmedia_signer::Platform*
socialmedia_signer::Platforms::iterator::operator->()
{
  std::pair<const ustr, Platform&>& super_pair
    = std::map<const ustr, Platform&>::iterator::operator*();

  return &super_pair.second;
}

/* ***************************************************************  */

socialmedia_signer::Platforms*
socialmedia_signer::Platforms::instance = nullptr;

/* ***************************************************************  */

socialmedia_signer::Platforms::Platforms()
  :platforms({
      PlatformThreads(),
      PlatformXCom()
  }), platform_map()
{
  for (Platform& cur: this->platforms) {
    const auto& [_, success] = this->platform_map.insert({
        cur.get_id(), cur});
    if (!success)
      Log::fatal(u8"Could not build Platforms::platform_map!");
  }
}

socialmedia_signer::Platforms::~Platforms()
{
}

/* ***************************************************************  */

void
socialmedia_signer::Platforms::init()
{
  if (Platforms::instance != nullptr)
    Log::fatal(u8"Platforms::init(): double call!");

  Platforms::instance = new Platforms();
}

void
socialmedia_signer::Platforms::release()
{
  delete Platforms::instance;
}

socialmedia_signer::Platforms*
socialmedia_signer::Platforms::get()
{
  if (Platforms::instance == nullptr)
    Log::fatal(u8"Platforms::init() not called!");

  return Platforms::instance;
}

/* ***************************************************************  */

socialmedia_signer::Platform*
socialmedia_signer::Platforms::get_by_id(const ustr& id) const
{
  const auto& platform_search = this->platform_map.find(id);
  if (platform_search == this->platform_map.end())
    return nullptr;

  return &platform_search->second;
}

/* ---------------------------------------------------------------  */

socialmedia_signer::Platforms::iterator
socialmedia_signer::Platforms::begin() noexcept
{
  return Platforms::iterator(this->platform_map.begin());
}

socialmedia_signer::Platforms::iterator
socialmedia_signer::Platforms::end() noexcept
{
  return Platforms::iterator(this->platform_map.end());
}

/* ***************************************************************  */
