/*
 * string.c - ssh string functions
 *
 * This file is part of the SSH Library
 *
 * Copyright (c) 2003-2008 by Aris Adamantiadis
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

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#include "libssh/priv.h"
#include "libssh/string.h"
/** \defgroup ssh_string SSH Strings
 * \brief string manipulations
 */
/** \addtogroup ssh_string
 * @{ */

/**
 * \brief Creates a new SSH String object
 * \param size size of the string
 * \return the newly allocated string
 */
struct ssh_string_struct *string_new(size_t size) {
  struct ssh_string_struct *str = NULL;

  str = malloc(size + 4);
  if (str == NULL) {
    return NULL;
  }

  str->size = htonl(size);
  return str;
}

/**
 * @brief Fill a string with given data. The string should be big enough.
 *
 * @param s        An allocated string to fill with data.
 *
 * @param data     The data to fill the string with.
 *
 * @param len      Size of data.
 *
 * @return         0 on success, < 0 on error.
 */
int string_fill(struct ssh_string_struct *s, const void *data, size_t len) {
  if ((s == NULL) || (data == NULL) ||
      (len == 0) || (len > s->size)) {
    return -1;
  }

  memcpy(s->string, data, len);
  return 0;
}

/**
 * \brief Creates a ssh stream using a C string
 * \param what source 0-terminated C string
 * \return the newly allocated string.
 * \warning The nul byte is not copied nor counted in the ouput string.
 */
struct ssh_string_struct *string_from_char(const char *what) {
  struct ssh_string_struct *ptr = NULL;
  size_t len = strlen(what);

  ptr = malloc(4 + len);
  if (ptr == NULL) {
    return NULL;
  }
  ptr->size = htonl(len);
  memcpy(ptr->string, what, len);

  return ptr;
}

/**
 * \brief returns the size of a SSH string
 * \param s the input SSH string
 * \return size of the content of str, 0 on error
 */
size_t string_len(struct ssh_string_struct *s) {
  if (s == NULL) {
    return ntohl(0);
  }

  return ntohl(s->size);
}

/**
 * \brief convert a SSH string to a C nul-terminated string
 * \param s the input SSH string
 * \return a malloc'ed string pointer.
 * \warning If the input SSH string contains zeroes, some parts of
 * the output string may not be readable with regular libc functions.
 */
char *string_to_char(struct ssh_string_struct *s) {
  size_t len = ntohl(s->size) + 1;
  char *new = malloc(len);

  if (new == NULL) {
    return NULL;
  }
  memcpy(new, s->string, len - 1);
  new[len - 1] = '\0';
  return new;
}

/**
 * @brief Copy a string, return a newly allocated string. The caller has to
 *        free the string.
 *
 * @param s             String to copy.
 *
 * @return              Newly allocated copy of the string, NULL on error.
 */
struct ssh_string_struct *string_copy(struct ssh_string_struct *s) {
  struct ssh_string_struct *new = malloc(ntohl(s->size) + 4);

  if (new == NULL) {
    return NULL;
  }
  new->size = s->size;
  memcpy(new->string, s->string, ntohl(s->size));

  return new;
}

/** \brief destroy data in a string so it couldn't appear in a core dump
 * \param s string to burn
 */
void string_burn(struct ssh_string_struct *s) {
  if (s == NULL) {
    return;
  }
  memset(s->string, 'X', string_len(s));
}

/**
 * @brief Get the payload of the string.
 *
 * @param s             The string to get the data from.
 *
 * @return              Return the data of the string or NULL on error.
 */
void *string_data(struct ssh_string_struct *s) {
  if (s == NULL) {
    return NULL;
  }

  return s->string;
}

/**
 * \brief deallocate a STRING object
 * \param s String to delete
 */
void string_free(struct ssh_string_struct *s) {
  SAFE_FREE(s);
}

/** @} */
/* vim: set ts=2 sw=2 et cindent: */
