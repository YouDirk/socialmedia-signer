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


#ifndef IMAGE_HPP__
#define IMAGE_HPP__

#include "common.hpp"

/* ***************************************************************  */

namespace socialmedia_signer {

/**
 * Class which includes data of an image, for example BITMAP, JPEG,
 * PNG, etc...
 */
class Image
{
public:
  /**
   * Open empty image for signature only, without custom background.
   */
  explicit Image();
  /**
   * Open image from `filename` as custom background or as a signature
   * to analyze.
   */
  explicit Image(const ustr& filename);
  virtual ~Image();

  virtual bool is_empty() const;
  virtual const ustr& to_string() const;

private:
  static const ustr EMPTY_IMAGE_STR;

  const ustr filename;
  int* image;
};

}

/* ***************************************************************  */

#endif /* IMAGE_HPP__  */
