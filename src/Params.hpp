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


#ifndef PARAMS_HPP__
#define PARAMS_HPP__

#include "common.hpp"

/* ***************************************************************  */

namespace socialmedia_signer {

class Params
{
public:
  /* Singleton class  */
  static void init(int argc, const char** argv);
  static void release();

  static Params* get();

private:
  explicit Params(int argc, const char** argv);
  virtual ~Params();

  static Params* instance;
};

};

/* ***************************************************************  */

#endif /* PARAMS_HPP__  */
