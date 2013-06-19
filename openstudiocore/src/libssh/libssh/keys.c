/*
 * keys.c - decoding a public key or signature and verifying them
 *
 * This file is part of the SSH Library
 *
 * Copyright (c) 2003-2005 by Aris Adamantiadis
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

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_LIBCRYPTO
#include <openssl/dsa.h>
#include <openssl/rsa.h>
#endif
#include "libssh/priv.h"
#include "libssh/ssh2.h"
#include "libssh/server.h"
#include "libssh/buffer.h"
#include "libssh/agent.h"
#include "libssh/session.h"
#include "libssh/keys.h"
#include "libssh/dh.h"
#include "libssh/messages.h"
#include "libssh/string.h"

/** \addtogroup ssh_auth
 * @{
 */
/* Public key decoding functions */

const char *ssh_type_to_char(int type) {
  switch (type) {
    case TYPE_DSS:
      return "ssh-dss";
    case TYPE_RSA:
      return "ssh-rsa";
    case TYPE_RSA1:
      return "ssh-rsa1";
    default:
      return NULL;
  }
}

int ssh_type_from_name(const char *name) {
  if (strcmp(name, "rsa1") == 0) {
    return TYPE_RSA1;
  } else if (strcmp(name, "rsa") == 0) {
    return TYPE_RSA;
  } else if (strcmp(name, "dsa") == 0) {
    return TYPE_DSS;
  } else if (strcmp(name, "ssh-rsa1") == 0) {
    return TYPE_RSA1;
  } else if (strcmp(name, "ssh-rsa") == 0) {
    return TYPE_RSA;
  } else if (strcmp(name, "ssh-dss") == 0) {
    return TYPE_DSS;
  }

  return -1;
}

ssh_public_key publickey_make_dss(ssh_session session, ssh_buffer buffer) {
  ssh_string p = NULL;
  ssh_string q = NULL;
  ssh_string g = NULL;
  ssh_string pubkey = NULL;
  ssh_public_key key = NULL;

  key = malloc(sizeof(struct ssh_public_key_struct));
  if (key == NULL) {
    buffer_free(buffer);
    return NULL;
  }

  key->type = TYPE_DSS;
  key->type_c = ssh_type_to_char(key->type);

  p = buffer_get_ssh_string(buffer);
  q = buffer_get_ssh_string(buffer);
  g = buffer_get_ssh_string(buffer);
  pubkey = buffer_get_ssh_string(buffer);

  buffer_free(buffer); /* we don't need it anymore */

  if (p == NULL || q == NULL || g == NULL || pubkey == NULL) {
    ssh_set_error(session, SSH_FATAL, "Invalid DSA public key");
    goto error;
  }

#ifdef HAVE_LIBGCRYPT
  gcry_sexp_build(&key->dsa_pub, NULL,
      "(public-key(dsa(p %b)(q %b)(g %b)(y %b)))",
      string_len(p), string_data(p),
      string_len(q), string_data(q),
      string_len(g), string_data(g),
      string_len(pubkey), string_data(pubkey));
  if (key->dsa_pub == NULL) {
    goto error;
  }
#elif defined HAVE_LIBCRYPTO

  key->dsa_pub = DSA_new();
  if (key->dsa_pub == NULL) {
    goto error;
  }
  key->dsa_pub->p = make_string_bn(p);
  key->dsa_pub->q = make_string_bn(q);
  key->dsa_pub->g = make_string_bn(g);
  key->dsa_pub->pub_key = make_string_bn(pubkey);
  if (key->dsa_pub->p == NULL ||
      key->dsa_pub->q == NULL ||
      key->dsa_pub->g == NULL ||
      key->dsa_pub->pub_key == NULL) {
    goto error;
  }
#endif /* HAVE_LIBCRYPTO */

#ifdef DEBUG_CRYPTO
  ssh_print_hexa("p", string_data(p), string_len(p));
  ssh_print_hexa("q", string_data(q), string_len(q));
  ssh_print_hexa("g", string_data(g), string_len(g));
#endif

  string_burn(p);
  string_free(p);
  string_burn(q);
  string_free(q);
  string_burn(g);
  string_free(g);
  string_burn(pubkey);
  string_free(pubkey);

  return key;
error:
  string_burn(p);
  string_free(p);
  string_burn(q);
  string_free(q);
  string_burn(g);
  string_free(g);
  string_burn(pubkey);
  string_free(pubkey);
  publickey_free(key);

  return NULL;
}

ssh_public_key publickey_make_rsa(ssh_session session, ssh_buffer buffer,
    int type) {
  ssh_string e = NULL;
  ssh_string n = NULL;
  ssh_public_key key = NULL;

  key = malloc(sizeof(struct ssh_public_key_struct));
  if (key == NULL) {
    buffer_free(buffer);
    return NULL;
  }

  key->type = type;
  key->type_c = ssh_type_to_char(key->type);

  e = buffer_get_ssh_string(buffer);
  n = buffer_get_ssh_string(buffer);

  buffer_free(buffer); /* we don't need it anymore */

  if(e == NULL || n == NULL) {
    ssh_set_error(session, SSH_FATAL, "Invalid RSA public key");
    goto error;
  }
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_build(&key->rsa_pub, NULL,
      "(public-key(rsa(n %b)(e %b)))",
      string_len(n), string_data(n),
      string_len(e),string_data(e));
  if (key->rsa_pub == NULL) {
    goto error;
  }
#elif HAVE_LIBCRYPTO
  key->rsa_pub = RSA_new();
  if (key->rsa_pub == NULL) {
    goto error;
  }

  key->rsa_pub->e = make_string_bn(e);
  key->rsa_pub->n = make_string_bn(n);
  if (key->rsa_pub->e == NULL ||
      key->rsa_pub->n == NULL) {
    goto error;
  }
#endif

#ifdef DEBUG_CRYPTO
  ssh_print_hexa("e", string_data(e), string_len(e));
  ssh_print_hexa("n", string_data(n), string_len(n));
#endif

  string_burn(e);
  string_free(e);
  string_burn(n);
  string_free(n);

  return key;
error:
  string_burn(e);
  string_free(e);
  string_burn(n);
  string_free(n);
  publickey_free(key);

  return NULL;
}

void publickey_free(ssh_public_key key) {
  if (key == NULL) {
    return;
  }

  switch(key->type) {
    case TYPE_DSS:
#ifdef HAVE_LIBGCRYPT
      gcry_sexp_release(key->dsa_pub);
#elif HAVE_LIBCRYPTO
      DSA_free(key->dsa_pub);
#endif
      break;
    case TYPE_RSA:
    case TYPE_RSA1:
#ifdef HAVE_LIBGCRYPT
      gcry_sexp_release(key->rsa_pub);
#elif defined HAVE_LIBCRYPTO
      RSA_free(key->rsa_pub);
#endif
      break;
    default:
      break;
  }
  SAFE_FREE(key);
}

ssh_public_key publickey_from_string(ssh_session session, ssh_string pubkey_s) {
  ssh_buffer tmpbuf = NULL;
  ssh_string type_s = NULL;
  char *type_c = NULL;
  int type;

  tmpbuf = buffer_new();
  if (tmpbuf == NULL) {
    return NULL;
  }

  if (buffer_add_data(tmpbuf, string_data(pubkey_s), string_len(pubkey_s)) < 0) {
    goto error;
  }

  type_s = buffer_get_ssh_string(tmpbuf);
  if (type_s == NULL) {
    ssh_set_error(session,SSH_FATAL,"Invalid public key format");
    goto error;
  }

  type_c = string_to_char(type_s);
  string_free(type_s);
  if (type_c == NULL) {
    goto error;
  }

  type = ssh_type_from_name(type_c);
  SAFE_FREE(type_c);

  switch (type) {
    case TYPE_DSS:
      return publickey_make_dss(session, tmpbuf);
    case TYPE_RSA:
    case TYPE_RSA1:
      return publickey_make_rsa(session, tmpbuf, type);
  }

  ssh_set_error(session, SSH_FATAL, "Unknown public key protocol %s",
      ssh_type_to_char(type));

error:
  buffer_free(tmpbuf);
  return NULL;
}

/** \brief Makes a PUBLIC_KEY object out of a PRIVATE_KEY object
 * \param prv the Private key
 * \returns the public key
 * \see publickey_to_string()
 */
ssh_public_key publickey_from_privatekey(ssh_private_key prv) {
  ssh_public_key key = NULL;
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_t sexp;
  const char *tmp = NULL;
  size_t size;
  ssh_string p = NULL;
  ssh_string q = NULL;
  ssh_string g = NULL;
  ssh_string y = NULL;
  ssh_string e = NULL;
  ssh_string n = NULL;
#endif /* HAVE_LIBGCRYPT */

  key = malloc(sizeof(struct ssh_public_key_struct));
  if (key == NULL) {
    return NULL;
  }

  key->type = prv->type;
  switch(key->type) {
    case TYPE_DSS:
#ifdef HAVE_LIBGCRYPT
      sexp = gcry_sexp_find_token(prv->dsa_priv, "p", 0);
      if (sexp == NULL) {
        goto error;
      }
      tmp = gcry_sexp_nth_data(sexp, 1, &size);
      p = string_new(size);
      if (p == NULL) {
        goto error;
      }
      string_fill(p,(char *) tmp, size);
      gcry_sexp_release(sexp);

      sexp = gcry_sexp_find_token(prv->dsa_priv,"q",0);
      if (sexp == NULL) {
        goto error;
      }
      tmp = gcry_sexp_nth_data(sexp,1,&size);
      q = string_new(size);
      if (q == NULL) {
        goto error;
      }
      string_fill(q,(char *) tmp,size);
      gcry_sexp_release(sexp);

      sexp = gcry_sexp_find_token(prv->dsa_priv, "g", 0);
      if (sexp == NULL) {
        goto error;
      }
      tmp = gcry_sexp_nth_data(sexp,1,&size);
      g = string_new(size);
      if (g == NULL) {
        goto error;
      }
      string_fill(g,(char *) tmp,size);
      gcry_sexp_release(sexp);

      sexp = gcry_sexp_find_token(prv->dsa_priv,"y",0);
      if (sexp == NULL) {
        goto error;
      }
      tmp = gcry_sexp_nth_data(sexp,1,&size);
      y = string_new(size);
      if (y == NULL) {
        goto error;
      }
      string_fill(y,(char *) tmp,size);
      gcry_sexp_release(sexp);

      gcry_sexp_build(&key->dsa_pub, NULL,
          "(public-key(dsa(p %b)(q %b)(g %b)(y %b)))",
          string_len(p), string_data(p),
          string_len(q), string_data(q),
          string_len(g), string_data(g),
          string_len(y), string_data(y));

      string_burn(p);
      string_free(p);
      string_burn(q);
      string_free(q);
      string_burn(g);
      string_free(g);
      string_burn(y);
      string_free(y);
#elif defined HAVE_LIBCRYPTO
      key->dsa_pub = DSA_new();
      if (key->dsa_pub == NULL) {
        goto error;
      }
      key->dsa_pub->p = BN_dup(prv->dsa_priv->p);
      key->dsa_pub->q = BN_dup(prv->dsa_priv->q);
      key->dsa_pub->g = BN_dup(prv->dsa_priv->g);
      key->dsa_pub->pub_key = BN_dup(prv->dsa_priv->pub_key);
      if (key->dsa_pub->p == NULL ||
          key->dsa_pub->q == NULL ||
          key->dsa_pub->g == NULL ||
          key->dsa_pub->pub_key == NULL) {
        goto error;
      }
#endif /* HAVE_LIBCRYPTO */
      break;
    case TYPE_RSA:
    case TYPE_RSA1:
#ifdef HAVE_LIBGCRYPT
      sexp = gcry_sexp_find_token(prv->rsa_priv, "n", 0);
      if (sexp == NULL) {
        goto error;
      }
      tmp = gcry_sexp_nth_data(sexp, 1, &size);
      n = string_new(size);
      if (n == NULL) {
        goto error;
      }
      string_fill(n, (char *) tmp, size);
      gcry_sexp_release(sexp);

      sexp = gcry_sexp_find_token(prv->rsa_priv, "e", 0);
      if (sexp == NULL) {
        goto error;
      }
      tmp = gcry_sexp_nth_data(sexp, 1, &size);
      e = string_new(size);
      if (e == NULL) {
        goto error;
      }
      string_fill(e, (char *) tmp, size);
      gcry_sexp_release(sexp);

      gcry_sexp_build(&key->rsa_pub, NULL,
          "(public-key(rsa(n %b)(e %b)))",
          string_len(n), string_data(n),
          string_len(e), string_data(e));
      if (key->rsa_pub == NULL) {
        goto error;
      }

      string_burn(e);
      string_free(e);
      string_burn(n);
      string_free(n);
#elif defined HAVE_LIBCRYPTO
      key->rsa_pub = RSA_new();
      if (key->rsa_pub == NULL) {
        goto error;
      }
      key->rsa_pub->e = BN_dup(prv->rsa_priv->e);
      key->rsa_pub->n = BN_dup(prv->rsa_priv->n);
      if (key->rsa_pub->e == NULL ||
          key->rsa_pub->n == NULL) {
        goto error;
      }
#endif
      break;
  }
  key->type_c = ssh_type_to_char(prv->type);

  return key;
error:
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_release(sexp);
  string_burn(p);
  string_free(p);
  string_burn(q);
  string_free(q);
  string_burn(g);
  string_free(g);
  string_burn(y);
  string_free(y);

  string_burn(e);
  string_free(e);
  string_burn(n);
  string_free(n);
#endif
  publickey_free(key);

  return NULL;
}

#ifdef HAVE_LIBGCRYPT
static int dsa_public_to_string(gcry_sexp_t key, ssh_buffer buffer) {
#elif defined HAVE_LIBCRYPTO
static int dsa_public_to_string(DSA *key, ssh_buffer buffer) {
#endif
  ssh_string p = NULL;
  ssh_string q = NULL;
  ssh_string g = NULL;
  ssh_string n = NULL;

  int rc = -1;

#ifdef HAVE_LIBGCRYPT
  const char *tmp = NULL;
  size_t size;
  gcry_sexp_t sexp;

  sexp = gcry_sexp_find_token(key, "p", 0);
  if (sexp == NULL) {
    goto error;
  }
  tmp = gcry_sexp_nth_data(sexp, 1, &size);
  p = string_new(size);
  if (p == NULL) {
    goto error;
  }
  string_fill(p, (char *) tmp, size);
  gcry_sexp_release(sexp);

  sexp = gcry_sexp_find_token(key, "q", 0);
  if (sexp == NULL) {
    goto error;
  }
  tmp = gcry_sexp_nth_data(sexp, 1, &size);
  q = string_new(size);
  if (q == NULL) {
    goto error;
  }
  string_fill(q, (char *) tmp, size);
  gcry_sexp_release(sexp);

  sexp = gcry_sexp_find_token(key, "g", 0);
  if (sexp == NULL) {
    goto error;
  }
  tmp = gcry_sexp_nth_data(sexp, 1, &size);
  g = string_new(size);
  if (g == NULL) {
    goto error;
  }
  string_fill(g, (char *) tmp, size);
  gcry_sexp_release(sexp);

  sexp = gcry_sexp_find_token(key, "y", 0);
  if (sexp == NULL) {
    goto error;
  }
  tmp = gcry_sexp_nth_data(sexp, 1, &size);
  n = string_new(size);
  if (n == NULL) {
    goto error;
  }
  string_fill(n, (char *) tmp, size);

#elif defined HAVE_LIBCRYPTO
  p = make_bignum_string(key->p);
  q = make_bignum_string(key->q);
  g = make_bignum_string(key->g);
  n = make_bignum_string(key->pub_key);
  if (p == NULL || q == NULL || g == NULL || n == NULL) {
    goto error;
  }
#endif /* HAVE_LIBCRYPTO */
  if (buffer_add_ssh_string(buffer, p) < 0) {
    goto error;
  }
  if (buffer_add_ssh_string(buffer, q) < 0) {
    goto error;
  }
  if (buffer_add_ssh_string(buffer, g) < 0) {
    goto error;
  }
  if (buffer_add_ssh_string(buffer, n) < 0) {
    goto error;
  }

  rc = 0;
error:
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_release(sexp);
#endif

  string_burn(p);
  string_free(p);
  string_burn(q);
  string_free(q);
  string_burn(g);
  string_free(g);
  string_burn(n);
  string_free(n);

  return rc;
}

#ifdef HAVE_LIBGCRYPT
static int rsa_public_to_string(gcry_sexp_t key, ssh_buffer buffer) {
#elif defined HAVE_LIBCRYPTO
static int rsa_public_to_string(RSA *key, ssh_buffer buffer) {
#endif

  ssh_string e = NULL;
  ssh_string n = NULL;

  int rc = -1;

#ifdef HAVE_LIBGCRYPT
  const char *tmp;
  size_t size;
  gcry_sexp_t sexp;

  sexp = gcry_sexp_find_token(key, "n", 0);
  if (sexp == NULL) {
    goto error;
  }
  tmp = gcry_sexp_nth_data(sexp, 1, &size);
  n = string_new(size);
  if (n == NULL) {
    goto error;
  }
  string_fill(n, (char *) tmp, size);
  gcry_sexp_release(sexp);

  sexp = gcry_sexp_find_token(key, "e", 0);
  if (sexp == NULL) {
    goto error;
  }
  tmp = gcry_sexp_nth_data(sexp, 1, &size);
  e = string_new(size);
  if (e == NULL) {
    goto error;
  }
  string_fill(e, (char *) tmp, size);

#elif defined HAVE_LIBCRYPTO
  e = make_bignum_string(key->e);
  n = make_bignum_string(key->n);
  if (e == NULL || n == NULL) {
    goto error;
  }
#endif

  if (buffer_add_ssh_string(buffer, e) < 0) {
    goto error;
  }
  if (buffer_add_ssh_string(buffer, n) < 0) {
    goto error;
  }

  rc = 0;
error:
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_release(sexp);
#endif

  string_burn(e);
  string_free(e);
  string_burn(n);
  string_free(n);

  return rc;
}

/** \brief makes a SSH String out of a PUBLIC_KEY object
 * \param key the public key
 * \returns a SSH String containing the public key
 * \see string_free()
 */
ssh_string publickey_to_string(ssh_public_key key) {
  ssh_string type = NULL;
  ssh_string ret = NULL;
  ssh_buffer buf = NULL;

  buf = buffer_new();
  if (buf == NULL) {
    return NULL;
  }

  type = string_from_char(key->type_c);
  if (type == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(buf, type) < 0) {
    goto error;
  }

  switch (key->type) {
    case TYPE_DSS:
      if (dsa_public_to_string(key->dsa_pub, buf) < 0) {
        goto error;
      }
      break;
    case TYPE_RSA:
    case TYPE_RSA1:
      if (rsa_public_to_string(key->rsa_pub, buf) < 0) {
        goto error;
      }
      break;
  }

  ret = string_new(buffer_get_len(buf));
  if (ret == NULL) {
    goto error;
  }

  string_fill(ret, buffer_get(buf), buffer_get_len(buf));
error:
  buffer_free(buf);
  string_free(type);

  return ret;
}

/* Signature decoding functions */
static ssh_string signature_to_string(SIGNATURE *sign) {
  unsigned char buffer[40] = {0};
  ssh_buffer tmpbuf = NULL;
  ssh_string str = NULL;
  ssh_string tmp = NULL;
  ssh_string rs = NULL;
  int rc = -1;
#ifdef HAVE_LIBGCRYPT
  const char *r = NULL;
  const char *s = NULL;
  gcry_sexp_t sexp;
  size_t size = 0;
#elif defined HAVE_LIBCRYPTO
  ssh_string r = NULL;
  ssh_string s = NULL;
#endif

  tmpbuf = buffer_new();
  if (tmpbuf == NULL) {
    return NULL;
  }

  tmp = string_from_char(ssh_type_to_char(sign->type));
  if (tmp == NULL) {
    buffer_free(tmpbuf);
    return NULL;
  }
  if (buffer_add_ssh_string(tmpbuf, tmp) < 0) {
    buffer_free(tmpbuf);
    string_free(tmp);
    return NULL;
  }
  string_free(tmp);

  switch(sign->type) {
    case TYPE_DSS:
#ifdef HAVE_LIBGCRYPT
      sexp = gcry_sexp_find_token(sign->dsa_sign, "r", 0);
      if (sexp == NULL) {
        buffer_free(tmpbuf);
        return NULL;
      }
      r = gcry_sexp_nth_data(sexp, 1, &size);
      if (*r == 0) {      /* libgcrypt put 0 when first bit is set */
        size--;
        r++;
      }
      memcpy(buffer, r + size - 20, 20);
      gcry_sexp_release(sexp);

      sexp = gcry_sexp_find_token(sign->dsa_sign, "s", 0);
      if (sexp == NULL) {
        buffer_free(tmpbuf);
        return NULL;
      }
      s = gcry_sexp_nth_data(sexp,1,&size);
      if (*s == 0) {
        size--;
        s++;
      }
      memcpy(buffer+ 20, s + size - 20, 20);
      gcry_sexp_release(sexp);
#elif defined HAVE_LIBCRYPTO
      r = make_bignum_string(sign->dsa_sign->r);
      if (r == NULL) {
        buffer_free(tmpbuf);
        return NULL;
      }
      s = make_bignum_string(sign->dsa_sign->s);
      if (s == NULL) {
        buffer_free(tmpbuf);
        string_free(r);
        return NULL;
      }

      memcpy(buffer, (char *)string_data(r) + string_len(r) - 20, 20);
      memcpy(buffer + 20, (char *)string_data(s) + string_len(s) - 20, 20);

      string_free(r);
      string_free(s);
#endif /* HAVE_LIBCRYPTO */
      rs = string_new(40);
      if (rs == NULL) {
        buffer_free(tmpbuf);
        return NULL;
      }

      string_fill(rs, buffer, 40);
      rc = buffer_add_ssh_string(tmpbuf, rs);
      string_free(rs);
      if (rc < 0) {
        buffer_free(tmpbuf);
        return NULL;
      }

      break;
    case TYPE_RSA:
    case TYPE_RSA1:
#ifdef HAVE_LIBGCRYPT
      sexp = gcry_sexp_find_token(sign->rsa_sign, "s", 0);
      if (sexp == NULL) {
        buffer_free(tmpbuf);
        return NULL;
      }
      s = gcry_sexp_nth_data(sexp,1,&size);
      if (*s == 0) {
        size--;
        s++;
      }
      rs = string_new(size);
      if (rs == NULL) {
        buffer_free(tmpbuf);
        return NULL;
      }

      string_fill(rs, (char *) s, size);
      rc = buffer_add_ssh_string(tmpbuf, rs);
      gcry_sexp_release(sexp);
      string_free(rs);
      if (rc < 0) {
        buffer_free(tmpbuf);
        return NULL;
      }
#elif defined HAVE_LIBCRYPTO
      if (buffer_add_ssh_string(tmpbuf,sign->rsa_sign) < 0) {
        buffer_free(tmpbuf);
        return NULL;
      }
#endif
      break;
  }

  str = string_new(buffer_get_len(tmpbuf));
  if (str == NULL) {
    buffer_free(tmpbuf);
    return NULL;
  }
  string_fill(str, buffer_get(tmpbuf), buffer_get_len(tmpbuf));
  buffer_free(tmpbuf);

  return str;
}

/* TODO : split this function in two so it becomes smaller */
SIGNATURE *signature_from_string(ssh_session session, ssh_string signature,
    ssh_public_key pubkey, int needed_type) {
  SIGNATURE *sign = NULL;
  ssh_buffer tmpbuf = NULL;
  ssh_string rs = NULL;
  ssh_string type_s = NULL;
  ssh_string e = NULL;
  char *type_c = NULL;
  int type;
  int len;
  int rsalen;
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_t sig;
#elif defined HAVE_LIBCRYPTO
  DSA_SIG *sig = NULL;
  ssh_string r = NULL;
  ssh_string s = NULL;
#endif

  sign = malloc(sizeof(SIGNATURE));
  if (sign == NULL) {
    ssh_set_error(session, SSH_FATAL, "Not enough space");
    return NULL;
  }

  tmpbuf = buffer_new();
  if (tmpbuf == NULL) {
    ssh_set_error(session, SSH_FATAL, "Not enough space");
    signature_free(sign);
    return NULL;
  }

  if (buffer_add_data(tmpbuf, string_data(signature), string_len(signature)) < 0) {
    signature_free(sign);
    buffer_free(tmpbuf);
    return NULL;
  }

  type_s = buffer_get_ssh_string(tmpbuf);
  if (type_s == NULL) {
    ssh_set_error(session, SSH_FATAL, "Invalid signature packet");
    signature_free(sign);
    buffer_free(tmpbuf);
    return NULL;
  }

  type_c = string_to_char(type_s);
  string_free(type_s);
  if (type_c == NULL) {
    signature_free(sign);
    buffer_free(tmpbuf);
    return NULL;
  }
  type = ssh_type_from_name(type_c);
  SAFE_FREE(type_c);

  if (needed_type != type) {
    ssh_set_error(session, SSH_FATAL, "Invalid signature type: %s",
        ssh_type_to_char(type));
    signature_free(sign);
    buffer_free(tmpbuf);
    return NULL;
  }

  switch(needed_type) {
    case TYPE_DSS:
      rs = buffer_get_ssh_string(tmpbuf);
      buffer_free(tmpbuf);

      /* 40 is the dual signature blob len. */
      if (rs == NULL || string_len(rs) != 40) {
        string_free(rs);
        signature_free(sign);
        return NULL;
      }

      /* we make use of strings (because we have all-made functions to convert
       * them to bignums (ou pas ;) */
#ifdef HAVE_LIBGCRYPT
      if (gcry_sexp_build(&sig, NULL, "(sig-val(dsa(r %b)(s %b)))",
            20 ,string_data(rs), 20,(unsigned char *)string_data(rs) + 20)) {
        string_free(rs);
        signature_free(sign);
        return NULL;
      }
#elif defined HAVE_LIBCRYPTO
      r = string_new(20);
      s = string_new(20);
      if (r == NULL || s == NULL) {
        string_free(r);
        string_free(s);
        string_free(rs);
        signature_free(sign);
        return NULL;
      }

      string_fill(r, string_data(rs), 20);
      string_fill(s, (char *)string_data(rs) + 20, 20);

      sig = DSA_SIG_new();
      if (sig == NULL) {
        string_free(r);
        string_free(s);
        string_free(rs);
        signature_free(sign);
        return NULL;
      }
      sig->r = make_string_bn(r); /* is that really portable ? Openssh's hack isn't better */
      sig->s = make_string_bn(s);
      string_free(r);
      string_free(s);

      if (sig->r == NULL || sig->s == NULL) {
        string_free(rs);
        DSA_SIG_free(sig);
        signature_free(sign);
        return NULL;
      }
#endif

#ifdef DEBUG_CRYPTO
      ssh_print_hexa("r", string_data(rs), 20);
      ssh_print_hexa("s", (const unsigned char *)string_data(rs) + 20, 20);
#endif
      string_free(rs);

      sign->type = TYPE_DSS;
      sign->dsa_sign = sig;

      return sign;
    case TYPE_RSA:
      e = buffer_get_ssh_string(tmpbuf);
      buffer_free(tmpbuf);
      if (e == NULL) {
        signature_free(sign);
        return NULL;
      }
      len = string_len(e);
#ifdef HAVE_LIBGCRYPT
      rsalen = (gcry_pk_get_nbits(pubkey->rsa_pub) + 7) / 8;
#elif defined HAVE_LIBCRYPTO
      rsalen = RSA_size(pubkey->rsa_pub);
#endif
      if (len > rsalen) {
        string_free(e);
        signature_free(sign);
        ssh_set_error(session, SSH_FATAL, "Signature too big! %d instead of %d",
            len, rsalen);
        return NULL;
      }

      if (len < rsalen) {
        ssh_log(session, SSH_LOG_RARE, "RSA signature len %d < %d",
            len, rsalen);
      }
      sign->type = TYPE_RSA;
#ifdef HAVE_LIBGCRYPT
      if (gcry_sexp_build(&sig, NULL, "(sig-val(rsa(s %b)))",
          string_len(e), string_data(e))) {
        signature_free(sign);
        string_free(e);
        return NULL;
      }

      sign->rsa_sign = sig;
#elif defined HAVE_LIBCRYPTO
      sign->rsa_sign = e;
#endif

#ifdef DEBUG_CRYPTO
      ssh_log(session, SSH_LOG_FUNCTIONS, "len e: %d", len);
      ssh_print_hexa("RSA signature", string_data(e), len);
#endif

#ifdef HAVE_LIBGCRYPT
      string_free(e);
#endif

      return sign;
    default:
      return NULL;
  }

  return NULL;
}

void signature_free(SIGNATURE *sign) {
  if (sign == NULL) {
    return;
  }

  switch(sign->type) {
    case TYPE_DSS:
#ifdef HAVE_LIBGCRYPT
      gcry_sexp_release(sign->dsa_sign);
#elif defined HAVE_LIBCRYPTO
      DSA_SIG_free(sign->dsa_sign);
#endif
      break;
    case TYPE_RSA:
    case TYPE_RSA1:
#ifdef HAVE_LIBGCRYPT
      gcry_sexp_release(sign->rsa_sign);
#elif defined HAVE_LIBCRYPTO
      SAFE_FREE(sign->rsa_sign);
#endif
      break;
    default:
      /* FIXME Passing NULL segfaults */
#if 0
       ssh_log(NULL, SSH_LOG_RARE, "Freeing a signature with no type!\n"); */
#endif
         break;
    }
  SAFE_FREE(sign);
}

#ifdef HAVE_LIBCRYPTO
/*
 * Maybe the missing function from libcrypto
 *
 * I think now, maybe it's a bad idea to name it has it should have be
 * named in libcrypto
 */
static ssh_string RSA_do_sign(const unsigned char *payload, int len, RSA *privkey) {
  ssh_string sign = NULL;
  unsigned char *buffer = NULL;
  unsigned int size;

  buffer = malloc(RSA_size(privkey));
  if (buffer == NULL) {
    return NULL;
  }

  if (RSA_sign(NID_sha1, payload, len, buffer, &size, privkey) == 0) {
    SAFE_FREE(buffer);
    return NULL;
  }

  sign = string_new(size);
  if (sign == NULL) {
    SAFE_FREE(buffer);
    return NULL;
  }

  string_fill(sign, buffer, size);
  SAFE_FREE(buffer);

  return sign;
}
#endif

#ifndef _WIN32
ssh_string ssh_do_sign_with_agent(ssh_session session,
    struct ssh_buffer_struct *buf, struct ssh_public_key_struct *publickey) {
  struct ssh_buffer_struct *sigbuf = NULL;
  struct ssh_string_struct *signature = NULL;
  struct ssh_string_struct *session_id = NULL;
  struct ssh_crypto_struct *crypto = NULL;

  if (session->current_crypto) {
    crypto = session->current_crypto;
  } else {
    crypto = session->next_crypto;
  }

  /* prepend session identifier */
  session_id = string_new(SHA_DIGEST_LEN);
  if (session_id == NULL) {
    return NULL;
  }
  string_fill(session_id, crypto->session_id, SHA_DIGEST_LEN);

  sigbuf = buffer_new();
  if (sigbuf == NULL) {
    string_free(session_id);
    return NULL;
  }

  if (buffer_add_ssh_string(sigbuf, session_id) < 0) {
    buffer_free(sigbuf);
    string_free(session_id);
    return NULL;
  }
  string_free(session_id);

  /* append out buffer */
  if (buffer_add_buffer(sigbuf, buf) < 0) {
    buffer_free(sigbuf);
    return NULL;
  }

  /* create signature */
  signature = agent_sign_data(session, sigbuf, publickey);

  buffer_free(sigbuf);

  return signature;
}
#endif /* _WIN32 */

/*
 * This function concats in a buffer the values needed to do a signature
 * verification. */
ssh_buffer ssh_userauth_build_digest(ssh_session session, ssh_message msg, char *service) {
/*
     The value of 'signature' is a signature by the corresponding private
   key over the following data, in the following order:

      string    session identifier
      byte      SSH_MSG_USERAUTH_REQUEST
      string    user name
      string    service name
      string    "publickey"
      boolean   TRUE
      string    public key algorithm name
      string    public key to be used for authentication
*/
	struct ssh_crypto_struct *crypto = session->current_crypto ? session->current_crypto :
                                             session->next_crypto;
  ssh_buffer buffer = NULL;
  ssh_string session_id = NULL;
  uint8_t type = SSH2_MSG_USERAUTH_REQUEST;
  ssh_string username = string_from_char(msg->auth_request.username);
  ssh_string servicename = string_from_char(service);
  ssh_string method = string_from_char("publickey");
  uint8_t has_sign = 1;
  ssh_string algo = string_from_char(msg->auth_request.public_key->type_c);
  ssh_string publickey = publickey_to_string(msg->auth_request.public_key);

  buffer = buffer_new();
  if (buffer == NULL) {
    goto error;
  }
  session_id = string_new(SHA_DIGEST_LEN);
  if (session_id == NULL) {
    buffer_free(buffer);
    buffer = NULL;
    goto error;
  }
  string_fill(session_id, crypto->session_id, SHA_DIGEST_LEN);

  if(buffer_add_ssh_string(buffer, session_id) < 0 ||
     buffer_add_u8(buffer, type) < 0 ||
     buffer_add_ssh_string(buffer, username) < 0 ||
     buffer_add_ssh_string(buffer, servicename) < 0 ||
     buffer_add_ssh_string(buffer, method) < 0 ||
     buffer_add_u8(buffer, has_sign) < 0 ||
     buffer_add_ssh_string(buffer, algo) < 0 ||
     buffer_add_ssh_string(buffer, publickey) < 0) {
    buffer_free(buffer);
    buffer = NULL;
    goto error;
  }

error:
  if(session_id) string_free(session_id);
  if(username) string_free(username);
  if(servicename) string_free(servicename);
  if(method) string_free(method);
  if(algo) string_free(algo);
  if(publickey) string_free(publickey);
  return buffer;
}

/*
 * This function signs the session id (known as H) as a string then
 * the content of sigbuf */
ssh_string ssh_do_sign(ssh_session session, ssh_buffer sigbuf,
    ssh_private_key privatekey) {
  struct ssh_crypto_struct *crypto = session->current_crypto ? session->current_crypto :
    session->next_crypto;
  unsigned char hash[SHA_DIGEST_LEN + 1] = {0};
  ssh_string session_str = NULL;
  ssh_string signature = NULL;
  SIGNATURE *sign = NULL;
  SHACTX ctx = NULL;
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_t gcryhash;
#endif

  session_str = string_new(SHA_DIGEST_LEN);
  if (session_str == NULL) {
    return NULL;
  }
  string_fill(session_str, crypto->session_id, SHA_DIGEST_LEN);

  ctx = sha1_init();
  if (ctx == NULL) {
    string_free(session_str);
    return NULL;
  }

  sha1_update(ctx, session_str, string_len(session_str) + 4);
  string_free(session_str);
  sha1_update(ctx, buffer_get(sigbuf), buffer_get_len(sigbuf));
  sha1_final(hash + 1,ctx);
  hash[0] = 0;

#ifdef DEBUG_CRYPTO
  ssh_print_hexa("Hash being signed with dsa", hash + 1, SHA_DIGEST_LEN);
#endif

  sign = malloc(sizeof(SIGNATURE));
  if (sign == NULL) {
    return NULL;
  }

  switch(privatekey->type) {
    case TYPE_DSS:
#ifdef HAVE_LIBGCRYPT
      if (gcry_sexp_build(&gcryhash, NULL, "%b", SHA_DIGEST_LEN + 1, hash) ||
          gcry_pk_sign(&sign->dsa_sign, gcryhash, privatekey->dsa_priv)) {
        ssh_set_error(session, SSH_FATAL, "Signing: libcrypt error");
        gcry_sexp_release(gcryhash);
        signature_free(sign);
        return NULL;
      }
#elif defined HAVE_LIBCRYPTO
      sign->dsa_sign = DSA_do_sign(hash + 1, SHA_DIGEST_LEN,
          privatekey->dsa_priv);
      if (sign->dsa_sign == NULL) {
        ssh_set_error(session, SSH_FATAL, "Signing: openssl error");
        signature_free(sign);
        return NULL;
      }
#ifdef DEBUG_CRYPTO
      ssh_print_bignum("r", sign->dsa_sign->r);
      ssh_print_bignum("s", sign->dsa_sign->s);
#endif
#endif /* HAVE_LIBCRYPTO */
      sign->rsa_sign = NULL;
      break;
    case TYPE_RSA:
#ifdef HAVE_LIBGCRYPT
      if (gcry_sexp_build(&gcryhash, NULL, "(data(flags pkcs1)(hash sha1 %b))",
            SHA_DIGEST_LEN, hash + 1) ||
          gcry_pk_sign(&sign->rsa_sign, gcryhash, privatekey->rsa_priv)) {
        ssh_set_error(session, SSH_FATAL, "Signing: libcrypt error");
        gcry_sexp_release(gcryhash);
        signature_free(sign);
        return NULL;
      }
#elif defined HAVE_LIBCRYPTO
      sign->rsa_sign = RSA_do_sign(hash + 1, SHA_DIGEST_LEN,
          privatekey->rsa_priv);
      if (sign->rsa_sign == NULL) {
        ssh_set_error(session, SSH_FATAL, "Signing: openssl error");
        signature_free(sign);
        return NULL;
      }
#endif
      sign->dsa_sign = NULL;
      break;
  }
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_release(gcryhash);
#endif

  sign->type = privatekey->type;

  signature = signature_to_string(sign);
  signature_free(sign);

  return signature;
}

ssh_string ssh_encrypt_rsa1(ssh_session session, ssh_string data, ssh_public_key key) {
  ssh_string str = NULL;
  size_t len = string_len(data);
  size_t size = 0;
#ifdef HAVE_LIBGCRYPT
  const char *tmp = NULL;
  gcry_sexp_t ret_sexp;
  gcry_sexp_t data_sexp;

  if (gcry_sexp_build(&data_sexp, NULL, "(data(flags pkcs1)(value %b))",
      len, string_data(data))) {
    ssh_set_error(session, SSH_FATAL, "RSA1 encrypt: libgcrypt error");
    return NULL;
  }
  if (gcry_pk_encrypt(&ret_sexp, data_sexp, key->rsa_pub)) {
    gcry_sexp_release(data_sexp);
    ssh_set_error(session, SSH_FATAL, "RSA1 encrypt: libgcrypt error");
    return NULL;
  }

  gcry_sexp_release(data_sexp);

  data_sexp = gcry_sexp_find_token(ret_sexp, "a", 0);
  if (data_sexp == NULL) {
    ssh_set_error(session, SSH_FATAL, "RSA1 encrypt: libgcrypt error");
    gcry_sexp_release(ret_sexp);
    return NULL;
  }
  tmp = gcry_sexp_nth_data(data_sexp, 1, &size);
  if (*tmp == 0) {
    size--;
    tmp++;
  }

  str = string_new(size);
  if (str == NULL) {
    ssh_set_error(session, SSH_FATAL, "Not enough space");
    gcry_sexp_release(data_sexp);
    gcry_sexp_release(ret_sexp);
    return NULL;
  }
  string_fill(str, tmp, size);

  gcry_sexp_release(data_sexp);
  gcry_sexp_release(ret_sexp);
#elif defined HAVE_LIBCRYPTO
  size = RSA_size(key->rsa_pub);

  str = string_new(size);
  if (str == NULL) {
    ssh_set_error(session, SSH_FATAL, "Not enough space");
    return NULL;
  }

  if (RSA_public_encrypt(len, string_data(data), string_data(str), key->rsa_pub,
      RSA_PKCS1_PADDING) < 0) {
    string_free(str);
    return NULL;
  }
#endif

  return str;
}


/* this function signs the session id */
ssh_string ssh_sign_session_id(ssh_session session, ssh_private_key privatekey) {
	struct ssh_crypto_struct *crypto=session->current_crypto ? session->current_crypto :
    session->next_crypto;
  unsigned char hash[SHA_DIGEST_LEN + 1] = {0};
  ssh_string signature = NULL;
  SIGNATURE *sign = NULL;
  SHACTX ctx = NULL;
#ifdef HAVE_LIBGCRYPT
  gcry_sexp_t data_sexp;
#endif

  ctx = sha1_init();
  if (ctx == NULL) {
    return NULL;
  }
  sha1_update(ctx,crypto->session_id,SHA_DIGEST_LEN);
  sha1_final(hash + 1,ctx);
  hash[0] = 0;

#ifdef DEBUG_CRYPTO
  ssh_print_hexa("Hash being signed with dsa",hash+1,SHA_DIGEST_LEN);
#endif

  sign = malloc(sizeof(SIGNATURE));
  if (sign == NULL) {
    return NULL;
  }

  switch(privatekey->type) {
    case TYPE_DSS:
#ifdef HAVE_LIBGCRYPT
      if (gcry_sexp_build(&data_sexp, NULL, "%b", SHA_DIGEST_LEN + 1, hash) ||
          gcry_pk_sign(&sign->dsa_sign, data_sexp, privatekey->dsa_priv)) {
        ssh_set_error(session, SSH_FATAL, "Signing: libgcrypt error");
        gcry_sexp_release(data_sexp);
        signature_free(sign);
        return NULL;
      }
#elif defined HAVE_LIBCRYPTO
      sign->dsa_sign = DSA_do_sign(hash + 1, SHA_DIGEST_LEN,
          privatekey->dsa_priv);
      if (sign->dsa_sign == NULL) {
        ssh_set_error(session, SSH_FATAL, "Signing: openssl error");
        signature_free(sign);
        return NULL;
      }

#ifdef DEBUG_CRYPTO
      ssh_print_bignum("r",sign->dsa_sign->r);
      ssh_print_bignum("s",sign->dsa_sign->s);
#endif

#endif /* HAVE_LIBCRYPTO */
      sign->rsa_sign = NULL;
      break;
    case TYPE_RSA:
#ifdef HAVE_LIBGCRYPT
      if (gcry_sexp_build(&data_sexp, NULL, "(data(flags pkcs1)(hash sha1 %b))",
            SHA_DIGEST_LEN, hash + 1) ||
          gcry_pk_sign(&sign->rsa_sign, data_sexp, privatekey->rsa_priv)) {
        ssh_set_error(session, SSH_FATAL, "Signing: libgcrypt error");
        gcry_sexp_release(data_sexp);
        signature_free(sign);
        return NULL;
      }
#elif defined HAVE_LIBCRYPTO
      sign->rsa_sign = RSA_do_sign(hash + 1, SHA_DIGEST_LEN,
          privatekey->rsa_priv);
      if (sign->rsa_sign == NULL) {
        ssh_set_error(session, SSH_FATAL, "Signing: openssl error");
        signature_free(sign);
        return NULL;
      }
#endif
      sign->dsa_sign = NULL;
      break;
  }

#ifdef HAVE_LIBGCRYPT
  gcry_sexp_release(data_sexp);
#endif

  sign->type = privatekey->type;

  signature = signature_to_string(sign);
  signature_free(sign);

  return signature;
}

/** @} */
/* vim: set ts=2 sw=2 et cindent: */
