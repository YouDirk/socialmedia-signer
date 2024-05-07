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
#include <openssl/provider.h>

#define CRYPTO_PKEY_NAME           "RSA-PSS"

/* ***************************************************************  */

struct socialmedia_signer::Crypto::private_key
{
  EVP_PKEY* ossl_pkey;
};

/* ---------------------------------------------------------------  */

socialmedia_signer::Crypto*
socialmedia_signer::Crypto::instance = nullptr;

/* -------------------------------------------------------------------
 * Make library dependent private member variables STATIC, to prevent
 * library dependencies in header files.
 */

static OSSL_LIB_CTX* this_ossl_libctx        = nullptr;
static const char*   this_ossl_pkeyctx_propq = nullptr;

/* ***************************************************************  */

socialmedia_signer::Crypto::CryptoErr::CryptoErr(const ustr& reason)
  :Error(), count_others(-1), error_code(0xffffffff),
   file_name(), file_line(-1), lib_name(), func_name(), lib_reason(),
   data(), flags(0xffffffff)
{
  /* For debug purposes:
   *
   * Uncomment this line to print out the full OpenSSL ERR_ stack.
   */
  //ERR_print_errors_fp(stderr);

  this->set_reason(reason);
}

void
socialmedia_signer::Crypto::CryptoErr::set_reason(const ustr& reason)
{
  unsigned long err;
  const char* file_name  = nullptr;
  int         file_line  = -1;
  const char* lib_name   = nullptr;
  const char* func_name  = nullptr;
  const char* lib_reason = nullptr;
  const char* data       = nullptr;
  int         flags      = -1;

  for (this->count_others = -1;
       (err = ERR_get_error_all(&file_name, &file_line, &func_name,
                                &data, &flags)) != 0;
       this->count_others++) {
    this->error_code = err;

    lib_name   = ERR_lib_error_string(this->error_code);
    lib_reason = ERR_reason_error_string(this->error_code);

    if (file_name != nullptr && strlen(file_name) > 0)
      this->file_name = reinterpret_cast<const char8_t*>(file_name);
    if (this->file_line <= 0)
      this->file_line = file_line;
    if (lib_name != nullptr && strlen(lib_name) > 0)
      this->lib_name = reinterpret_cast<const char8_t*>(lib_name);
    if (func_name != nullptr && strlen(func_name) > 0)
      this->func_name = reinterpret_cast<const char8_t*>(func_name);
    if (lib_reason != nullptr && strlen(lib_reason) > 0)
      this->lib_reason = reinterpret_cast<const char8_t*>(lib_reason);
    if (data != nullptr && strlen(data) > 0)
      this->data = reinterpret_cast<const char8_t*>(data);
    if (this->flags < 0)
      this->flags = flags;
  }

  if ( this->file_name.empty())
    this->file_name = u8"<file>";
  if ( this->lib_name.empty())
    this->lib_name = u8"<OpenSSL>";
  if ( this->func_name.empty())
    this->func_name = u8"<function>";
  if ( this->lib_reason.empty())
    this->lib_reason = u8"<no OpenSSL reason>";
  if (!this->data.empty())
    this->data = u8"-> " + this->data;

  // TODO
  Log::debug(ustr::format("******* {}:{}: [{}]::{}() {} {} (flags 0x{:08x})", this->file_name, this->file_line, this->lib_name, this->func_name, this->lib_reason, this->data, (unsigned) this->flags));

  Error::set_reason(ustr::format(
    "Crypto:{}(code 0x{:08x}, {:+} others): {} ({})",
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

  /* default library context of OpenSSL  */
  this_ossl_libctx        = nullptr;

  this_ossl_pkeyctx_propq = nullptr;

  /* Optimizable, i.e. do not load all ciphers and digests.  Config
   * load needed, for CA?
   *
   * OPENSSL_INIT_ASYNC is set by default.
   *
   * OPENSSL_INIT_ENGINE_* are deprecated since OpenSSL 3.0.
   */
  if (OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS
                       | OPENSSL_INIT_LOAD_CRYPTO_STRINGS
                       | OPENSSL_INIT_NO_ADD_ALL_CIPHERS
                       | OPENSSL_INIT_NO_ADD_ALL_DIGESTS
                       | OPENSSL_INIT_NO_LOAD_CONFIG
                    /* | OPENSSL_INIT_NO_ATFORK  */
                       | OPENSSL_INIT_NO_ATEXIT /* FREE in destructor  */
                       , nullptr) == 0) {
    Log::fatal(CryptoErr(u8"Could not OPENSSL_init_ssl()!").uwhat());
  }

  // TODO
#if 0
  if (OSSL_PROVIDER_available(this_ossl_libctx, "fips") == 0) {
    if (OSSL_PROVIDER_set_default_search_path(this_ossl_libctx,
        "/path/to?/openssl-git/providers") == 0) {
      Log::fatal(CryptoErr(u8"Could not set search path to TODO!")
                 .uwhat());
    }

    if (OSSL_LIB_CTX_load_config(this_ossl_libctx,
        "/path/to?/src/openssl.cnf") == 0) {
      Log::fatal(CryptoErr(u8"oooooooooooooooooooooooooooooooooooo!")
                 .uwhat());
    }
  }

  if (OSSL_PROVIDER_available(NULL, "fips") == 0)
    Log::fatal(u8"*******************************************");

  if (EVP_default_properties_enable_fips(this_ossl_libctx, true) == 0) {
    Log::fatal(CryptoErr(u8"Could not enable OpenSSL FIPS support!")
               .uwhat());
  }
#endif
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

struct socialmedia_signer::Crypto::private_key*
socialmedia_signer::Crypto::priv_generate_new() const
{
  EVP_PKEY_CTX* pkey_ctx
    = EVP_PKEY_CTX_new_from_name(this_ossl_libctx, CRYPTO_PKEY_NAME,
                                 this_ossl_pkeyctx_propq);
  if (pkey_ctx == nullptr) {
    Log::fatal(CryptoErr(
      u8"Could not allocate OpenSSL PKEY_CTX!").uwhat());
  }

  // TODO
  EVP_PKEY_CTX_free(pkey_ctx);
  return new private_key {.ossl_pkey = nullptr};
}

void
socialmedia_signer::Crypto::priv_delete(struct private_key* priv_key)
  const
{
  EVP_PKEY_free(priv_key->ossl_pkey);
  delete priv_key;
}

/* ***************************************************************  */
