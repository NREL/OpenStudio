/*
 * log.c - logging and debugging functions
 *
 * This file is part of the SSH Library
 *
 * Copyright (c) 2008      by Aris Adamantiadis
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "libssh/priv.h"
#include "libssh/session.h"

/**
 * @defgroup ssh_log SSH Logging
 *
 * @brief Logging functions for debugging and problem resolving
 */
/** \addtogroup ssh_log
 * @{ */

/**
 * @brief Log a SSH event.
 *
 * @param session       The SSH session.
 *
 * @param verbosity     The verbosity of the event.
 *
 * @param format        The format string of the log entry.
 */
void ssh_log(ssh_session session, int verbosity, const char *format, ...) {
  char buffer[1024];
  char indent[256];
  int min;
  va_list va;

  if (verbosity <= session->log_verbosity) {
    va_start(va, format);
    vsnprintf(buffer, sizeof(buffer), format, va);
    va_end(va);

    if (session->callbacks && session->callbacks->log_function) {
      session->callbacks->log_function(session, verbosity, buffer,
          session->callbacks->userdata);
    } else if (verbosity == SSH_LOG_FUNCTIONS) {
      if (session->log_indent > 255) {
        min = 255;
      } else {
        min = session->log_indent;
      }

      memset(indent, ' ', min);
      indent[min] = '\0';

      fprintf(stderr, "[func] %s%s\n", indent, buffer);
    } else {
      fprintf(stderr, "[%d] %s\n", verbosity, buffer);
    }
  }
}

/** @} */
/* vim: set ts=2 sw=2 et cindent: */
