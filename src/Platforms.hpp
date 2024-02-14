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


#ifndef PLATFORMS_HPP__
#define PLATFORMS_HPP__

#include "common.hpp"

#include "Platform.hpp"

#include <map>
#include <forward_list>

/* ***************************************************************  */

namespace socialmedia_signer {

/**
 * Singleton class which can be accessed via Platforms::get().  It
 * provides process wide access to the list of supported ::Platform´s.
 */
class Platforms
{
public:

  class iterator: public std::map<const ustr, Platform&>::iterator {
  public:
    explicit iterator(
      const std::map<const ustr, Platform&>::iterator& other);

    virtual Platform& operator*();
    virtual Platform* operator->();
  };

  /* Singleton class  */
  static void init();
  static void release();

  /* Get instance of singleton  */
  static Platforms* get();

  /**
   * Returns `nullptr` if `id` does not exist.
   */
  virtual Platform* get_by_id(const ustr& id) const;

  /**
   * Iterate in alphabetic order of platform id.
   */
  virtual iterator begin() noexcept;
  virtual iterator end() noexcept;

private:
  explicit Platforms();
  virtual ~Platforms();

  static Platforms* instance;

  /** Memory location of ::Platform´s .  */
  std::forward_list<Platform> platforms;

  /** Used to lookup the list Platforms::platforms .  */
  std::map<const ustr, Platform&> platform_map;
};

}

/* ***************************************************************  */

#endif /* PLATFORMS_HPP__  */
