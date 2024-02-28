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


#ifndef CRYPTO_HPP__
#define CRYPTO_HPP__

#include "common.hpp"

/* ***************************************************************  */

namespace socialmedia_signer {

/**
 * Singleton class which can be accessed via Crypto::get().  It
 * provides process wide access to an abstraction layer of the linked
 * `libcrypto` library as part of the OpenSSL SSL/TLS `libssl`
 * library.
 *
 * Official OpenSSL documentation for version 3.2 at
 * https://www.openssl.org/docs/man3.2/index.html
 *
 * Also for interest
 * https://wiki.openssl.org/index.php/API
 */
class Crypto
{
public:
  /* Singleton class  */
  static void init();
  static void release();

  /* Get instance of singleton  */
  static Crypto* get();

private:
  explicit Crypto();
  virtual ~Crypto();

  static Crypto* instance;
};

}

/* ***************************************************************  */

#endif /* CRYPTO_HPP__  */
