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


#ifndef APPGUI_HPP__
#define APPGUI_HPP__

#include "App.hpp"

#include "common.hpp"

/* ***************************************************************  */

namespace socialmedia_signer {

/**
 * Class which implements the core functionality of the Socialmedia
 * Signer.
 *
 * This class will be instantiated directly for the GUI version and
 * not compiled for the command-line version.
 */
class AppGui: public App
{
public:
  explicit AppGui();
  virtual ~AppGui();

  /* -------------------------------------------------------------  */

  virtual void run() const noexcept(false) override;

  /* -------------------------------------------------------------  */
};

}

/* ***************************************************************  */

#endif /* APPGUI_HPP__  */
