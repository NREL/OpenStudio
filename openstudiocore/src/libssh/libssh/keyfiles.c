/*
 * keyfiles.c - private and public key handling for authentication.
 *
 * This file is part of the SSH Library
 *
 * Copyright (c) 2003-2009 by Aris Adamantiadis
 * Copyright (c) 2009      by Andreas Schneider <mail@cynapses.org>
 *
 * The SSH Library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * The SSH Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the SSH Library; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#include "config.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#include "libssh/priv.h"
#include "libssh/buffer.h"
#include "libssh/keyfiles.h"
#include "libssh/session.h"
#include "libssh/wrapper.h"
#include "libssh/misc.h"
#include "libssh/keys.h"

/*todo: remove this include */
#include "libssh/string.h"


#ifdef HAVE_LIBGCRYPT
#include <gcrypt.h>
#elif defined HAVE_LIBCRYPTO
#include <openssl/pem.h>
#include <openssl/dsa.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#endif /* HAVE_LIBCRYPTO */

#define MAXLINESIZE 80
#define RSA_HEADER_BEGIN "-----BEGIN RSA PRIVATE KEY-----"
#define RSA_HEADER_END "-----END RSA PRIVATE KEY-----"
#define DSA_HEADER_BEGIN "-----BEGIN DSA PRIVATE KEY-----"
#define DSA_HEADER_END "-----END DSA PRIVATE KEY-----"

#ifdef HAVE_LIBGCRYPT

#define MAX_KEY_SIZE 32
#define MAX_PASSPHRASE_SIZE 1024
#define ASN1_INTEGER 2
#define ASN1_SEQUENCE 48
#define PKCS5_SALT_LEN 8

static int load_iv(char *header, unsigned char *iv, int iv_len) {
  int i;
  int j;
  int k;

  memset(iv, 0, iv_len);
  for (i = 0; i < iv_len; i++) {
    if ((header[2*i] >= '0') && (header[2*i] <= '9'))
      j = header[2*i] - '0';
    else if ((header[2*i] >= 'A') && (header[2*i] <= 'F'))
      j = header[2*i] - 'A' + 10;
    else if ((header[2*i] >= 'a') && (header[2*i] <= 'f'))
      j = header[2*i] - 'a' + 10;
    else
      return -1;
    if ((header[2*i+1] >= '0') && (header[2*i+1] <= '9'))
      k = header[2*i+1] - '0';
    else if ((header[2*i+1] >= 'A') && (header[2*i+1] <= 'F'))
      k = header[2*i+1] - 'A' + 10;
    else if ((header[2*i+1] >= 'a') && (header[2*i+1] <= 'f'))
      k = header[2*i+1] - 'a' + 10;
    else
      return -1;
    iv[i] = (j << 4) + k;
  }
  return 0;
}

static uint32_t char_to_u32(unsigned char *data, uint32_t size) {
  uint32_t ret;
  uint32_t i;

  for (i = 0, ret = 0; i < size; ret = ret << 8, ret += data[i++])
    ;
  return ret;
}

static uint32_t asn1_get_len(ssh_buffer buffer) {
  uint32_t len;
  unsigned char tmp[4];

  if (buffer_get_data(buffer,tmp,1) == 0) {
    return 0;
  }

  if (tmp[0] > 127) {
    len = tmp[0] & 127;
    if (len > 4) {
      return 0; /* Length doesn't fit in u32. Can this really happen? */
    }
    if (buffer_get_data(buffer,tmp,len) == 0) {
      return 0;
    }
    len = char_to_u32(tmp, len);
  } else {
    len = char_to_u32(tmp, 1);
  }

  return len;
}

static ssh_string asn1_get_int(ssh_buffer buffer) {
  ssh_string str;
  unsigned char type;
  uint32_t size;

  if (buffer_get_data(buffer, &type, 1) == 0 || type != ASN1_INTEGER) {
    return NULL;
  }
  size = asn1_get_len(buffer);
  if (size == 0) {
    return NULL;
  }

  str = string_new(size);
  if (str == NULL) {
    return NULL;
  }

  if (buffer_get_data(buffer, str->string, size) == 0) {
    string_free(str);
    return NULL;
  }

  return str;
}

static int asn1_check_sequence(ssh_buffer buffer) {
  unsigned char *j = NULL;
  unsigned char tmp;
  int i;
  uint32_t size;
  uint32_t padding;

  if (buffer_get_data(buffer, &tmp, 1) == 0 || tmp != ASN1_SEQUENCE) {
    return 0;
  }

  size = asn1_get_len(buffer);
  if ((padding = buffer_get_len(buffer) - buffer->pos - size) > 0) {
    for (i = buffer_get_len(buffer) - buffer->pos - size,
         j = (unsigned char*)buffer_get(buffer) + size + buffer->pos;
         i;
         i--, j++)
    {
      if (*j != padding) {                   /* padding is allowed */
        return 0;                            /* but nothing else */
      }
    }
  }

  return 1;
}

static int read_line(char *data, unsigned int len, FILE *fp) {
  char tmp;
  unsigned int i;

  for (i = 0; fread(&tmp, 1, 1, fp) && tmp != '\n' && i < len; data[i++] = tmp)
    ;
  if (tmp == '\n') {
    return i;
  }

  if (i >= len) {
    return -1;
  }

  return 0;
}

static int passphrase_to_key(char *data, unsigned int datalen,
    unsigned char *salt, unsigned char *key, unsigned int keylen) {
  MD5CTX md;
  unsigned char digest[MD5_DIGEST_LEN] = {0};
  unsigned int i;
  unsigned int j;
  unsigned int md_not_empty;

  for (j = 0, md_not_empty = 0; j < keylen; ) {
    md = md5_init();
    if (md == NULL) {
      return -1;
    }

    if (md_not_empty) {
      md5_update(md, digest, MD5_DIGEST_LEN);
    } else {
      md_not_empty = 1;
    }

    md5_update(md, data, datalen);
    if (salt) {
      md5_update(md, salt, PKCS5_SALT_LEN);
    }
    md5_final(digest, md);

    for (i = 0; j < keylen && i < MD5_DIGEST_LEN; j++, i++) {
      if (key) {
        key[j] = digest[i];
      }
    }
  }

  return 0;
}

static int privatekey_decrypt(int algo, int mode, unsigned int key_len,
                       unsigned char *iv, unsigned int iv_len,
                       ssh_buffer data, ssh_auth_callback cb,
                       void *userdata,
                       const char *desc)
{
  char passphrase[MAX_PASSPHRASE_SIZE] = {0};
  unsigned char key[MAX_KEY_SIZE] = {0};
  unsigned char *tmp = NULL;
  gcry_cipher_hd_t cipher;
  int rc = -1;

  if (!algo) {
    return -1;
  }

  if (cb) {
    rc = (*cb)(desc, passphrase, MAX_PASSPHRASE_SIZE, 0, 0, userdata);
    if (rc < 0) {
      return -1;
    }
  } else if (cb == NULL && userdata != NULL) {
    snprintf(passphrase, MAX_PASSPHRASE_SIZE, "%s", (char *) userdata);
  }

  if (passphrase_to_key(passphrase, strlen(passphrase), iv, key, key_len) < 0) {
    return -1;
  }

  if (gcry_cipher_open(&cipher, algo, mode, 0)
      || gcry_cipher_setkey(cipher, key, key_len)
      || gcry_cipher_setiv(cipher, iv, iv_len)
      || (tmp = malloc(buffer_get_len(data) * sizeof (char))) == NULL
      || gcry_cipher_decrypt(cipher, tmp, buffer_get_len(data),
                       buffer_get(data), buffer_get_len(data))) {
    gcry_cipher_close(cipher);
    return -1;
  }

  memcpy(buffer_get(data), tmp, buffer_get_len(data));

  SAFE_FREE(tmp);
  gcry_cipher_close(cipher);

  return 0;
}

static int privatekey_dek_header(char *header, unsigned int header_len,
    int *algo, int *mode, unsigned int *key_len, unsigned char **iv,
    unsigned int *iv_len) {
  unsigned int iv_pos;

  if (header_len > 13 && !strncmp("DES-EDE3-CBC", header, 12))
  {
    *algo = GCRY_CIPHER_3DES;
    iv_pos = 13;
    *mode = GCRY_CIPHER_MODE_CBC;
    *key_len = 24;
    *iv_len = 8;
  }
  else if (header_len > 8 && !strncmp("DES-CBC", header, 7))
  {
    *algo = GCRY_CIPHER_DES;
    iv_pos = 8;
    *mode = GCRY_CIPHER_MODE_CBC;
    *key_len = 8;
    *iv_len = 8;
  }
  else if (header_len > 12 && !strncmp("AES-128-CBC", header, 11))
  {
    *algo = GCRY_CIPHER_AES128;
    iv_pos = 12;
    *mode = GCRY_CIPHER_MODE_CBC;
    *key_len = 16;
    *iv_len = 16;
  }
  else if (header_len > 12 && !strncmp("AES-192-CBC", header, 11))
  {
    *algo = GCRY_CIPHER_AES192;
    iv_pos = 12;
    *mode = GCRY_CIPHER_MODE_CBC;
    *key_len = 24;
    *iv_len = 16;
  }
  else if (header_len > 12 && !strncmp("AES-256-CBC", header, 11))
  {
    *algo = GCRY_CIPHER_AES256;
    iv_pos = 12;
    *mode = GCRY_CIPHER_MODE_CBC;
    *key_len = 32;
    *iv_len = 16;
  } else {
    return -1;
  }

  *iv = malloc(*iv_len);
  if (*iv == NULL) {
    return -1;
  }

  return load_iv(header + iv_pos, *iv, *iv_len);
}

static ssh_buffer privatekey_file_to_buffer(FILE *fp, int type,
    ssh_auth_callback cb, void *userdata, const char *desc) {
  ssh_buffer buffer = NULL;
  ssh_buffer out = NULL;
  char buf[MAXLINESIZE] = {0};
  unsigned char *iv = NULL;
  const char *header_begin;
  const char *header_end;
  unsigned int header_begin_size;
  unsigned int header_end_size;
  unsigned int key_len = 0;
  unsigned int iv_len = 0;
  int algo = 0;
  int mode = 0;
  int len;

  buffer = buffer_new();
  if (buffer == NULL) {
    return NULL;
  }

  switch(type) {
    case TYPE_DSS:
      header_begin = DSA_HEADER_BEGIN;
      header_end = DSA_HEADER_END;
      break;
    case TYPE_RSA:
      header_begin = RSA_HEADER_BEGIN;
      header_end = RSA_HEADER_END;
      break;
    default:
      buffer_free(buffer);
      return NULL;
  }

  header_begin_size = strlen(header_begin);
  header_end_size = strlen(header_end);

  while (read_line(buf, MAXLINESIZE, fp) &&
      strncmp(buf, header_begin, header_begin_size))
    ;

  len = read_line(buf, MAXLINESIZE, fp);
  if (len > 11 && strncmp("Proc-Type: 4,ENCRYPTED", buf, 11) == 0) {
    len = read_line(buf, MAXLINESIZE, fp);
    if (len > 10 && strncmp("DEK-Info: ", buf, 10) == 0) {
      if ((privatekey_dek_header(buf + 10, len - 10, &algo, &mode, &key_len,
                                 &iv, &iv_len) < 0)
          || read_line(buf, MAXLINESIZE, fp)) {
        buffer_free(buffer);
        SAFE_FREE(iv);
        return NULL;
      }
    } else {
      buffer_free(buffer);
      SAFE_FREE(iv);
      return NULL;
    }
  } else {
    if (buffer_add_data(buffer, buf, len) < 0) {
      buffer_free(buffer);
      SAFE_FREE(iv);
      return NULL;
    }
  }

  while ((len = read_line(buf,MAXLINESIZE,fp)) &&
      strncmp(buf, header_end, header_end_size) != 0) {
    if (len == -1) {
      buffer_free(buffer);
      SAFE_FREE(iv);
      return NULL;
    }
    if (buffer_add_data(buffer, buf, len) < 0) {
      buffer_free(buffer);
      SAFE_FREE(iv);
      return NULL;
    }
  }

  if (strncmp(buf,header_end,header_end_size) != 0) {
    buffer_free(buffer);
    SAFE_FREE(iv);
    return NULL;
  }

  if (buffer_add_data(buffer, "\0", 1) < 0) {
    buffer_free(buffer);
    SAFE_FREE(iv);
    return NULL;
  }

  out = base64_to_bin(buffer_get(buffer));
  buffer_free(buffer);
  if (out == NULL) {
    SAFE_FREE(iv);
    return NULL;
  }

  if (algo) {
    if (privatekey_decrypt(algo, mode, key_len, iv, iv_len, out,
          cb, userdata, desc) < 0) {
      buffer_free(out);
      SAFE_FREE(iv);
      return NULL;
    }
  }
  SAFE_FREE(iv);

  return out;
}

static int read_rsa_privatekey(FILE *fp, gcry_sexp_t *r,
    ssh_auth_callback cb, void *userdata, const char *desc) {
  ssh_string n = NULL;
  ssh_string e = NULL;
  ssh_string d = NULL;
  ssh_string p = NULL;
  ssh_string q = NULL;
  ssh_string unused1 = NULL;
  ssh_string unused2 = NULL;
  ssh_string u = NULL;
  ssh_string v = NULL;
  ssh_buffer buffer = NULL;
  int rc = 1;

  buffer = privatekey_file_to_buffer(fp, TYPE_RSA, cb, userdata, desc);
  if (buffer == NULL) {
    return 0;
  }

  if (!asn1_check_sequence(buffer)) {
    buffer_free(buffer);
    return 0;
  }

  v = asn1_get_int(buffer);
  if (ntohl(v->size) != 1 || v->string[0] != 0) {
    buffer_free(buffer);
    return 0;
  }

  n = asn1_get_int(buffer);
  e = asn1_get_int(buffer);
  d = asn1_get_int(buffer);
  q = asn1_get_int(buffer);
  p = asn1_get_int(buffer);
  unused1 = asn1_get_int(buffer);
  unused2 = asn1_get_int(buffer);
  u = asn1_get_int(buffer);

  buffer_free(buffer);

  if (n == NULL || e == NULL || d == NULL || p == NULL || q == NULL ||
      unused1 == NULL || unused2 == NULL|| u == NULL) {
    rc = 0;
    goto error;
  }

  if (gcry_sexp_build(r, NULL,
      "(private-key(rsa(n %b)(e %b)(d %b)(p %b)(q %b)(u %b)))",
      ntohl(n->size), n->string,
      ntohl(e->size), e->string,
      ntohl(d->size), d->string,
      ntohl(p->size), p->string,
      ntohl(q->size), q->string,
      ntohl(u->size), u->string)) {
    rc = 0;
  }

error:
  string_free(n);
  string_free(e);
  string_free(d);
  string_free(p);
  string_free(q);
  string_free(unused1);
  string_free(unused2);
  string_free(u);
  string_free(v);

  return rc;
}

static int read_dsa_privatekey(FILE *fp, gcry_sexp_t *r, ssh_auth_callback cb,
    void *userdata, const char *desc) {
  ssh_buffer buffer = NULL;
  ssh_string p = NULL;
  ssh_string q = NULL;
  ssh_string g = NULL;
  ssh_string y = NULL;
  ssh_string x = NULL;
  ssh_string v = NULL;
  int rc = 1;

  buffer = privatekey_file_to_buffer(fp, TYPE_DSS, cb, userdata, desc);
  if (buffer == NULL) {
    return 0;
  }

  if (!asn1_check_sequence(buffer)) {
    buffer_free(buffer);
    return 0;
  }

  v = asn1_get_int(buffer);
  if (ntohl(v->size) != 1 || v->string[0] != 0) {
    buffer_free(buffer);
    return 0;
  }

  p = asn1_get_int(buffer);
  q = asn1_get_int(buffer);
  g = asn1_get_int(buffer);
  y = asn1_get_int(buffer);
  x = asn1_get_int(buffer);
  buffer_free(buffer);

  if (p == NULL || q == NULL || g == NULL || y == NULL || x == NULL) {
    rc = 0;
    goto error;
  }

  if (gcry_sexp_build(r, NULL,
        "(private-key(dsa(p %b)(q %b)(g %b)(y %b)(x %b)))",
        ntohl(p->size), p->string,
        ntohl(q->size), q->string,
        ntohl(g->size), g->string,
        ntohl(y->size), y->string,
        ntohl(x->size), x->string)) {
    rc = 0;
  }

error:
  string_free(p);
  string_free(q);
  string_free(g);
  string_free(y);
  string_free(x);
  string_free(v);

  return rc;
}
#endif /* HAVE_LIBGCRYPT */

#ifdef HAVE_LIBCRYPTO
static int pem_get_password(char *buf, int size, int rwflag, void *userdata) {
  ssh_session session = userdata;

  /* unused flag */
  (void) rwflag;

  ZERO_STRUCTP(buf);
  ssh_log(session, SSH_LOG_RARE,
      "Trying to call external authentication function");

  if (session && session->callbacks->auth_function) {
    if (session->callbacks->auth_function("Passphrase for private key:", buf, size, 0, 0,
        session->callbacks->userdata) < 0) {
      return 0;
    }

    return strlen(buf);
  }

  return 0;
}
#endif /* HAVE_LIBCRYPTO */

static int privatekey_type_from_file(FILE *fp) {
  char buffer[MAXLINESIZE] = {0};

  if (!fgets(buffer, MAXLINESIZE, fp)) {
    return 0;
  }
  fseek(fp, 0, SEEK_SET);
  if (strncmp(buffer, DSA_HEADER_BEGIN, strlen(DSA_HEADER_BEGIN)) == 0) {
    return TYPE_DSS;
  }
  if (strncmp(buffer, RSA_HEADER_BEGIN, strlen(RSA_HEADER_BEGIN)) == 0) {
    return TYPE_RSA;
  }
  return 0;
}

/** \addtogroup ssh_auth
 * @{
 */
/* TODO : implement it to read both DSA and RSA at once */
/** \brief Reads a SSH private key from a file
 * \param session SSH Session
 * \param filename Filename containing the private key
 * \param type Type of the private key. One of TYPE_DSS or TYPE_RSA. Pass 0 to automatically detect the type.
 * \param passphrase Passphrase to decrypt the private key. Set to null if none is needed or it is unknown.
 * \returns a PRIVATE_KEY object containing the private key, or NULL if it failed.
 * \see privatekey_free()
 * \see publickey_from_privatekey()
 */
ssh_private_key privatekey_from_file(ssh_session session, const char *filename,
    int type, const char *passphrase) {
  ssh_auth_callback auth_cb = NULL;
  ssh_private_key privkey = NULL;
  void *auth_ud = NULL;
  FILE *file = NULL;
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_t dsa = NULL;
  gcry_sexp_t rsa = NULL;
  int valid;
#elif defined HAVE_LIBCRYPTO
  DSA *dsa = NULL;
  RSA *rsa = NULL;
#endif
  ssh_log(session, SSH_LOG_RARE, "Trying to open %s", filename);
  file = fopen(filename,"r");
  if (file == NULL) {
    ssh_set_error(session, SSH_REQUEST_DENIED,
        "Error opening %s: %s", filename, strerror(errno));
    return NULL;
  }

  ssh_log(session, SSH_LOG_RARE, "Trying to read %s, passphase=%s, authcb=%s",
      filename, passphrase ? "true" : "false",
      session->callbacks && session->callbacks->auth_function ? "true" : "false");

  if (type == 0) {
    type = privatekey_type_from_file(file);
    if (type == 0) {
      fclose(file);
      ssh_set_error(session, SSH_FATAL, "Invalid private key file.");
      return NULL;
    }
  }
  switch (type) {
    case TYPE_DSS:
      if (passphrase == NULL) {
        if (session->callbacks && session->callbacks->auth_function) {
          auth_cb = session->callbacks->auth_function;
          auth_ud = session->callbacks->userdata;

#ifdef HAVE_LIBGCRYPT
          valid = read_dsa_privatekey(file, &dsa, auth_cb, auth_ud,
              "Passphrase for private key:");
        } else { /* authcb */
          valid = read_dsa_privatekey(file, &dsa, NULL, NULL, NULL);
        } /* authcb */
      } else { /* passphrase */
        valid = read_dsa_privatekey(file, &dsa, NULL,
            (void *) passphrase, NULL);
      }

      fclose(file);

      if (!valid) {
        ssh_set_error(session, SSH_FATAL, "Parsing private key %s", filename);
#elif defined HAVE_LIBCRYPTO
          dsa = PEM_read_DSAPrivateKey(file, NULL, pem_get_password, session);
        } else { /* authcb */
          /* openssl uses it's own callback to get the passphrase here */
          dsa = PEM_read_DSAPrivateKey(file, NULL, NULL, NULL);
        } /* authcb */
      } else { /* passphrase */
        dsa = PEM_read_DSAPrivateKey(file, NULL, NULL, (void *) passphrase);
      }

      fclose(file);
      if (dsa == NULL) {
        ssh_set_error(session, SSH_FATAL,
            "Parsing private key %s: %s",
            filename, ERR_error_string(ERR_get_error(), NULL));
#endif
        return NULL;
      }
      break;
    case TYPE_RSA:
      if (passphrase == NULL) {
	if (session->callbacks && session->callbacks->auth_function) {
		auth_cb = session->callbacks->auth_function;
		auth_ud = session->callbacks->userdata;
#ifdef HAVE_LIBGCRYPT
          valid = read_rsa_privatekey(file, &rsa, auth_cb, auth_ud,
              "Passphrase for private key:");
        } else { /* authcb */
          valid = read_rsa_privatekey(file, &rsa, NULL, NULL, NULL);
        } /* authcb */
      } else { /* passphrase */
        valid = read_rsa_privatekey(file, &rsa, NULL,
            (void *) passphrase, NULL);
      }

      fclose(file);

      if (!valid) {
        ssh_set_error(session,SSH_FATAL, "Parsing private key %s", filename);
#elif defined HAVE_LIBCRYPTO
          rsa = PEM_read_RSAPrivateKey(file, NULL, pem_get_password, session);
        } else { /* authcb */
          /* openssl uses it's own callback to get the passphrase here */
          rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL);
        } /* authcb */
      } else { /* passphrase */
        rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, (void *) passphrase);
      }

      fclose(file);

      if (rsa == NULL) {
        ssh_set_error(session, SSH_FATAL,
            "Parsing private key %s: %s",
            filename, ERR_error_string(ERR_get_error(),NULL));
#endif
        return NULL;
      }
      break;
    default:
      fclose(file);
      ssh_set_error(session, SSH_FATAL, "Invalid private key type %d", type);
      return NULL;
  } /* switch */

  privkey = malloc(sizeof(struct ssh_private_key_struct));
  if (privkey == NULL) {
#ifdef HAVE_LIBGCRYPT
    gcry_sexp_release(dsa);
    gcry_sexp_release(rsa);
#elif defined HAVE_LIBCRYPTO
    DSA_free(dsa);
    RSA_free(rsa);
#endif
    return NULL;
  }

  privkey->type = type;
  privkey->dsa_priv = dsa;
  privkey->rsa_priv = rsa;

  return privkey;
}

/**
 * @brief returns the type of a private key
 * @param privatekey[in] the private key handle
 * @returns one of TYPE_RSA,TYPE_DSS,TYPE_RSA1
 * @returns 0 if the type is unknown
 * @see privatekey_from_file
 * @see ssh_userauth_offer_pubkey
 */
int ssh_privatekey_type(ssh_private_key privatekey){
  if (privatekey==NULL)
    return 0;
  return privatekey->type;
}

/* same that privatekey_from_file() but without any passphrase things. */
ssh_private_key _privatekey_from_file(void *session, const char *filename,
    int type) {
  ssh_private_key privkey = NULL;
  FILE *file = NULL;
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_t dsa = NULL;
  gcry_sexp_t rsa = NULL;
  int valid;
#elif defined HAVE_LIBCRYPTO
  DSA *dsa = NULL;
  RSA *rsa = NULL;
#endif

  file = fopen(filename,"r");
  if (file == NULL) {
    ssh_set_error(session, SSH_REQUEST_DENIED,
        "Error opening %s: %s", filename, strerror(errno));
    return NULL;
  }

  switch (type) {
    case TYPE_DSS:
#ifdef HAVE_LIBGCRYPT
      valid = read_dsa_privatekey(file, &dsa, NULL, NULL, NULL);

      fclose(file);

      if (!valid) {
        ssh_set_error(session, SSH_FATAL, "Parsing private key %s", filename);
#elif defined HAVE_LIBCRYPTO
      dsa = PEM_read_DSAPrivateKey(file, NULL, NULL, NULL);

      fclose(file);

      if (dsa == NULL) {
        ssh_set_error(session, SSH_FATAL,
            "Parsing private key %s: %s",
            filename, ERR_error_string(ERR_get_error(), NULL));
#endif
        return NULL;
      }
      break;
    case TYPE_RSA:
#ifdef HAVE_LIBGCRYPT
      valid = read_rsa_privatekey(file, &rsa, NULL, NULL, NULL);

      fclose(file);

      if (!valid) {
        ssh_set_error(session, SSH_FATAL, "Parsing private key %s", filename);
#elif defined HAVE_LIBCRYPTO
      rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL);

      fclose(file);

      if (rsa == NULL) {
        ssh_set_error(session, SSH_FATAL,
            "Parsing private key %s: %s",
            filename, ERR_error_string(ERR_get_error(), NULL));
#endif
        return NULL;
      }
      break;
    default:
        ssh_set_error(session, SSH_FATAL, "Invalid private key type %d", type);
        return NULL;
  }

  privkey = malloc(sizeof(struct ssh_private_key_struct));
  if (privkey == NULL) {
#ifdef HAVE_LIBGCRYPT
    gcry_sexp_release(dsa);
    gcry_sexp_release(rsa);
#elif defined HAVE_LIBCRYPTO
    DSA_free(dsa);
    RSA_free(rsa);
#endif
    return NULL;
  }

  privkey->type = type;
  privkey->dsa_priv = dsa;
  privkey->rsa_priv = rsa;

  return privkey;
}

/** \brief deallocate a private key
 * \param prv a PRIVATE_KEY object
 */
void privatekey_free(ssh_private_key prv) {
  if (prv == NULL) {
    return;
  }

#ifdef HAVE_LIBGCRYPT
  gcry_sexp_release(prv->dsa_priv);
  gcry_sexp_release(prv->rsa_priv);
#elif defined HAVE_LIBCRYPTO
  DSA_free(prv->dsa_priv);
  RSA_free(prv->rsa_priv);
#endif
  memset(prv, 0, sizeof(struct ssh_private_key_struct));
  SAFE_FREE(prv);
}

/**
 * @brief Write a public key to a file.
 *
 * @param[in]  session  The ssh session to use.
 *
 * @param[in]  file     The filename to write the key into.
 *
 * @param[in]  pubkey   The public key to write.
 *
 * @param[in]  type     The type of the public key.
 *
 * @return              0 on success, -1 on error.
 */
int ssh_publickey_to_file(ssh_session session, const char *file,
    ssh_string pubkey, int type) {
  FILE *fp;
  char *user;
  char buffer[1024];
  char host[256];
  unsigned char *pubkey_64;
  size_t len;
  int rc;

  pubkey_64 = bin_to_base64(pubkey->string, string_len(pubkey));
  if (pubkey_64 == NULL) {
    return -1;
  }

  user = ssh_get_local_username(session);
  if (user == NULL) {
    SAFE_FREE(pubkey_64);
    return -1;
  }

  rc = gethostname(host, sizeof(host));
  if (rc < 0) {
    SAFE_FREE(user);
    SAFE_FREE(pubkey_64);
    return -1;
  }

  snprintf(buffer, sizeof(buffer), "%s %s %s@%s\n",
      ssh_type_to_char(type),
      pubkey_64,
      user,
      host);

  SAFE_FREE(pubkey_64);
  SAFE_FREE(user);

  ssh_log(session, SSH_LOG_RARE, "Trying to write public key file: %s", file);
  ssh_log(session, SSH_LOG_PACKET, "public key file content: %s", buffer);

  fp = fopen(file, "w+");
  if (fp == NULL) {
    ssh_set_error(session, SSH_REQUEST_DENIED,
        "Error opening %s: %s", file, strerror(errno));
    return -1;
  }

  len = strlen(buffer);
  if (fwrite(buffer, len, 1, fp) != 1 || ferror(fp)) {
    ssh_set_error(session, SSH_REQUEST_DENIED,
        "Unable to write to %s", file);
    fclose(fp);
    unlink(file);
    return -1;
  }

  fclose(fp);
  return 0;
}

/** \brief Retrieve a public key from a file
 * \param session the SSH session
 * \param filename Filename of the key
 * \param type Pointer to a integer. If it is not null, it contains the type of the key after execution.
 * \return a SSH String containing the public key, or NULL if it failed.
 * \see string_free()
 * \see publickey_from_privatekey()
 */
ssh_string publickey_from_file(ssh_session session, const char *filename,
    int *type) {
  ssh_buffer buffer = NULL;
  char buf[4096] = {0};
  ssh_string str = NULL;
  char *ptr = NULL;
  int key_type;
  int fd = -1;
  int r;

  fd = open(filename, O_RDONLY);
  if (fd < 0) {
    ssh_set_error(session, SSH_REQUEST_DENIED, "Public key file doesn't exist");
    return NULL;
  }

  if (read(fd, buf, 8) != 8) {
    close(fd);
    ssh_set_error(session, SSH_REQUEST_DENIED, "Invalid public key file");
    return NULL;
  }

  buf[7] = '\0';

  key_type = ssh_type_from_name(buf);
  if (key_type == -1) {
    close(fd);
    ssh_set_error(session, SSH_REQUEST_DENIED, "Invalid public key file");
    return NULL;
  }

  r = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (r <= 0) {
    ssh_set_error(session, SSH_REQUEST_DENIED, "Invalid public key file");
    return NULL;
  }

  buf[r] = 0;
  ptr = strchr(buf, ' ');

  /* eliminate the garbage at end of file */
  if (ptr) {
    *ptr = '\0';
  }

  buffer = base64_to_bin(buf);
  if (buffer == NULL) {
    ssh_set_error(session, SSH_REQUEST_DENIED, "Invalid public key file");
    return NULL;
  }

  str = string_new(buffer_get_len(buffer));
  if (str == NULL) {
    ssh_set_error(session, SSH_FATAL, "Not enough space");
    buffer_free(buffer);
    return NULL;
  }

  string_fill(str, buffer_get(buffer), buffer_get_len(buffer));
  buffer_free(buffer);

  if (type) {
    *type = key_type;
  }

  return str;
}

/**
 * @brief Try to read the public key from a given file.
 *
 * @param[in]  session  The ssh session to use.
 *
 * @param[in]  keyfile  The name of the private keyfile.
 *
 * @param[out] publickey A ssh_string to store the public key.
 *
 * @param[out] type     A pointer to an integer to store the type.
 *
 * @return              0 on success, -1 on error or the private key doesn't
 *                      exist, 1 if the public key doesn't exist.
 */
int ssh_try_publickey_from_file(ssh_session session, const char *keyfile,
    ssh_string *publickey, int *type) {
  char *pubkey_file;
  size_t len;
  ssh_string pubkey_string;
  int pubkey_type;

  if (session == NULL || keyfile == NULL || publickey == NULL || type == NULL) {
    return -1;
  }

  if (session->sshdir == NULL) {
    if (ssh_options_apply(session) < 0) {
      return -1;
    }
  }

  ssh_log(session, SSH_LOG_PACKET, "Trying to open privatekey %s", keyfile);
  if (!ssh_file_readaccess_ok(keyfile)) {
    ssh_log(session, SSH_LOG_PACKET, "Failed to open privatekey %s", keyfile);
    return -1;
  }

  len = strlen(keyfile) + 5;
  pubkey_file = malloc(len);
  if (pubkey_file == NULL) {
    return -1;
  }
  snprintf(pubkey_file, len, "%s.pub", keyfile);

  ssh_log(session, SSH_LOG_PACKET, "Trying to open publickey %s",
                                   pubkey_file);
  if (!ssh_file_readaccess_ok(pubkey_file)) {
    ssh_log(session, SSH_LOG_PACKET, "Failed to open publickey %s",
                                     pubkey_file);
    SAFE_FREE(pubkey_file);
    return 1;
  }

  ssh_log(session, SSH_LOG_PACKET, "Success opening public and private key");

  /*
   * We are sure both the private and public key file is readable. We return
   * the public as a string, and the private filename as an argument
   */
  pubkey_string = publickey_from_file(session, pubkey_file, &pubkey_type);
  if (pubkey_string == NULL) {
    ssh_log(session, SSH_LOG_PACKET,
        "Wasn't able to open public key file %s: %s",
        pubkey_file,
        ssh_get_error(session));
    SAFE_FREE(pubkey_file);
    return -1;
  }

  SAFE_FREE(pubkey_file);

  *publickey = pubkey_string;
  *type = pubkey_type;

  return 0;
}

ssh_string try_publickey_from_file(ssh_session session, struct ssh_keys_struct keytab,
    char **privkeyfile, int *type) {
  const char *priv;
  const char *pub;
  char *new;
  ssh_string pubkey=NULL;

  pub = keytab.publickey;
  if (pub == NULL) {
    return NULL;
  }
  priv = keytab.privatekey;
  if (priv == NULL) {
    return NULL;
  }

  if (session->sshdir == NULL) {
    if (ssh_options_apply(session) < 0) {
      return NULL;
    }
  }

  ssh_log(session, SSH_LOG_PACKET, "Trying to open publickey %s", pub);
  if (!ssh_file_readaccess_ok(pub)) {
    ssh_log(session, SSH_LOG_PACKET, "Failed to open publickey %s", pub);
    goto error;
  }

  ssh_log(session, SSH_LOG_PACKET, "Trying to open privatekey %s", priv);
  if (!ssh_file_readaccess_ok(priv)) {
    ssh_log(session, SSH_LOG_PACKET, "Failed to open privatekey %s", priv);
    goto error;
  }

  ssh_log(session, SSH_LOG_PACKET, "Success opening public and private key");

  /*
   * We are sure both the private and public key file is readable. We return
   * the public as a string, and the private filename as an argument
   */
  pubkey = publickey_from_file(session, pub, type);
  if (pubkey == NULL) {
    ssh_log(session, SSH_LOG_PACKET,
        "Wasn't able to open public key file %s: %s",
        pub,
        ssh_get_error(session));
    goto error;
  }

  new = realloc(*privkeyfile, strlen(priv) + 1);
  if (new == NULL) {
    string_free(pubkey);
    goto error;
  }

  strcpy(new, priv);
  *privkeyfile = new;
error:
  return pubkey;
}

static int alldigits(const char *s) {
  while (*s) {
    if (isdigit(*s)) {
      s++;
    } else {
      return 0;
    }
  }

  return 1;
}

/** @}
 */

/** \addtogroup ssh_session
 * @{ */

/**
 * \brief Lowercase a string.
 * \param  str          String to lowercase.
 * \return              The malloced lowered string or NULL on error.
 * \internal
 */
static char *lowercase(const char* str) {
  char *new, *p;

  if (str == NULL) {
    return NULL;
  }

  new = strdup(str);
  if (new == NULL) {
    return NULL;
  }

  for (p = new; *p; p++) {
    *p = tolower(*p);
  }

  return new;
}

/** \brief frees a token array
 * \internal
 */
static void tokens_free(char **tokens) {
  if (tokens == NULL) {
    return;
  }

  SAFE_FREE(tokens[0]);
  /* It's not needed to free other pointers because tokens generated by
   * space_tokenize fit all in one malloc
   */
  SAFE_FREE(tokens);
}

/** \brief returns one line of known host file
 * will return a token array containing (host|ip) keytype key
 * \param file pointer to the known host file. Could be pointing to NULL at start
 * \param filename file name of the known host file
 * \param found_type pointer to a string to be set with the found key type
 * \internal
 * \returns NULL if no match was found or the file was not found
 * \returns found_type type of key (ie "dsa","ssh-rsa1"). Don't free that value.
 */
static char **ssh_get_knownhost_line(ssh_session session, FILE **file,
    const char *filename, const char **found_type) {
  char buffer[4096] = {0};
  char *ptr;
  char **tokens;

  enter_function();

  if(*file == NULL){
    *file = fopen(filename,"r");
    if (*file == NULL) {
      leave_function();
      return NULL;
    }
  }

  while (fgets(buffer, sizeof(buffer), *file)) {
    ptr = strchr(buffer, '\n');
    if (ptr) {
      *ptr =  '\0';
    }

    ptr = strchr(buffer,'\r');
    if (ptr) {
      *ptr = '\0';
    }

    if (!buffer[0] || buffer[0] == '#') {
      continue; /* skip empty lines */
    }

    tokens = space_tokenize(buffer);
    if (tokens == NULL) {
      fclose(*file);
      *file = NULL;
      leave_function();
      return NULL;
    }

    if(!tokens[0] || !tokens[1] || !tokens[2]) {
      /* it should have at least 3 tokens */
      tokens_free(tokens);
      continue;
    }

    *found_type = tokens[1];
    if (tokens[3]) {
      /* openssh rsa1 format has 4 tokens on the line. Recognize it
         by the fact that everything is all digits */
      if (tokens[4]) {
        /* that's never valid */
        tokens_free(tokens);
        continue;
      }
      if (alldigits(tokens[1]) && alldigits(tokens[2]) && alldigits(tokens[3])) {
        *found_type = "ssh-rsa1";
      } else {
        /* 3 tokens only, not four */
        tokens_free(tokens);
        continue;
      }
    }
    leave_function();
    return tokens;
  }

  fclose(*file);
  *file = NULL;

  /* we did not find anything, end of file*/
  leave_function();
  return NULL;
}

/**
 * \brief Check the public key in the known host line matches the
 * public key of the currently connected server.
 * \param tokens list of tokens in the known_hosts line.
 * \return 1 if the key matches
 * \return 0 if the key doesn't match
 * \return -1 on error
 */
static int check_public_key(ssh_session session, char **tokens) {
  ssh_string pubkey = session->current_crypto->server_pubkey;
  ssh_buffer pubkey_buffer;
  char *pubkey_64;

  /* ok we found some public key in known hosts file. now un-base64it */
  if (alldigits(tokens[1])) {
    /* openssh rsa1 format */
    bignum tmpbn;
    ssh_string tmpstring;
    unsigned int len;
    int i;

    pubkey_buffer = buffer_new();
    if (pubkey_buffer == NULL) {
      return -1;
    }

    tmpstring = string_from_char("ssh-rsa1");
    if (tmpstring == NULL) {
      buffer_free(pubkey_buffer);
      return -1;
    }

    if (buffer_add_ssh_string(pubkey_buffer, tmpstring) < 0) {
      buffer_free(pubkey_buffer);
      string_free(tmpstring);
      return -1;
    }
    string_free(tmpstring);

    for (i = 2; i < 4; i++) { /* e, then n */
      tmpbn = NULL;
      bignum_dec2bn(tokens[i], &tmpbn);
      if (tmpbn == NULL) {
        buffer_free(pubkey_buffer);
        return -1;
      }
      /* for some reason, make_bignum_string does not work
         because of the padding which it does --kv */
      /* tmpstring = make_bignum_string(tmpbn); */
      /* do it manually instead */
      len = bignum_num_bytes(tmpbn);
      tmpstring = malloc(4 + len);
      if (tmpstring == NULL) {
        buffer_free(pubkey_buffer);
        bignum_free(tmpbn);
        return -1;
      }
      /* TODO: fix the hardcoding */
      tmpstring->size = htonl(len);
#ifdef HAVE_LIBGCRYPT
      bignum_bn2bin(tmpbn, len, tmpstring->string);
#elif defined HAVE_LIBCRYPTO
      bignum_bn2bin(tmpbn, tmpstring->string);
#endif
      bignum_free(tmpbn);
      if (buffer_add_ssh_string(pubkey_buffer, tmpstring) < 0) {
        buffer_free(pubkey_buffer);
        string_free(tmpstring);
        bignum_free(tmpbn);
        return -1;
      }
      string_free(tmpstring);
    }
  } else {
    /* ssh-dss or ssh-rsa */
    pubkey_64 = tokens[2];
    pubkey_buffer = base64_to_bin(pubkey_64);
  }

  if (pubkey_buffer == NULL) {
    ssh_set_error(session, SSH_FATAL,
        "Verifying that server is a known host: base64 error");
    return -1;
  }

  if (buffer_get_len(pubkey_buffer) != string_len(pubkey)) {
    buffer_free(pubkey_buffer);
    return 0;
  }

  /* now test that they are identical */
  if (memcmp(buffer_get(pubkey_buffer), pubkey->string,
        buffer_get_len(pubkey_buffer)) != 0) {
    buffer_free(pubkey_buffer);
    return 0;
  }

  buffer_free(pubkey_buffer);
  return 1;
}

/**
 * \brief checks if a hostname matches a openssh-style hashed known host
 * \param host host to check
 * \param hashed hashed value
 * \returns 1 if it matches
 * \returns 0 otherwise
 */
static int match_hashed_host(ssh_session session, const char *host,
    const char *sourcehash) {
  /* Openssh hash structure :
   * |1|base64 encoded salt|base64 encoded hash
   * hash is produced that way :
   * hash := HMAC_SHA1(key=salt,data=host)
   */
  unsigned char buffer[256] = {0};
  ssh_buffer salt;
  ssh_buffer hash;
  HMACCTX mac;
  char *source;
  char *b64hash;
  int match;
  unsigned int size;

  enter_function();

  if (strncmp(sourcehash, "|1|", 3) != 0) {
    return 0;
  }

  source = strdup(sourcehash + 3);
  if (source == NULL) {
    leave_function();
    return 0;
  }

  b64hash = strchr(source, '|');
  if (b64hash == NULL) {
    /* Invalid hash */
    SAFE_FREE(source);
    leave_function();
    return 0;
  }

  *b64hash = '\0';
  b64hash++;

  salt = base64_to_bin(source);
  if (salt == NULL) {
    SAFE_FREE(source);
    leave_function();
    return 0;
  }

  hash = base64_to_bin(b64hash);
  SAFE_FREE(source);
  if (hash == NULL) {
    buffer_free(salt);
    leave_function();
    return 0;
  }

  mac = hmac_init(buffer_get(salt), buffer_get_len(salt), HMAC_SHA1);
  if (mac == NULL) {
    buffer_free(salt);
    buffer_free(hash);
    leave_function();
    return 0;
  }
  size = sizeof(buffer);
  hmac_update(mac, host, strlen(host));
  hmac_final(mac, buffer, &size);

  if (size == buffer_get_len(hash) &&
      memcmp(buffer, buffer_get(hash), size) == 0) {
    match = 1;
  } else {
    match = 0;
  }

  buffer_free(salt);
  buffer_free(hash);

  ssh_log(session, SSH_LOG_PACKET,
      "Matching a hashed host: %s match=%d", host, match);

  leave_function();
  return match;
}

/* How it's working :
 * 1- we open the known host file and bitch if it doesn't exist
 * 2- we need to examine each line of the file, until going on state SSH_SERVER_KNOWN_OK:
 *  - there's a match. if the key is good, state is SSH_SERVER_KNOWN_OK,
 *    else it's SSH_SERVER_KNOWN_CHANGED (or SSH_SERVER_FOUND_OTHER)
 *  - there's no match : no change
 */

/**
 * \brief Check if the server is known.
 * Checks the user's known host file for a previous connection to the
 * current server.
 *
 * \param session ssh session
 *
 * \return SSH_SERVER_KNOWN_OK:      The server is known and has not changed\n
 *         SSH_SERVER_KNOWN_CHANGED: The server key has changed. Either you are
 *                                   under attack or the administrator changed
 *                                   the key. You HAVE to warn the user about
 *                                   a possible attack\n
 *         SSH_SERVER_FOUND_OTHER:   The server gave use a key of a type while
 *                                   we had an other type recorded. It is a
 *                                   possible attack \n
 *         SSH_SERVER_NOT_KNOWN:     The server is unknown. User should confirm
 *                                   the MD5 is correct\n
 *         SSH_SERVER_FILE_NOT_FOUND:The known host file does not exist. The
 *                                   host is thus unknown. File will be created
 *                                   if host key is accepted\n
 *         SSH_SERVER_ERROR:         Some error happened
 *
 * \see ssh_get_pubkey_hash()
 *
 * \bug There is no current way to remove or modify an entry into the known
 * host table.
 */
int ssh_is_server_known(ssh_session session) {
  FILE *file = NULL;
  char **tokens;
  char *host;
  char *hostport;
  const char *type;
  int match;
  int ret = SSH_SERVER_NOT_KNOWN;

  enter_function();

  if (session->knownhosts == NULL) {
    if (ssh_options_apply(session) < 0) {
      ssh_set_error(session, SSH_REQUEST_DENIED,
          "Can't find a known_hosts file");
      leave_function();
      return SSH_SERVER_FILE_NOT_FOUND;
    }
  }

  if (session->host == NULL) {
    ssh_set_error(session, SSH_FATAL,
        "Can't verify host in known hosts if the hostname isn't known");
    leave_function();
    return SSH_SERVER_ERROR;
  }

  host = lowercase(session->host);
  hostport = ssh_hostport(host,session->port);
  if (host == NULL || hostport == NULL) {
    ssh_set_error_oom(session);
    SAFE_FREE(host);
    SAFE_FREE(hostport);
    leave_function();
    return SSH_SERVER_ERROR;
  }

  do {
    tokens = ssh_get_knownhost_line(session, &file,
        session->knownhosts, &type);

    /* End of file, return the current state */
    if (tokens == NULL) {
      break;
    }
    match = match_hashed_host(session, host, tokens[0]);
    if (match == 0){
    	match = match_hostname(hostport, tokens[0], strlen(tokens[0]));
    }
    if (match == 0) {
      match = match_hostname(host, tokens[0], strlen(tokens[0]));
    }
    if (match == 0) {
      match = match_hashed_host(session, hostport, tokens[0]);
    }
    if (match) {
      /* We got a match. Now check the key type */
      if (strcmp(session->current_crypto->server_pubkey_type, type) != 0) {
        /* Different type. We don't override the known_changed error which is
         * more important */
        if (ret != SSH_SERVER_KNOWN_CHANGED)
          ret = SSH_SERVER_FOUND_OTHER;
        tokens_free(tokens);
        continue;
      }
      /* so we know the key type is good. We may get a good key or a bad key. */
      match = check_public_key(session, tokens);
      tokens_free(tokens);

      if (match < 0) {
        ret = SSH_SERVER_ERROR;
        break;
      } else if (match == 1) {
        ret = SSH_SERVER_KNOWN_OK;
        break;
      } else if(match == 0) {
        /* We override the status with the wrong key state */
        ret = SSH_SERVER_KNOWN_CHANGED;
      }
    } else {
      tokens_free(tokens);
    }
  } while (1);

  SAFE_FREE(host);
  SAFE_FREE(hostport);
  if (file != NULL) {
    fclose(file);
  }

  /* Return the current state at end of file */
  leave_function();
  return ret;
}

/**
 * @brief Write the current server as known in the known hosts file.
 *
 * This will create the known hosts file if it does not exist. You generaly use
 * it when ssh_is_server_known() answered SSH_SERVER_NOT_KNOWN.
 *
 * @param[in]  session  The ssh session to use.
 *
 * @return              SSH_OK on success, SSH_ERROR on error.
 */
int ssh_write_knownhost(ssh_session session) {
  ssh_string pubkey = session->current_crypto->server_pubkey;
  unsigned char *pubkey_64;
  char buffer[4096] = {0};
  FILE *file;
  char *dir;
  char *host;
  char *hostport;
  size_t len = 0;

  if (session->host == NULL) {
    ssh_set_error(session, SSH_FATAL,
        "Can't write host in known hosts if the hostname isn't known");
    return SSH_ERROR;
  }

  host = lowercase(session->host);
  /* If using a nonstandard port, save the host in the [host]:port format */
  if(session->port != 22){
    hostport = ssh_hostport(host,session->port);
    SAFE_FREE(host);
    host=hostport;
    hostport=NULL;
  }

  if (session->knownhosts == NULL) {
    if (ssh_options_apply(session) < 0) {
      ssh_set_error(session, SSH_FATAL, "Can't find a known_hosts file");
      return -1;
    }
  }

  /* Check if ~/.ssh exists and create it if not */
  dir = ssh_dirname(session->knownhosts);
  if (dir == NULL) {
    ssh_set_error(session, SSH_FATAL, "%s", strerror(errno));
    return -1;
  }
  if (! ssh_file_readaccess_ok(dir)) {
    if (ssh_mkdir(dir, 0700) < 0) {
      ssh_set_error(session, SSH_FATAL,
          "Cannot create %s directory.", dir);
      SAFE_FREE(dir);
      return -1;
    }
  }
  SAFE_FREE(dir);

  file = fopen(session->knownhosts, "a");
  if (file == NULL) {
    ssh_set_error(session, SSH_FATAL,
        "Couldn't open known_hosts file %s for appending: %s",
        session->knownhosts, strerror(errno));
    SAFE_FREE(host);
    return -1;
  }

  if (strcmp(session->current_crypto->server_pubkey_type, "ssh-rsa1") == 0) {
    /* openssh uses a different format for ssh-rsa1 keys.
       Be compatible --kv */
    ssh_public_key key;
    char *e_string = NULL;
    char *n_string = NULL;
    bignum e = NULL;
    bignum n = NULL;
    int rsa_size;
#ifdef HAVE_LIBGCRYPT
    gcry_sexp_t sexp;
#endif

    key = publickey_from_string(session, pubkey);
    if (key == NULL) {
      fclose(file);
      SAFE_FREE(host);
      return -1;
    }

#ifdef HAVE_LIBGCRYPT
    sexp = gcry_sexp_find_token(key->rsa_pub, "e", 0);
    if (sexp == NULL) {
      publickey_free(key);
      fclose(file);
      SAFE_FREE(host);
      return -1;
    }
    e = gcry_sexp_nth_mpi(sexp, 1, GCRYMPI_FMT_USG);
    gcry_sexp_release(sexp);
    if (e == NULL) {
      publickey_free(key);
      fclose(file);
      SAFE_FREE(host);
      return -1;
    }

    sexp = gcry_sexp_find_token(key->rsa_pub, "n", 0);
    if (sexp == NULL) {
      publickey_free(key);
      bignum_free(e);
      fclose(file);
      SAFE_FREE(host);
      return -1;
    }
    n = gcry_sexp_nth_mpi(sexp, 1, GCRYMPI_FMT_USG);
    gcry_sexp_release(sexp);
    if (n == NULL) {
      publickey_free(key);
      bignum_free(e);
      fclose(file);
      SAFE_FREE(host);
      return -1;
    }

    rsa_size = (gcry_pk_get_nbits(key->rsa_pub) + 7) / 8;
#elif defined HAVE_LIBCRYPTO
    e = key->rsa_pub->e;
    n = key->rsa_pub->n;
    rsa_size = RSA_size(key->rsa_pub);
#endif

    e_string = bignum_bn2dec(e);
    n_string = bignum_bn2dec(n);
    if (e_string == NULL || n_string == NULL) {
#ifdef HAVE_LIBGCRYPT
      bignum_free(e);
      bignum_free(n);
      SAFE_FREE(e_string);
      SAFE_FREE(n_string);
#elif defined HAVE_LIBCRYPTO
      OPENSSL_free(e_string);
      OPENSSL_free(n_string);
#endif
      publickey_free(key);
      fclose(file);
      SAFE_FREE(host);
      return -1;
    }

    snprintf(buffer, sizeof(buffer),
        "%s %d %s %s\n",
        host,
        rsa_size << 3,
        e_string,
        n_string);

#ifdef HAVE_LIBGCRYPT
    bignum_free(e);
    bignum_free(n);
    SAFE_FREE(e_string);
    SAFE_FREE(n_string);
#elif defined HAVE_LIBCRYPTO
    OPENSSL_free(e_string);
    OPENSSL_free(n_string);
#endif

    publickey_free(key);
  } else {
    pubkey_64 = bin_to_base64(pubkey->string, string_len(pubkey));
    if (pubkey_64 == NULL) {
      fclose(file);
      SAFE_FREE(host);
      return -1;
    }

    snprintf(buffer, sizeof(buffer),
        "%s %s %s\n",
        host,
        session->current_crypto->server_pubkey_type,
        pubkey_64);

    SAFE_FREE(pubkey_64);
  }
  SAFE_FREE(host);
  len = strlen(buffer);
  if (fwrite(buffer, len, 1, file) != 1 || ferror(file)) {
    fclose(file);
    return -1;
  }

  fclose(file);
  return 0;
}

/** @} */
/* vim: set ts=2 sw=2 et cindent: */
