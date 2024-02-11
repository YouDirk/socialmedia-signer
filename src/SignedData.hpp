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


#ifndef SIGNEDDATA_HPP__
#define SIGNEDDATA_HPP__

#include "Image.hpp"

#include "common.hpp"

/* ***************************************************************  */

namespace socialmedia_signer {

/**
 * Class which includes all data which is needed to verify.  Is there
 * a signature socialmedia_signer::Image is missing then it can be
 * generated via signing.
 */
class SignedData
{
public:
  explicit SignedData(
    const std::u8string& signed_msg, const Image& signature);
  virtual ~SignedData();

private:
  /**
   * Signed messages are needed to be `char8_t*` to make sure that
   * std::codecvt.in()/.out() won't be executed to prevent conversion
   * errors.
   */
  const std::u8string message;
  const Image signature;
};

}

/* ***************************************************************  */

#endif /* SIGNEDDATA_HPP__  */
