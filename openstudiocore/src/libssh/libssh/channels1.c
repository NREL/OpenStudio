/*
 * channels1.c - Support for SSH-1 type channels
 *
 * This file is part of the SSH Library
 *
 * Copyright (c) 2003-2008 by Aris Adamantiadis
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

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#ifndef _WIN32
#include <arpa/inet.h>
#endif
#include "libssh/priv.h"
#include "libssh/ssh1.h"
#include "libssh/buffer.h"
#include "libssh/packet.h"
#include "libssh/channels.h"
#include "libssh/session.h"

#ifdef WITH_SSH1

/*
 * This is a big hack. In fact, SSH1 doesn't make a clever use of channels.
 * The whole packets concerning shells are sent outside of a channel.
 * Thus, an inside limitation of this behaviour is that you can't only
 * request one shell.
 * The question is stil how they managed to imbed two "channel" into one
 * protocol.
 */

int channel_open_session1(ssh_channel chan) {
  /*
   * We guess we are requesting an *exec* channel. It can only have one exec
   * channel. So we abort with an error if we need more than one.
   */
  ssh_session session = chan->session;
  if (session->exec_channel_opened) {
    ssh_set_error(session, SSH_REQUEST_DENIED,
        "SSH1 supports only one execution channel. "
        "One has already been opened");
    return -1;
  }
  session->exec_channel_opened = 1;
  chan->open = 1;
  chan->local_maxpacket = 32000;
  chan->local_window = 64000;
  ssh_log(session, SSH_LOG_PACKET, "Opened a SSH1 channel session");

  return 0;
}

/*  10 SSH_CMSG_REQUEST_PTY
 *
 *  string       TERM environment variable value (e.g. vt100)
 *  32-bit int   terminal height, rows (e.g., 24)
 *  32-bit int   terminal width, columns (e.g., 80)
 *  32-bit int   terminal width, pixels (0 if no graphics) (e.g., 480)
 *  32-bit int   terminal height, pixels (0 if no graphics) (e.g., 640)
 *  n bytes      tty modes encoded in binary
 *  Some day, someone should have a look at that nasty tty encoded. It's
 *  much simplier under ssh2. I just hope the defaults values are ok ...
 */

int channel_request_pty_size1(ssh_channel channel, const char *terminal, int col,
    int row) {
  ssh_session session = channel->session;
  ssh_string str = NULL;

  str = string_from_char(terminal);
  if (str == NULL) {
    return -1;
  }

  if (buffer_add_u8(session->out_buffer, SSH_CMSG_REQUEST_PTY) < 0 ||
      buffer_add_ssh_string(session->out_buffer, str) < 0) {
    string_free(str);
    return -1;
  }
  string_free(str);

  if (buffer_add_u32(session->out_buffer, ntohl(row)) < 0 ||
      buffer_add_u32(session->out_buffer, ntohl(col)) < 0 ||
      buffer_add_u32(session->out_buffer, 0) < 0 || /* x */
      buffer_add_u32(session->out_buffer, 0) < 0 || /* y */
      buffer_add_u8(session->out_buffer, 0) < 0) { /* tty things */
    return -1;
  }

  ssh_log(session, SSH_LOG_FUNCTIONS, "Opening a ssh1 pty");
  if (packet_send(session) != SSH_OK ||
      packet_read(session) != SSH_OK ||
      packet_translate(session) != SSH_OK) {
    return -1;
  }

  switch (session->in_packet.type) {
    case SSH_SMSG_SUCCESS:
      ssh_log(session, SSH_LOG_RARE, "PTY: Success");
      return 0;
      break;
    case SSH_SMSG_FAILURE:
      ssh_set_error(session, SSH_REQUEST_DENIED,
          "Server denied PTY allocation");
      ssh_log(session, SSH_LOG_RARE, "PTY: denied\n");
      break;
    default:
      ssh_log(session, SSH_LOG_RARE, "PTY: error\n");
      ssh_set_error(session, SSH_FATAL,
          "Received unexpected packet type %d",
          session->in_packet.type);
      return -1;
  }

  return -1;
}

int channel_change_pty_size1(ssh_channel channel, int cols, int rows) {
  ssh_session session = channel->session;

  if (buffer_add_u8(session->out_buffer, SSH_CMSG_WINDOW_SIZE) < 0 ||
      buffer_add_u32(session->out_buffer, ntohl(rows)) < 0 ||
      buffer_add_u32(session->out_buffer, ntohl(cols)) < 0 ||
      buffer_add_u32(session->out_buffer, 0) < 0 ||
      buffer_add_u32(session->out_buffer, 0) < 0) {
    return -1;
  }

  if (packet_send(session)) {
    return -1;
  }

  ssh_log(session, SSH_LOG_RARE, "Change pty size send");

  if (packet_wait(session, SSH_SMSG_SUCCESS, 1) != SSH_OK) {
    return -1;
  }

  switch (session->in_packet.type) {
    case SSH_SMSG_SUCCESS:
      ssh_log(session, SSH_LOG_RARE, "pty size changed");
      return 0;
    case SSH_SMSG_FAILURE:
      ssh_log(session, SSH_LOG_RARE, "pty size change denied");
      ssh_set_error(session, SSH_REQUEST_DENIED, "pty size change denied");
      return -1;
  }

  ssh_set_error(session, SSH_FATAL, "Received unexpected packet type %d",
      session->in_packet.type);

  return -1;
}

int channel_request_shell1(ssh_channel channel) {
  ssh_session session = channel->session;

  if (buffer_add_u8(session->out_buffer,SSH_CMSG_EXEC_SHELL) < 0) {
    return -1;
  }

  if (packet_send(session) != SSH_OK) {
    return -1;
  }

  ssh_log(session, SSH_LOG_RARE, "Launched a shell");

  return 0;
}

int channel_request_exec1(ssh_channel channel, const char *cmd) {
  ssh_session session = channel->session;
  ssh_string command = NULL;

  command = string_from_char(cmd);
  if (command == NULL) {
    return -1;
  }

  if (buffer_add_u8(session->out_buffer, SSH_CMSG_EXEC_CMD) < 0 ||
      buffer_add_ssh_string(session->out_buffer, command) < 0) {
    string_free(command);
    return -1;
  }
  string_free(command);

  if(packet_send(session) != SSH_OK) {
    return -1;
  }

  ssh_log(session, SSH_LOG_RARE, "Executing %s ...", cmd);

  return 0;
}

static int channel_rcv_data1(ssh_session session, int is_stderr) {
    ssh_channel channel = session->channels;
    ssh_string str = NULL;

    str = buffer_get_ssh_string(session->in_buffer);
    if (str == NULL) {
      ssh_log(session, SSH_LOG_FUNCTIONS, "Invalid data packet !\n");
      return -1;
    }

    ssh_log(session, SSH_LOG_RARE,
        "Adding %zu bytes data in %d",
        string_len(str), is_stderr);

    if (channel_default_bufferize(channel, string_data(str), string_len(str),
          is_stderr) < 0) {
      string_free(str);
      return -1;
    }
    string_free(str);

    return 0;
}

static int channel_rcv_close1(ssh_session session) {
  ssh_channel channel = session->channels;
  uint32_t status;

  buffer_get_u32(session->in_buffer, &status);
  /*
   * It's much more than a channel closing. spec says it's the last
   * message sent by server (strange)
   */

  /* actually status is lost somewhere */
  channel->open = 0;
  channel->remote_eof = 1;

  if (buffer_add_u8(session->out_buffer, SSH_CMSG_EXIT_CONFIRMATION) < 0) {
    return -1;
  }

  if (packet_send(session) != SSH_OK) {
    return -1;
  }

  return 0;
}

int channel_handle1(ssh_session session, int type) {
  ssh_log(session, SSH_LOG_RARE, "Channel_handle1(%d)", type);
  switch (type) {
    case SSH_SMSG_STDOUT_DATA:
      if (channel_rcv_data1(session,0) < 0) {
        return -1;
      }
      break;
    case SSH_SMSG_STDERR_DATA:
      if (channel_rcv_data1(session,1) < 0) {
        return -1;
      }
      break;
    case SSH_SMSG_EXITSTATUS:
      if (channel_rcv_close1(session) < 0) {
        return -1;
      }
      break;
    default:
      ssh_log(session, SSH_LOG_FUNCTIONS, "Unexepected message %d", type);
  }

  return 0;
}

int channel_write1(ssh_channel channel, const void *data, int len) {
  ssh_session session = channel->session;
  int origlen = len;
  int effectivelen;
  const unsigned char *ptr=data;
  while (len > 0) {
    if (buffer_add_u8(session->out_buffer, SSH_CMSG_STDIN_DATA) < 0) {
      return -1;
    }

    effectivelen = len > 32000 ? 32000 : len;

    if (buffer_add_u32(session->out_buffer, htonl(effectivelen)) < 0 ||
        buffer_add_data(session->out_buffer, ptr, effectivelen) < 0) {
      return -1;
    }

    ptr += effectivelen;
    len -= effectivelen;

    if (packet_send(session) != SSH_OK) {
      return -1;
    }
  }

  return origlen;
}

#endif /* WITH_SSH1 */
/* vim: set ts=2 sw=2 et cindent: */
