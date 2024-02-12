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


#ifndef APP_HPP__
#define APP_HPP__

#include "Platform.hpp"
#include "Image.hpp"
#include "SignedData.hpp"

#include "common.hpp"

/* ***************************************************************  */

namespace socialmedia_signer {

/**
 * Class which implements the core functionality of the Socialmedia
 * Signer.
 *
 * This class will be instantiated directly for the command-line
 * version and is the base class of AppGui.
 */
class App
{
public:
  explicit App();
  virtual ~App();

  /* -------------------------------------------------------------  */

  virtual void run() const noexcept(false);

  /* -------------------------------------------------------------  */

protected:

  virtual const SignedData& sign(const Platform& platform,
    const ustr& message, const Image* image = nullptr)
    const noexcept(false);

  virtual const SignedData& verify(const ustr& url)
    const noexcept(false);

  /* -------------------------------------------------------------  */

private:
  SignedData* signed_data;
};

}

/* ***************************************************************  */

#endif /* APP_HPP__  */
