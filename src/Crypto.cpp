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


#include "Crypto.hpp"

/* Official OpenSSL documentation for version 3.2 at
 * https://www.openssl.org/docs/man3.2/index.html
 *
 * Also for interest
 * https://wiki.openssl.org/index.php/API
 */

#include <openssl/ssl3.h>
#include <openssl/err.h>

/* ***************************************************************  */

socialmedia_signer::Crypto*
socialmedia_signer::Crypto::instance = nullptr;

/* ***************************************************************  */

socialmedia_signer::Crypto::CryptoErr::CryptoErr(const ustr& reason)
  :Error(), count_others(-1), error_code(0), lib_name(),
   lib_reason()
{
  this->set_reason(reason);
}

void
socialmedia_signer::Crypto::CryptoErr::set_reason(const ustr& reason)
{
  unsigned long err;
  while (0 != (err = ERR_get_error())) {
    this->error_code = err;
    this->count_others++;
  }

  const char* lib_name = ERR_lib_error_string(this->error_code);
  const char* lib_reason = ERR_reason_error_string(this->error_code);

  this->lib_name = lib_name != nullptr
    ? reinterpret_cast<const char8_t*>(lib_name): u8"<OpenSSL>";
  this->lib_reason = lib_reason != nullptr
    ? reinterpret_cast<const char8_t*>(lib_reason): u8"<None>";

  Error::set_reason(ustr::format("SSL:{}(code {}, {:+} others): {} ({})",
    this->lib_name, this->error_code, this->count_others, reason,
    this->lib_reason));
}

/* ***************************************************************  */

socialmedia_signer::Crypto::Crypto()
{
  /* Detailed OpenSSL initialization is described here
   * https://wiki.openssl.org/index.php/Library_Initialization
   *
   * See also:
   * https://wiki.openssl.org/index.php/Libcrypto_API
   */

  /* Optimizable, i.e. do not load all ciphers and digests.  Config
   * load needed, for CA?
   *
   * OPENSSL_INIT_ASYNC is set by default.
   *
   * OPENSSL_INIT_ENGINE_* are deprecated since OpenSSL 3.0.
   */
  if (OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS
                       | OPENSSL_INIT_LOAD_CRYPTO_STRINGS
                       | OPENSSL_INIT_ADD_ALL_CIPHERS /* optimizable  */
                       | OPENSSL_INIT_ADD_ALL_DIGESTS /* optimizable  */
                       | OPENSSL_INIT_NO_LOAD_CONFIG
                    /* | OPENSSL_INIT_NO_ATFORK  */
                       | OPENSSL_INIT_NO_ATEXIT /* FREE in destructor  */
                       , nullptr) == 0) {
    Log::fatal(CryptoErr(u8"Could not OPENSSL_init_ssl()!").uwhat());
  }
}

socialmedia_signer::Crypto::~Crypto()
{
  OPENSSL_cleanup();
}

/* ***************************************************************  */

void
socialmedia_signer::Crypto::init()
{
  if (Crypto::instance != nullptr)
    Log::fatal(u8"Crypto::init(): double call!");

  Crypto::instance = new Crypto();
}

void
socialmedia_signer::Crypto::release()
{
  delete Crypto::instance;
}

socialmedia_signer::Crypto*
socialmedia_signer::Crypto::get()
{
  if (Crypto::instance == nullptr)
    Log::fatal(u8"Crypto::init() not called!");

  return Crypto::instance;
}

/* ***************************************************************  */
