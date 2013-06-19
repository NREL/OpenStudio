/*
 * This file is part of the SSH Library
 *
 * Copyright (c) 2003-2009 by Aris Adamantiadis
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

/*
 * priv.h file
 * This include file contains everything you shouldn't deal with in
 * user programs. Consider that anything in this file might change
 * without notice; libssh.h file will keep backward compatibility
 * on binary & source
 */

#ifndef _LIBSSH_PRIV_H
#define _LIBSSH_PRIV_H

#include "config.h"

#ifdef _MSC_VER

/** Imitate define of inttypes.h */
#define PRIdS "Id"

#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define strtoull _strtoui64
#define isblank(ch) ((ch) == ' ' || (ch) == '\t' || (ch) == '\n' || (ch) == '\r')

#if _MSC_VER >= 1400
#define strdup _strdup
#endif
#define usleep(X) Sleep(((X)+1000)/1000)

#undef strtok_r
#define strtok_r strtok_s

#ifndef HAVE_SNPRINTF
#ifdef HAVE__SNPRINTF_S
#define snprintf(d, n, ...) _snprintf_s((d), (n), _TRUNCATE, __VA_ARGS__)
#else
#ifdef HAVE__SNPRINTF
#define snprintf _snprintf
#else 
#error "no snprintf compatible function found"
#endif /* HAVE__SNPRINTF */
#endif /* HAVE__SNPRINTF_S */
#endif /* HAVE_SNPRINTF */

#ifndef HAVE_VSNPRINTF
#ifdef HAVE__VSNPRINTF_S
#define vsnprintf(s, n, f, v) _vsnprintf_s((s), (n), _TRUNCATE, (f), (v))
#else
#ifdef HAVE__VSNPRINTF
#define vsnprintf _vsnprintf
#else /* HAVE_VSNPRINTF */
#error "No vsnprintf compatible function found"
#endif /* HAVE__VSNPRINTF */
#endif /* HAVE__VSNPRINTF_S */
#endif /* HAVE_VSNPRINTF */

#ifndef HAVE_STRNCPY
#define strncpy(d, s, n) strncpy_s((d), (n), (s), _TRUNCATE)
#endif
#else /* _MSC_VER */

#include <unistd.h>
#define PRIdS "zd"

#endif /* _MSC_VER */

#include "libssh/libssh.h"
#include "libssh/callbacks.h"
#include "libssh/crypto.h"
/* some constants */
#define MAX_PACKET_LEN 262144
#define ERROR_BUFFERLEN 1024
#define CLIENTBANNER1 "SSH-1.5-libssh-" SSH_STRINGIFY(LIBSSH_VERSION)
#define CLIENTBANNER2 "SSH-2.0-libssh-" SSH_STRINGIFY(LIBSSH_VERSION)
#define KBDINT_MAX_PROMPT 256 /* more than openssh's :) */
/* some types for public keys */
enum public_key_types_e{
	TYPE_DSS=1,
	TYPE_RSA,
	TYPE_RSA1
};

#ifdef __cplusplus
extern "C" {
#endif


#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

typedef struct kex_struct {
	unsigned char cookie[16];
	char **methods;
} KEX;

struct error_struct {
/* error handling */
    unsigned int error_code;
    char error_buffer[ERROR_BUFFERLEN];
};

/* TODO: remove that include */
#include "libssh/wrapper.h"

struct ssh_keys_struct {
  const char *privatekey;
  const char *publickey;
};

struct ssh_message_struct;


/* server data */

struct ssh_bind_struct {
  struct error_struct error;

  ssh_callbacks callbacks; /* Callbacks to user functions */

  /* options */
  char *wanted_methods[10];
  char *banner;
  char *dsakey;
  char *rsakey;
  char *bindaddr;
  socket_t bindfd;
  unsigned int bindport;
  unsigned int log_verbosity;

  int blocking;
  int toaccept;
};


/* client.c */

int ssh_send_banner(ssh_session session, int is_server);
char *ssh_get_banner(ssh_session session);

/* config.c */
int ssh_config_parse_file(ssh_session session, const char *filename);

/* errors.c */
void ssh_set_error(void *error, int code, const char *descr, ...) PRINTF_ATTRIBUTE(3, 4);
void ssh_set_error_oom(void *);
void ssh_set_error_invalid(void *, const char *);

/* in crypt.c */
uint32_t packet_decrypt_len(ssh_session session,char *crypted);
int packet_decrypt(ssh_session session, void *packet,unsigned int len);
unsigned char *packet_encrypt(ssh_session session,void *packet,unsigned int len);
 /* it returns the hmac buffer if exists*/
int packet_hmac_verify(ssh_session session,ssh_buffer buffer,unsigned char *mac);

/* connect.c */
int ssh_regex_init(void);
void ssh_regex_finalize(void);
ssh_session ssh_session_new(void);
socket_t ssh_connect_host(ssh_session session, const char *host,const char
        *bind_addr, int port, long timeout, long usec);

/* in kex.c */
extern const char *ssh_kex_nums[];
int ssh_send_kex(ssh_session session, int server_kex);
void ssh_list_kex(ssh_session session, KEX *kex);
int set_kex(ssh_session session);
int ssh_get_kex(ssh_session session, int server_kex);
int verify_existing_algo(int algo, const char *name);
char **space_tokenize(const char *chain);
int ssh_get_kex1(ssh_session session);
char *ssh_find_matching(const char *in_d, const char *what_d);

/* in base64.c */
ssh_buffer base64_to_bin(const char *source);
unsigned char *bin_to_base64(const unsigned char *source, int len);

/* gzip.c */
int compress_buffer(ssh_session session,ssh_buffer buf);
int decompress_buffer(ssh_session session,ssh_buffer buf, size_t maxlen);

/* crc32.c */
uint32_t ssh_crc32(const char *buf, uint32_t len);

/* auth1.c */
int ssh_userauth1_none(ssh_session session, const char *username);
int ssh_userauth1_offer_pubkey(ssh_session session, const char *username,
        int type, ssh_string pubkey);
int ssh_userauth1_password(ssh_session session, const char *username,
        const char *password);

/* channels1.c */
int channel_open_session1(ssh_channel channel);
int channel_request_pty_size1(ssh_channel channel, const char *terminal,
    int cols, int rows);
int channel_change_pty_size1(ssh_channel channel, int cols, int rows);
int channel_request_shell1(ssh_channel channel);
int channel_request_exec1(ssh_channel channel, const char *cmd);
int channel_handle1(ssh_session session, int type);
int channel_write1(ssh_channel channel, const void *data, int len);

/* match.c */
int match_hostname(const char *host, const char *pattern, unsigned int len);

/* log.c */

/* misc.c */
#ifdef _WIN32
int gettimeofday(struct timeval *__p, void *__t);
#endif /* _WIN32 */

#ifndef __FUNCTION__
#if defined(__SUNPRO_C)
#define __FUNCTION__ __func__
#endif
#endif

#define _enter_function(sess) \
	do {\
		if((sess)->log_verbosity >= SSH_LOG_FUNCTIONS){ \
			ssh_log((sess),SSH_LOG_FUNCTIONS,"entering function %s line %d in " __FILE__ , __FUNCTION__,__LINE__);\
			(sess)->log_indent++; \
		} \
	} while(0)

#define _leave_function(sess) \
	do { \
		if((sess)->log_verbosity >= SSH_LOG_FUNCTIONS){ \
			(sess)->log_indent--; \
			ssh_log((sess),SSH_LOG_FUNCTIONS,"leaving function %s line %d in " __FILE__ , __FUNCTION__,__LINE__);\
		}\
	} while(0)

#ifdef DEBUG_CALLTRACE
#define enter_function() _enter_function(session)
#define leave_function() _leave_function(session)
#else
#define enter_function() (void)session
#define leave_function() (void)session
#endif

/* options.c  */

int ssh_options_set_algo(ssh_session session, int algo, const char *list);
int ssh_options_apply(ssh_session session);

/** Free memory space */
#define SAFE_FREE(x) do { if ((x) != NULL) {free(x); x=NULL;} } while(0)

/** Zero a structure */
#define ZERO_STRUCT(x) memset((char *)&(x), 0, sizeof(x))

/** Zero a structure given a pointer to the structure */
#define ZERO_STRUCTP(x) do { if ((x) != NULL) memset((char *)(x), 0, sizeof(*(x))); } while(0)

/** Get the size of an array */
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

/** Overwrite the complete string with 'X' */
#define BURN_STRING(x) do { if ((x) != NULL) memset((x), 'X', strlen((x))); } while(0)

#ifdef HAVE_LIBGCRYPT
/* gcrypt_missing.c */
int my_gcry_dec2bn(bignum *bn, const char *data);
char *my_gcry_bn2dec(bignum bn);
#endif /* !HAVE_LIBGCRYPT */

#ifdef __cplusplus
}
#endif

#endif /* _LIBSSH_PRIV_H */
/* vim: set ts=2 sw=2 et cindent: */
