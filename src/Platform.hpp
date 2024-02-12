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


#ifndef PLATFORM_HPP__
#define PLATFORM_HPP__

#include "common.hpp"

/* ***************************************************************  */

namespace socialmedia_signer {

/**
 * Base interface class which needs to be implemented for every
 * supported social media platform.
 */
class Platform
{
public:
  explicit Platform(const ustr& id, const ustr& name);
  virtual ~Platform();

  const ustr& get_id();
  const ustr& get_name();

private:
  /** Used as abbreviation for command-line parameters.  */
  const ustr id;

  /** Human readable name of the platform.  */
  const ustr name;
};

}

/* ***************************************************************  */

#endif /* PLATFORM_HPP__  */
