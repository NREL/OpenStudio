/*
 * channels.c - SSH channel functions
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
#include <stdio.h>
#include <errno.h>
#include <time.h>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#include "libssh/priv.h"
#include "libssh/ssh2.h"
#include "libssh/buffer.h"
#include "libssh/packet.h"
#include "libssh/socket.h"
#include "libssh/channels.h"
#include "libssh/session.h"
#include "libssh/misc.h"
#include "libssh/messages.h"

#define WINDOWBASE 128000
#define WINDOWLIMIT (WINDOWBASE/2)

/**
 * @defgroup ssh_channel SSH Channels
 * @brief Functions that manage a channel.
 */

/**
 * @addtogroup ssh_channel
 * @{
 */

/**
 * @brief Allocate a new channel.
 *
 * @param session       The ssh session to use.
 *
 * @return A pointer to a newly allocated channel, NULL on error.
 */
ssh_channel channel_new(ssh_session session) {
  ssh_channel channel = NULL;

  channel = malloc(sizeof(struct ssh_channel_struct));
  if (channel == NULL) {
    return NULL;
  }
  memset(channel,0,sizeof(struct ssh_channel_struct));

  channel->stdout_buffer = buffer_new();
  if (channel->stdout_buffer == NULL) {
    SAFE_FREE(channel);
    return NULL;
  }

  channel->stderr_buffer = buffer_new();
  if (channel->stderr_buffer == NULL) {
    buffer_free(channel->stdout_buffer);
    SAFE_FREE(channel);
    return NULL;
  }

  channel->session = session;
  channel->version = session->version;
  channel->exit_status = -1;

  if(session->channels == NULL) {
    session->channels = channel;
    channel->next = channel->prev = channel;
    return channel;
  }
  channel->next = session->channels;
  channel->prev = session->channels->prev;
  channel->next->prev = channel;
  channel->prev->next = channel;

  return channel;
}

/**
 * @internal
 *
 * @brief Create a new channel identifier.
 *
 * @param  session      The SSH session to use.
 *
 * @return The new channel identifier.
 */
uint32_t ssh_channel_new_id(ssh_session session) {
  return ++(session->maxchannel);
}

static int channel_open(ssh_channel channel, const char *type_c, int window,
    int maxpacket, ssh_buffer payload) {
  ssh_session session = channel->session;
  ssh_string type = NULL;
  uint32_t tmp = 0;

  enter_function();

  channel->local_channel = ssh_channel_new_id(session);
  channel->local_maxpacket = maxpacket;
  channel->local_window = window;

  ssh_log(session, SSH_LOG_RARE,
      "Creating a channel %d with %d window and %d max packet",
      channel->local_channel, window, maxpacket);

  type = string_from_char(type_c);
  if (type == NULL) {
    leave_function();
    return -1;
  }

  if (buffer_add_u8(session->out_buffer, SSH2_MSG_CHANNEL_OPEN) < 0 ||
      buffer_add_ssh_string(session->out_buffer,type) < 0 ||
      buffer_add_u32(session->out_buffer, htonl(channel->local_channel)) < 0 ||
      buffer_add_u32(session->out_buffer, htonl(channel->local_window)) < 0 ||
      buffer_add_u32(session->out_buffer, htonl(channel->local_maxpacket)) < 0) {
    string_free(type);
    leave_function();
    return -1;
  }

  string_free(type);

  if (payload != NULL) {
    if (buffer_add_buffer(session->out_buffer, payload) < 0) {
      leave_function();
      return -1;
    }
  }

  if (packet_send(session) != SSH_OK) {
    leave_function();
    return -1;
  }

  ssh_log(session, SSH_LOG_RARE,
      "Sent a SSH_MSG_CHANNEL_OPEN type %s for channel %d",
      type_c, channel->local_channel);

  if (packet_wait(session, SSH2_MSG_CHANNEL_OPEN_CONFIRMATION, 1) != SSH_OK) {
    leave_function();
    return -1;
  }

  switch(session->in_packet.type) {
    case SSH2_MSG_CHANNEL_OPEN_CONFIRMATION:
      buffer_get_u32(session->in_buffer, &tmp);

      if (channel->local_channel != ntohl(tmp)) {
        ssh_set_error(session, SSH_FATAL,
            "Server answered with sender channel number %lu instead of given %u",
            (long unsigned int) ntohl(tmp),
            channel->local_channel);
        leave_function();
        return -1;
      }
      buffer_get_u32(session->in_buffer, &tmp);
      channel->remote_channel = ntohl(tmp);

      buffer_get_u32(session->in_buffer, &tmp);
      channel->remote_window = ntohl(tmp);

      buffer_get_u32(session->in_buffer,&tmp);
      channel->remote_maxpacket=ntohl(tmp);

      ssh_log(session, SSH_LOG_PROTOCOL,
          "Received a CHANNEL_OPEN_CONFIRMATION for channel %d:%d",
          channel->local_channel,
          channel->remote_channel);
      ssh_log(session, SSH_LOG_PROTOCOL,
          "Remote window : %lu, maxpacket : %lu",
          (long unsigned int) channel->remote_window,
          (long unsigned int) channel->remote_maxpacket);

      channel->open = 1;
      leave_function();
      return 0;
    case SSH2_MSG_CHANNEL_OPEN_FAILURE:
      {
        ssh_string error_s;
        char *error;
        uint32_t code;

        buffer_get_u32(session->in_buffer, &tmp);
        buffer_get_u32(session->in_buffer, &code);

        error_s = buffer_get_ssh_string(session->in_buffer);
        error = string_to_char(error_s);
        string_free(error_s);
        if (error == NULL) {
          leave_function();
          return -1;
        }

        ssh_set_error(session, SSH_REQUEST_DENIED,
            "Channel opening failure: channel %u error (%lu) %s",
            channel->local_channel,
            (long unsigned int) ntohl(code),
            error);
        SAFE_FREE(error);

        leave_function();
        return -1;
      }
    default:
      ssh_set_error(session, SSH_FATAL,
          "Received unknown packet %d\n", session->in_packet.type);
      leave_function();
      return -1;
  }

  leave_function();
  return -1;
}

/* get ssh channel from local session? */
ssh_channel ssh_channel_from_local(ssh_session session, uint32_t id) {
  ssh_channel initchan = session->channels;
  ssh_channel channel;

  /* We assume we are always the local */
  if (initchan == NULL) {
    return NULL;
  }

  for (channel = initchan; channel->local_channel != id;
      channel=channel->next) {
    if (channel->next == initchan) {
      return NULL;
    }
  }

  return channel;
}

static int grow_window(ssh_session session, ssh_channel channel, int minimumsize) {
  uint32_t new_window = minimumsize > WINDOWBASE ? minimumsize : WINDOWBASE;

  enter_function();

  if (buffer_add_u8(session->out_buffer, SSH2_MSG_CHANNEL_WINDOW_ADJUST) < 0 ||
      buffer_add_u32(session->out_buffer, htonl(channel->remote_channel)) < 0 ||
      buffer_add_u32(session->out_buffer, htonl(new_window)) < 0) {
    goto error;
  }

  if (packet_send(session) != SSH_OK) {
    /* FIXME should we fail here or not? */
    leave_function();
    return 1;
  }

  ssh_log(session, SSH_LOG_PROTOCOL,
      "growing window (channel %d:%d) to %d bytes",
      channel->local_channel,
      channel->remote_channel,
      channel->local_window + new_window);

  channel->local_window += new_window;

  leave_function();
  return 0;
error:
  buffer_reinit(session->out_buffer);

  leave_function();
  return -1;
}

static ssh_channel channel_from_msg(ssh_session session) {
  ssh_channel channel;
  uint32_t chan;

  if (buffer_get_u32(session->in_buffer, &chan) != sizeof(uint32_t)) {
    ssh_set_error(session, SSH_FATAL,
        "Getting channel from message: short read");
    return NULL;
  }

  channel = ssh_channel_from_local(session, ntohl(chan));
  if (channel == NULL) {
    ssh_set_error(session, SSH_FATAL,
        "Server specified invalid channel %lu",
        (long unsigned int) ntohl(chan));
  }

  return channel;
}

static void channel_rcv_change_window(ssh_session session) {
  ssh_channel channel;
  uint32_t bytes;
  int rc;

  enter_function();

  channel = channel_from_msg(session);
  if (channel == NULL) {
    ssh_log(session, SSH_LOG_FUNCTIONS, "%s", ssh_get_error(session));
  }

  rc = buffer_get_u32(session->in_buffer, &bytes);
  if (channel == NULL || rc != sizeof(uint32_t)) {
    ssh_log(session, SSH_LOG_PACKET,
        "Error getting a window adjust message: invalid packet");
    leave_function();
    return;
  }

  bytes = ntohl(bytes);
  ssh_log(session, SSH_LOG_PROTOCOL,
      "Adding %d bytes to channel (%d:%d) (from %d bytes)",
      bytes,
      channel->local_channel,
      channel->remote_channel,
      channel->remote_window);

  channel->remote_window += bytes;

  leave_function();
}

/* is_stderr is set to 1 if the data are extended, ie stderr */
static void channel_rcv_data(ssh_session session,int is_stderr) {
  ssh_channel channel;
  ssh_string str;
  size_t len;

  enter_function();

  channel = channel_from_msg(session);
  if (channel == NULL) {
    ssh_log(session, SSH_LOG_FUNCTIONS,
        "%s", ssh_get_error(session));
    leave_function();
    return;
  }

  if (is_stderr) {
    uint32_t ignore;
    /* uint32 data type code. we can ignore it */
    buffer_get_u32(session->in_buffer, &ignore);
  }

  str = buffer_get_ssh_string(session->in_buffer);
  if (str == NULL) {
    ssh_log(session, SSH_LOG_PACKET, "Invalid data packet!");
    leave_function();
    return;
  }
  len = string_len(str);

  ssh_log(session, SSH_LOG_PROTOCOL,
      "Channel receiving %zu bytes data in %d (local win=%d remote win=%d)",
      len,
      is_stderr,
      channel->local_window,
      channel->remote_window);

  /* What shall we do in this case? Let's accept it anyway */
  if (len > channel->local_window) {
    ssh_log(session, SSH_LOG_RARE,
        "Data packet too big for our window(%zu vs %d)",
        len,
        channel->local_window);
  }

  if (channel_default_bufferize(channel, string_data(str), len,
        is_stderr) < 0) {
    string_free(str);
    leave_function();
    return;
  }

  if (len <= channel->local_window) {
    channel->local_window -= len;
  } else {
    channel->local_window = 0; /* buggy remote */
  }

  ssh_log(session, SSH_LOG_PROTOCOL,
      "Channel windows are now (local win=%d remote win=%d)",
      channel->local_window,
      channel->remote_window);

  string_free(str);
  leave_function();
}

static void channel_rcv_eof(ssh_session session) {
  ssh_channel channel;

  enter_function();

  channel = channel_from_msg(session);
  if (channel == NULL) {
    ssh_log(session, SSH_LOG_FUNCTIONS, "%s", ssh_get_error(session));
    leave_function();
    return;
  }

  ssh_log(session, SSH_LOG_PACKET,
      "Received eof on channel (%d:%d)",
      channel->local_channel,
      channel->remote_channel);
  /* channel->remote_window = 0; */
  channel->remote_eof = 1;

  leave_function();
}

static void channel_rcv_close(ssh_session session) {
  ssh_channel channel;

  enter_function();

  channel = channel_from_msg(session);
  if (channel == NULL) {
    ssh_log(session, SSH_LOG_FUNCTIONS, "%s", ssh_get_error(session));
    leave_function();
    return;
  }

  ssh_log(session, SSH_LOG_PACKET,
      "Received close on channel (%d:%d)",
      channel->local_channel,
      channel->remote_channel);

  if ((channel->stdout_buffer &&
        buffer_get_rest_len(channel->stdout_buffer) > 0) ||
      (channel->stderr_buffer &&
       buffer_get_rest_len(channel->stderr_buffer) > 0)) {
    channel->delayed_close = 1;
  } else {
    channel->open = 0;
  }

  if (channel->remote_eof == 0) {
    ssh_log(session, SSH_LOG_PACKET,
        "Remote host not polite enough to send an eof before close");
  }
  channel->remote_eof = 1;
  /*
   * The remote eof doesn't break things if there was still data into read
   * buffer because the eof is ignored until the buffer is empty.
   */

  leave_function();
}

static void channel_rcv_request(ssh_session session) {
  ssh_channel channel;
  ssh_string request_s;
  char *request;
  uint32_t status;
  uint32_t startpos = session->in_buffer->pos;

  enter_function();

  channel = channel_from_msg(session);
  if (channel == NULL) {
    ssh_log(session, SSH_LOG_FUNCTIONS,"%s", ssh_get_error(session));
    leave_function();
    return;
  }

  request_s = buffer_get_ssh_string(session->in_buffer);
  if (request_s == NULL) {
    ssh_log(session, SSH_LOG_PACKET, "Invalid MSG_CHANNEL_REQUEST");
    leave_function();
    return;
  }

  request = string_to_char(request_s);
  string_free(request_s);
  if (request == NULL) {
    leave_function();
    return;
  }

  buffer_get_u8(session->in_buffer, (uint8_t *) &status);

  if (strcmp(request,"exit-status") == 0) {
    SAFE_FREE(request);
    ssh_log(session, SSH_LOG_PACKET, "received exit-status");
    buffer_get_u32(session->in_buffer, &status);
    channel->exit_status = ntohl(status);

    leave_function();
    return ;
  }

  if (strcmp(request, "exit-signal") == 0) {
    const char *core = "(core dumped)";
    ssh_string signal_s;
    char *signal;
    uint8_t i;

    SAFE_FREE(request);

    signal_s = buffer_get_ssh_string(session->in_buffer);
    if (signal_s == NULL) {
      ssh_log(session, SSH_LOG_PACKET, "Invalid MSG_CHANNEL_REQUEST");
      leave_function();
      return;
    }

    signal = string_to_char(signal_s);
    string_free(signal_s);
    if (signal == NULL) {
      leave_function();
      return;
    }

    buffer_get_u8(session->in_buffer, &i);
    if (i == 0) {
      core = "";
    }

    ssh_log(session, SSH_LOG_PACKET,
        "Remote connection closed by signal SIG %s %s", signal, core);
    SAFE_FREE(signal);

    leave_function();
    return;
  }

  if(strcmp(request,"keepalive@openssh.com")==0){
    SAFE_FREE(request);
    ssh_log(session, SSH_LOG_PROTOCOL,"Responding to Openssh's keepalive");
    buffer_add_u8(session->out_buffer, SSH2_MSG_CHANNEL_SUCCESS);
    buffer_add_u32(session->out_buffer, htonl(channel->remote_channel));
    packet_send(session);
    leave_function();
    return;
  }
  /* TODO call message_handle since it handles channel requests as messages */
  /* *but* reset buffer before !! */

  session->in_buffer->pos = startpos;
  message_handle(session, SSH2_MSG_CHANNEL_REQUEST);

  ssh_log(session, SSH_LOG_PACKET, "Unknown request %s", request);
  SAFE_FREE(request);

  leave_function();
}

/*
 * channel_handle() is called by packet_wait(), for example when there is
 * channel information to handle.
 */
void channel_handle(ssh_session session, int type){
  enter_function();

  ssh_log(session, SSH_LOG_PROTOCOL, "Channel_handle(%d)", type);

  switch(type) {
    case SSH2_MSG_CHANNEL_WINDOW_ADJUST:
      channel_rcv_change_window(session);
      break;
    case SSH2_MSG_CHANNEL_DATA:
      channel_rcv_data(session,0);
      break;
    case SSH2_MSG_CHANNEL_EXTENDED_DATA:
      channel_rcv_data(session,1);
      break;
    case SSH2_MSG_CHANNEL_EOF:
      channel_rcv_eof(session);
      break;
    case SSH2_MSG_CHANNEL_CLOSE:
      channel_rcv_close(session);
      break;
    case SSH2_MSG_CHANNEL_REQUEST:
      channel_rcv_request(session);
      break;
    default:
      ssh_log(session, SSH_LOG_FUNCTIONS,
          "Unexpected message %d", type);
  }

  leave_function();
}

/*
 * When data has been received from the ssh server, it can be applied to the
 * known user function, with help of the callback, or inserted here
 *
 * FIXME is the window changed?
 */
int channel_default_bufferize(ssh_channel channel, void *data, int len,
    int is_stderr) {
  ssh_session session = channel->session;

  ssh_log(session, SSH_LOG_RARE,
      "placing %d bytes into channel buffer (stderr=%d)", len, is_stderr);
  if (is_stderr == 0) {
    /* stdout */
    if (channel->stdout_buffer == NULL) {
      channel->stdout_buffer = buffer_new();
      if (channel->stdout_buffer == NULL) {
        ssh_set_error_oom(session);
        return -1;
      }
    }

    if (buffer_add_data(channel->stdout_buffer, data, len) < 0) {
      buffer_free(channel->stdout_buffer);
      channel->stdout_buffer = NULL;
      return -1;
    }
  } else {
    /* stderr */
    if (channel->stderr_buffer == NULL) {
      channel->stderr_buffer = buffer_new();
      if (channel->stderr_buffer == NULL) {
        ssh_set_error_oom(session);
        return -1;
      }
    }

    if (buffer_add_data(channel->stderr_buffer, data, len) < 0) {
      buffer_free(channel->stderr_buffer);
      channel->stderr_buffer = NULL;
      return -1;
    }
  }

  return 0;
}

/**
 * @brief Open a session channel (suited for a shell, not TCP forwarding).
 *
 * @param channel       An allocated channel.
 *
 * @return SSH_OK on success\n
 *         SSH_ERROR on error.
 *
 * @see channel_open_forward()
 * @see channel_request_env()
 * @see channel_request_shell()
 * @see channel_request_exec()
 */
int channel_open_session(ssh_channel channel) {
#ifdef WITH_SSH1
  if (channel->session->version == 1) {
    return channel_open_session1(channel);
  }
#endif

  return channel_open(channel,"session",64000,32000,NULL);
}

/**
 * @brief Open a TCP/IP forwarding channel.
 *
 * @param channel       An allocated channel.
 *
 * @param remotehost    The remote host to connected (host name or IP).
 *
 * @param remoteport    The remote port.
 *
 * @param sourcehost    The source host (your local computer). It's facultative
 *                      and for logging purpose.
 *
 * @param localport     The source port (your local computer). It's facultative
 *                      and for logging purpose.
 * @warning This function does not bind the local port and does not automatically
 *          forward the content of a socket to the channel. You still have to
 *          use channel_read and channel_write for this.
 *
 * @return SSH_OK on success\n
 *         SSH_ERROR on error
 */
int channel_open_forward(ssh_channel channel, const char *remotehost,
    int remoteport, const char *sourcehost, int localport) {
  ssh_session session = channel->session;
  ssh_buffer payload = NULL;
  ssh_string str = NULL;
  int rc = SSH_ERROR;

  enter_function();

  payload = buffer_new();
  if (payload == NULL) {
    goto error;
  }
  str = string_from_char(remotehost);
  if (str == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(payload, str) < 0 ||
      buffer_add_u32(payload,htonl(remoteport)) < 0) {
    goto error;
  }

  string_free(str);
  str = string_from_char(sourcehost);
  if (str == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(payload, str) < 0 ||
      buffer_add_u32(payload,htonl(localport)) < 0) {
    goto error;
  }

  rc = channel_open(channel, "direct-tcpip", 64000, 32000, payload);

error:
  buffer_free(payload);
  string_free(str);

  leave_function();
  return rc;
}

/**
 * @brief Close and free a channel.
 *
 * @param channel       The channel to free.
 *
 * @warning Any data unread on this channel will be lost.
 */
void channel_free(ssh_channel channel) {
  ssh_session session = channel->session;
  enter_function();

  if (channel == NULL) {
    leave_function();
    return;
  }

  if (session->alive && channel->open) {
    channel_close(channel);
  }

  /* handle the "my channel is first on session list" case */
  if (session->channels == channel) {
    session->channels = channel->next;
  }

  /* handle the "my channel is the only on session list" case */
  if (channel->next == channel) {
    session->channels = NULL;
  } else {
    channel->prev->next = channel->next;
    channel->next->prev = channel->prev;
  }

  buffer_free(channel->stdout_buffer);
  buffer_free(channel->stderr_buffer);

  /* debug trick to catch use after frees */
  memset(channel, 'X', sizeof(struct ssh_channel_struct));
  SAFE_FREE(channel);

  leave_function();
}

/**
 * @brief Send an end of file on the channel.
 *
 * This doesn't close the channel. You may still read from it but not write.
 *
 * @param channel       The channel to send the eof to.
 *
 * @return SSH_SUCCESS on success\n
 *         SSH_ERROR on error\n
 *
 * @see channel_close()
 * @see channel_free()
 */
int channel_send_eof(ssh_channel channel){
  ssh_session session = channel->session;
  int rc = SSH_ERROR;

  enter_function();

  if (buffer_add_u8(session->out_buffer, SSH2_MSG_CHANNEL_EOF) < 0) {
    goto error;
  }
  if (buffer_add_u32(session->out_buffer,htonl(channel->remote_channel)) < 0) {
    goto error;
  }
  rc = packet_send(session);
  ssh_log(session, SSH_LOG_PACKET,
      "Sent a EOF on client channel (%d:%d)",
      channel->local_channel,
      channel->remote_channel);

  channel->local_eof = 1;

  leave_function();
  return rc;
error:
  buffer_reinit(session->out_buffer);

  leave_function();
  return rc;
}

/**
 * @brief Close a channel.
 *
 * This sends an end of file and then closes the channel. You won't be able
 * to recover any data the server was going to send or was in buffers.
 *
 * @param channel       The channel to close.
 *
 * @return SSH_SUCCESS on success\n
 *         SSH_ERROR on error
 *
 * @see channel_free()
 * @see channel_eof()
 */
int channel_close(ssh_channel channel){
  ssh_session session = channel->session;
  int rc = 0;

  enter_function();

  if (channel->local_eof == 0) {
    rc = channel_send_eof(channel);
  }

  if (rc != SSH_OK) {
    leave_function();
    return rc;
  }

  if (buffer_add_u8(session->out_buffer, SSH2_MSG_CHANNEL_CLOSE) < 0 ||
      buffer_add_u32(session->out_buffer, htonl(channel->remote_channel)) < 0) {
    goto error;
  }

  rc = packet_send(session);
  ssh_log(session, SSH_LOG_PACKET,
      "Sent a close on client channel (%d:%d)",
      channel->local_channel,
      channel->remote_channel);

  if(rc == SSH_OK) {
    channel->open = 0;
  }

  leave_function();
  return rc;
error:
  buffer_reinit(session->out_buffer);

  leave_function();
  return rc;
}

int channel_write_common(ssh_channel channel, const void *data,
    uint32_t len, int is_stderr) {
  ssh_session session = channel->session;
  int origlen = len;
  int effectivelen;

  enter_function();

  if (channel->local_eof) {
    ssh_set_error(session, SSH_REQUEST_DENIED,
        "Can't write to channel %d:%d  after EOF was sent",
        channel->local_channel,
        channel->remote_channel);
    leave_function();
    return -1;
  }

  if (channel->open == 0 || channel->delayed_close != 0) {
    ssh_set_error(session, SSH_REQUEST_DENIED, "Remote channel is closed");
    leave_function();
    return -1;
  }

#ifdef WITH_SSH1
  if (channel->version == 1) {
    int rc = channel_write1(channel, data, len);
    leave_function();
    return rc;
  }
#endif

  while (len > 0) {
    if (channel->remote_window < len) {
      ssh_log(session, SSH_LOG_PROTOCOL,
          "Remote window is %d bytes. going to write %d bytes",
          channel->remote_window,
          len);
      ssh_log(session, SSH_LOG_PROTOCOL,
          "Waiting for a growing window message...");
      /* What happens when the channel window is zero? */
      while(channel->remote_window == 0) {
        /* parse every incoming packet */
        if (packet_wait(channel->session, 0, 0) == SSH_ERROR) {
          leave_function();
          return SSH_ERROR;
        }
      }
      effectivelen = len > channel->remote_window ? channel->remote_window : len;
    } else {
      effectivelen = len;
    }

    if (buffer_add_u8(session->out_buffer, is_stderr ?
				SSH2_MSG_CHANNEL_EXTENDED_DATA : SSH2_MSG_CHANNEL_DATA) < 0 ||
        buffer_add_u32(session->out_buffer,
          htonl(channel->remote_channel)) < 0 ||
        buffer_add_u32(session->out_buffer, htonl(effectivelen)) < 0 ||
        buffer_add_data(session->out_buffer, data, effectivelen) < 0) {
      goto error;
    }

    if (packet_send(session) != SSH_OK) {
      leave_function();
      return SSH_ERROR;
    }

    ssh_log(session, SSH_LOG_RARE,
        "channel_write wrote %d bytes", effectivelen);

    channel->remote_window -= effectivelen;
    len -= effectivelen;
    data = ((uint8_t*)data + effectivelen);
  }

  leave_function();
  return origlen;
error:
  buffer_reinit(session->out_buffer);

  leave_function();
  return SSH_ERROR;
}

/**
 * @brief Blocking write on channel.
 *
 * @param channel       The channel to write to.
 *
 * @param data          A pointer to the data to write.
 *
 * @param len           The length of the buffer to write to.
 *
 * @return The number of bytes written, SSH_ERROR on error.
 *
 * @see channel_read()
 */
int channel_write(ssh_channel channel, const void *data, uint32_t len) {
  return channel_write_common(channel, data, len, 0);
}

/**
 * @brief Check if the channel is open or not.
 *
 * @param channel       The channel to check.
 *
 * @return 0 if channel is closed, nonzero otherwise.
 *
 * @see channel_is_closed()
 */
int channel_is_open(ssh_channel channel) {
  return (channel->open != 0 && channel->session->alive != 0);
}

/**
 * @brief Check if the channel is closed or not.
 *
 * @param channel       The channel to check.
 *
 * @return 0 if channel is opened, nonzero otherwise.
 *
 * @see channel_is_open()
 */
int channel_is_closed(ssh_channel channel) {
  return (channel->open == 0 || channel->session->alive == 0);
}

/**
 * @brief Check if remote has sent an EOF.
 *
 * @param channel       The channel to check.
 *
 * @return 0 if there is no EOF, nonzero otherwise.
 */
int channel_is_eof(ssh_channel channel) {
  if ((channel->stdout_buffer &&
        buffer_get_rest_len(channel->stdout_buffer) > 0) ||
      (channel->stderr_buffer &&
       buffer_get_rest_len(channel->stderr_buffer) > 0)) {
    return 0;
  }

  return (channel->remote_eof != 0);
}

/**
 * @brief Put the channel into blocking or nonblocking mode.
 *
 * @param channel       The channel to use.
 *
 * @param blocking      A boolean for blocking or nonblocking.
 *
 * @bug This functionnality is still under development and
 *      doesn't work correctly.
 */
void channel_set_blocking(ssh_channel channel, int blocking) {
  channel->blocking = (blocking == 0 ? 0 : 1);
}

static int channel_request(ssh_channel channel, const char *request,
    ssh_buffer buffer, int reply) {
  ssh_session session = channel->session;
  ssh_string req = NULL;
  int rc = SSH_ERROR;

  enter_function();

  req = string_from_char(request);
  if (req == NULL) {
    goto error;
  }

  if (buffer_add_u8(session->out_buffer, SSH2_MSG_CHANNEL_REQUEST) < 0 ||
      buffer_add_u32(session->out_buffer, htonl(channel->remote_channel)) < 0 ||
      buffer_add_ssh_string(session->out_buffer, req) < 0 ||
      buffer_add_u8(session->out_buffer, reply == 0 ? 0 : 1) < 0) {
    goto error;
  }
  string_free(req);

  if (buffer != NULL) {
    if (buffer_add_data(session->out_buffer, buffer_get(buffer),
        buffer_get_len(buffer)) < 0) {
      goto error;
    }
  }

  if (packet_send(session) != SSH_OK) {
    leave_function();
    return rc;
  }

  ssh_log(session, SSH_LOG_RARE,
      "Sent a SSH_MSG_CHANNEL_REQUEST %s", request);
  if (reply == 0) {
    leave_function();
    return SSH_OK;
  }

  rc = packet_wait(session, SSH2_MSG_CHANNEL_SUCCESS, 1);
  if (rc == SSH_ERROR) {
    if (session->in_packet.type == SSH2_MSG_CHANNEL_FAILURE) {
      ssh_log(session, SSH_LOG_PACKET,
          "%s channel request failed", request);
      ssh_set_error(session, SSH_REQUEST_DENIED,
          "Channel request %s failed", request);
    } else {
      ssh_log(session, SSH_LOG_RARE,
          "Received an unexpected %d message", session->in_packet.type);
    }
  } else {
    ssh_log(session, SSH_LOG_RARE, "Received a SUCCESS");
  }

  leave_function();
  return rc;
error:
  buffer_reinit(session->out_buffer);
  string_free(req);

  leave_function();
  return rc;
}

/**
 * @brief Request a pty with a specific type and size.
 *
 * @param channel       The channel to sent the request.
 *
 * @param terminal      The terminal type ("vt100, xterm,...").
 *
 * @param col           The number of columns.
 *
 * @param row           The number of rows.
 *
 * @return SSH_SUCCESS on success, SSH_ERROR on error.
 */
int channel_request_pty_size(ssh_channel channel, const char *terminal,
    int col, int row) {
  ssh_session session = channel->session;
  ssh_string term = NULL;
  ssh_buffer buffer = NULL;
  int rc = SSH_ERROR;

  enter_function();
#ifdef WITH_SSH1
  if (channel->version==1) {
    channel_request_pty_size1(channel,terminal, col, row);
    leave_function();
    return rc;
    }
#endif
  buffer = buffer_new();
  if (buffer == NULL) {
    goto error;
  }

  term = string_from_char(terminal);
  if (term == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(buffer, term) < 0 ||
      buffer_add_u32(buffer, htonl(col)) < 0 ||
      buffer_add_u32(buffer, htonl(row)) < 0 ||
      buffer_add_u32(buffer, 0) < 0 ||
      buffer_add_u32(buffer, 0) < 0 ||
      buffer_add_u32(buffer, htonl(1)) < 0 || /* Add a 0byte string */
      buffer_add_u8(buffer, 0) < 0) {
    goto error;
  }

  rc = channel_request(channel, "pty-req", buffer, 1);
error:
  buffer_free(buffer);
  string_free(term);

  leave_function();
  return rc;
}

/**
 * @brief Request a PTY.
 *
 * @param channel       The channel to send the request.
 *
 * @return SSH_SUCCESS on success, SSH_ERROR on error.
 *
 * @see channel_request_pty_size()
 */
int channel_request_pty(ssh_channel channel) {
  return channel_request_pty_size(channel, "xterm", 80, 24);
}

/**
 * @brief Change the size of the terminal associated to a channel.
 *
 * @param channel       The channel to change the size.
 *
 * @param cols          The new number of columns.
 *
 * @param rows          The new number of rows.
 *
 * @warning Do not call it from a signal handler if you are not
 * sure any other libssh function using the same channel/session
 * is running at same time (not 100% threadsafe).
 */
int channel_change_pty_size(ssh_channel channel, int cols, int rows) {
  ssh_session session = channel->session;
  ssh_buffer buffer = NULL;
  int rc = SSH_ERROR;

  enter_function();

#ifdef WITH_SSH1
  if (channel->version == 1) {
    rc = channel_change_pty_size1(channel,cols,rows);
    leave_function();
    return rc;
  }
#endif

  buffer = buffer_new();
  if (buffer == NULL) {
    goto error;
  }

  if (buffer_add_u32(buffer, htonl(cols)) < 0 ||
      buffer_add_u32(buffer, htonl(rows)) < 0 ||
      buffer_add_u32(buffer, 0) < 0 ||
      buffer_add_u32(buffer, 0) < 0) {
    goto error;
  }

  rc = channel_request(channel, "window-change", buffer, 0);
error:
  buffer_free(buffer);

  leave_function();
  return rc;
}

/**
 * @brief Request a shell.
 *
 * @param channel      The channel to send the request.
 *
 * @returns SSH_SUCCESS on success, SSH_ERROR on error.
 */
int channel_request_shell(ssh_channel channel) {
#ifdef WITH_SSH1
  if (channel->version == 1) {
    return channel_request_shell1(channel);
  }
#endif
  return channel_request(channel, "shell", NULL, 1);
}

/**
 * @brief Request a subsystem (for example "sftp").
 *
 * @param channel       The channel to send the request.
 *
 * @param subsys        The subsystem to request (for example "sftp").
 *
 * @return SSH_SUCCESS on success, SSH_ERROR on error.
 *
 * @warning You normally don't have to call it for sftp, see sftp_new().
 */
int channel_request_subsystem(ssh_channel channel, const char *subsys) {
  ssh_buffer buffer = NULL;
  ssh_string subsystem = NULL;
  int rc = SSH_ERROR;

  buffer = buffer_new();
  if (buffer == NULL) {
    goto error;
  }

  subsystem = string_from_char(subsys);
  if (subsystem == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(buffer, subsystem) < 0) {
    goto error;
  }

  rc = channel_request(channel, "subsystem", buffer, 1);
error:
  buffer_free(buffer);
  string_free(subsystem);

  return rc;
}

int channel_request_sftp( ssh_channel channel){
    return channel_request_subsystem(channel, "sftp");
}

static ssh_string generate_cookie(void) {
  static const char *hex = "0123456789abcdef";
  char s[36];
  int i;

  srand ((unsigned int)time(NULL));
  for (i = 0; i < 32; i++) {
    s[i] = hex[rand() % 16];
  }
  s[32] = '\0';
  return string_from_char(s);
}

/**
 * @brief Sends the "x11-req" channel request over an existing session channel.
 *
 * This will enable redirecting the display of the remote X11 applications to
 * local X server over an secure tunnel.
 *
 * @param channel       An existing session channel where the remote X11
 *                      applications are going to be executed.
 *
 * @param single_connection    A boolean to mark only one X11 app will be
 *                             redirected.
 *
 * @param protocol      x11 authentication protocol. Pass NULL to use the
 *                      default value MIT-MAGIC-COOKIE-1
 *
 * @param cookie        x11 authentication cookie. Pass NULL to generate
 *                      a random cookie.
 *
 * @param screen_number        Screen number.
 *
 * @return SSH_OK on success\n
 *         SSH_ERROR on error
 */
int channel_request_x11(ssh_channel channel, int single_connection, const char *protocol,
    const char *cookie, int screen_number) {
  ssh_buffer buffer = NULL;
  ssh_string p = NULL;
  ssh_string c = NULL;
  int rc = SSH_ERROR;

  buffer = buffer_new();
  if (buffer == NULL) {
    goto error;
  }

  p = string_from_char(protocol ? protocol : "MIT-MAGIC-COOKIE-1");
  if (p == NULL) {
    goto error;
  }

  if (cookie) {
    c = string_from_char(cookie);
  } else {
    c = generate_cookie();
  }
  if (c == NULL) {
    goto error;
  }

  if (buffer_add_u8(buffer, single_connection == 0 ? 0 : 1) < 0 ||
      buffer_add_ssh_string(buffer, p) < 0 ||
      buffer_add_ssh_string(buffer, c) < 0 ||
      buffer_add_u32(buffer, htonl(screen_number)) < 0) {
    goto error;
  }

  rc = channel_request(channel, "x11-req", buffer, 1);

error:
  buffer_free(buffer);
  string_free(p);
  string_free(c);
  return rc;
}

static ssh_channel channel_accept(ssh_session session, int channeltype,
    int timeout_ms) {
#ifndef _WIN32
  static const struct timespec ts = {
    .tv_sec = 0,
    .tv_nsec = 50000000 /* 50ms */
  };
#endif
  ssh_message msg = NULL;
  ssh_channel channel = NULL;
  struct ssh_iterator *iterator;
  int t;

  for (t = timeout_ms; t >= 0; t -= 50)
  {
    ssh_handle_packets(session);

    if (session->ssh_message_list) {
      iterator = ssh_list_get_iterator(session->ssh_message_list);
      while (iterator) {
        msg = (ssh_message)iterator->data;
        if (ssh_message_type(msg) == SSH_REQUEST_CHANNEL_OPEN &&
            ssh_message_subtype(msg) == channeltype) {
          ssh_list_remove(session->ssh_message_list, iterator);
          channel = ssh_message_channel_request_open_reply_accept(msg);
          ssh_message_free(msg);
          return channel;
        }
        iterator = iterator->next;
      }
    }
#ifdef _WIN32
    Sleep(50); /* 50ms */
#else
    nanosleep(&ts, NULL);
#endif
  }

  return NULL;
}

/**
 * @brief Accept an X11 forwarding channel.
 *
 * @param channel       An x11-enabled session channel.
 *
 * @param timeout_ms    Timeout in milli-seconds.
 *
 * @return Newly created channel, or NULL if no X11 request from the server
 */
ssh_channel channel_accept_x11(ssh_channel channel, int timeout_ms) {
  return channel_accept(channel->session, SSH_CHANNEL_X11, timeout_ms);
}

static int global_request(ssh_session session, const char *request,
    ssh_buffer buffer, int reply) {
  ssh_string req = NULL;
  int rc = SSH_ERROR;

  enter_function();

  req = string_from_char(request);
  if (req == NULL) {
    goto error;
  }

  if (buffer_add_u8(session->out_buffer, SSH2_MSG_GLOBAL_REQUEST) < 0 ||
      buffer_add_ssh_string(session->out_buffer, req) < 0 ||
      buffer_add_u8(session->out_buffer, reply == 0 ? 0 : 1) < 0) {
    goto error;
  }
  string_free(req);

  if (buffer != NULL) {
    if (buffer_add_data(session->out_buffer, buffer_get(buffer),
        buffer_get_len(buffer)) < 0) {
      goto error;
    }
  }

  if (packet_send(session) != SSH_OK) {
    leave_function();
    return rc;
  }

  ssh_log(session, SSH_LOG_RARE,
      "Sent a SSH_MSG_GLOBAL_REQUEST %s", request);
  if (reply == 0) {
    leave_function();
    return SSH_OK;
  }

  rc = packet_wait(session, SSH2_MSG_REQUEST_SUCCESS, 1);
  if (rc == SSH_ERROR) {
    if (session->in_packet.type == SSH2_MSG_REQUEST_FAILURE) {
      ssh_log(session, SSH_LOG_PACKET,
          "%s channel request failed", request);
      ssh_set_error(session, SSH_REQUEST_DENIED,
          "Channel request %s failed", request);
    } else {
      ssh_log(session, SSH_LOG_RARE,
          "Received an unexpected %d message", session->in_packet.type);
    }
  } else {
    ssh_log(session, SSH_LOG_RARE, "Received a SUCCESS");
  }

  leave_function();
  return rc;
error:
  buffer_reinit(session->out_buffer);
  string_free(req);

  leave_function();
  return rc;
}

/**
 * @brief Sends the "tcpip-forward" global request to ask the server to begin
 *        listening for inbound connections.
 *
 * @param session       The ssh session to send the request.
 *
 * @param address       The address to bind to on the server. Pass NULL to bind
 *                      to all available addresses on all protocol families
 *                      supported by the server.
 *
 * @param port          The port to bind to on the server. Pass 0 to ask the
 *                      server to allocate the next available unprivileged port
 *                      number
 *
 * @param bound_port    The pointer to get actual bound port. Pass NULL to
 *                      ignore.
 *
 * @return SSH_OK on success\n
 *         SSH_ERROR on error
 */
int channel_forward_listen(ssh_session session, const char *address, int port, int *bound_port) {
  ssh_buffer buffer = NULL;
  ssh_string addr = NULL;
  int rc = SSH_ERROR;
  uint32_t tmp;

  buffer = buffer_new();
  if (buffer == NULL) {
    goto error;
  }

  addr = string_from_char(address ? address : "");
  if (addr == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(buffer, addr) < 0 ||
      buffer_add_u32(buffer, htonl(port)) < 0) {
    goto error;
  }

  rc = global_request(session, "tcpip-forward", buffer, 1);

  if (rc == SSH_OK && port == 0 && bound_port) {
    buffer_get_u32(session->in_buffer, &tmp);
    *bound_port = ntohl(tmp);
  }

error:
  buffer_free(buffer);
  string_free(addr);
  return rc;
}

/**
 * @brief Accept an incoming TCP/IP forwarding channel.
 *
 * @param session       The ssh session to use.
 *
 * @param timeout_ms    Timeout in milli-seconds.
 *
 * @return Newly created channel, or NULL if no incoming channel request from
 *         the server
 */
ssh_channel channel_forward_accept(ssh_session session, int timeout_ms) {
  return channel_accept(session, SSH_CHANNEL_FORWARDED_TCPIP, timeout_ms);
}

/**
 * @brief Sends the "cancel-tcpip-forward" global request to ask the server to
 *        cancel the tcpip-forward request.
 *
 * @param session       The ssh session to send the request.
 *
 * @param address       The bound address on the server.
 *
 * @param port          The bound port on the server.
 *
 * @return SSH_OK on success\n
 *         SSH_ERROR on error
 */
int channel_forward_cancel(ssh_session session, const char *address, int port) {
  ssh_buffer buffer = NULL;
  ssh_string addr = NULL;
  int rc = SSH_ERROR;

  buffer = buffer_new();
  if (buffer == NULL) {
    goto error;
  }

  addr = string_from_char(address ? address : "");
  if (addr == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(buffer, addr) < 0 ||
      buffer_add_u32(buffer, htonl(port)) < 0) {
    goto error;
  }

  rc = global_request(session, "cancel-tcpip-forward", buffer, 1);

error:
  buffer_free(buffer);
  string_free(addr);
  return rc;
}

/**
 * @brief Set environement variables.
 *
 * @param channel       The channel to set the environement variables.
 *
 * @param name          The name of the variable.
 *
 * @param value         The value to set.
 *
 * @return SSH_SUCCESS on success, SSH_ERROR on error.
 *
 * @warning Some environement variables may be refused by security reasons.
 * */
int channel_request_env(ssh_channel channel, const char *name, const char *value) {
  ssh_buffer buffer = NULL;
  ssh_string str = NULL;
  int rc = SSH_ERROR;

  buffer = buffer_new();
  if (buffer == NULL) {
    goto error;
  }

  str = string_from_char(name);
  if (str == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(buffer, str) < 0) {
    goto error;
  }

  string_free(str);
  str = string_from_char(value);
  if (str == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(buffer, str) < 0) {
    goto error;
  }

  rc = channel_request(channel, "env", buffer,1);
error:
  buffer_free(buffer);
  string_free(str);

  return rc;
}

/**
 * @brief Run a shell command without an interactive shell.
 *
 * This is similar to 'sh -c command'.
 *
 * @param channel       The channel to execute the command.
 *
 * @param cmd           The command to execute
 *                      (e.g. "ls ~/ -al | grep -i reports").
 *
 * @return SSH_SUCCESS on success, SSH_ERROR on error.
 *
 * @see channel_request_shell()
 */
int channel_request_exec(ssh_channel channel, const char *cmd) {
  ssh_buffer buffer = NULL;
  ssh_string command = NULL;
  int rc = SSH_ERROR;

#ifdef WITH_SSH1
  if (channel->version == 1) {
    return channel_request_exec1(channel, cmd);
  }
#endif

  buffer = buffer_new();
  if (buffer == NULL) {
    goto error;
  }

  command = string_from_char(cmd);
  if (command == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(buffer, command) < 0) {
    goto error;
  }

  rc = channel_request(channel, "exec", buffer, 1);
error:
  buffer_free(buffer);
  string_free(command);
  return rc;
}


/**
 * @brief Send a signal to remote process (as described in RFC 4254, section 6.9).
 *
 * Sends a signal 'signal' to the remote process.
 * Note, that remote system may not support signals concept.
 * In such a case this request will be silently ignored.
 * Only SSH-v2 is supported (I'm not sure about SSH-v1).
 *
 * @param channel       The channel to send signal.
 *
 * @param signal        The signal to send (without SIG prefix)
 *                      (e.g. "TERM" or "KILL").
 *
 * @return SSH_SUCCESS on success, SSH_ERROR on error (including attempt to send signal via SSH-v1 session).
 *
 */
int channel_request_send_signal(ssh_channel channel, const char *signal) {
  ssh_buffer buffer = NULL;
  ssh_string encoded_signal = NULL;
  int rc = SSH_ERROR;

#ifdef WITH_SSH1
  if (channel->version == 1) {
    return SSH_ERROR; // TODO: Add support for SSH-v1 if possible.
  }
#endif

  buffer = buffer_new();
  if (buffer == NULL) {
    goto error;
  }

  encoded_signal = string_from_char(signal);
  if (encoded_signal == NULL) {
    goto error;
  }

  if (buffer_add_ssh_string(buffer, encoded_signal) < 0) {
    goto error;
  }

  rc = channel_request(channel, "signal", buffer, 0);
error:
  buffer_free(buffer);
  string_free(encoded_signal);
  return rc;
}



/* TODO : fix the delayed close thing */
/* TODO : fix the blocking behaviours */

/**
 * @brief Read data from a channel into a buffer.
 *
 * @param channel       The channel to read from.
 *
 * @param buffer        The buffer which will get the data.
 *
 * @param count         The count of bytes to be read. If it is biggerthan 0,
 *                      the exact size will be read, else (bytes=0) it will
 *                      return once anything is available.
 *
 * @param is_stderr     A boolean value to mark reading from the stderr stream.
 *
 * @return The number of bytes read, 0 on end of file or SSH_ERROR on error.
 */
int channel_read_buffer(ssh_channel channel, ssh_buffer buffer, uint32_t count,
    int is_stderr) {
  ssh_session session=channel->session;
  ssh_buffer stdbuf = channel->stdout_buffer;
  uint32_t maxread = count;
  uint32_t len;

  buffer_reinit(buffer);

  enter_function();

  if (count == 0) {
    maxread = MAX_PACKET_LEN;
  }

  if (is_stderr) {
    stdbuf = channel->stderr_buffer;
  }

  /*
   * We may have problem if the window is too small to accept as much data
   * as asked
   */
  ssh_log(session, SSH_LOG_PROTOCOL,
      "Read (%d) buffered: %d bytes. Window: %d",
      count,
      buffer_get_rest_len(stdbuf),
      channel->local_window);

  if (count > buffer_get_rest_len(stdbuf) + channel->local_window) {
    if (grow_window(session, channel,
          count - buffer_get_rest_len(stdbuf)) < 0) {
      leave_function();
      return -1;
    }
  }
  /* block reading if asked bytes=0 */
  while (buffer_get_rest_len(stdbuf) == 0 ||
      buffer_get_rest_len(stdbuf) < count) {
    if (channel->remote_eof && buffer_get_rest_len(stdbuf) == 0) {
      leave_function();
      return 0;
    }
    if (channel->remote_eof) {
      /* Return the resting bytes in buffer */
      break;
    }
    if (buffer_get_rest_len(stdbuf) >= maxread) {
      /* Stop reading when buffer is full enough */
      break;
    }

    if ((packet_read(session)) != SSH_OK ||
        (packet_translate(session) != SSH_OK)) {
      leave_function();
      return -1;
    }
    packet_parse(session);
  }

  if(channel->local_window < WINDOWLIMIT) {
    if (grow_window(session, channel, 0) < 0) {
      leave_function();
      return -1;
    }
  }

  if (count == 0) {
    /* write the ful buffer information */
    if (buffer_add_data(buffer, buffer_get_rest(stdbuf),
          buffer_get_rest_len(stdbuf)) < 0) {
      leave_function();
      return -1;
    }
    buffer_reinit(stdbuf);
  } else {
    /* Read bytes bytes if len is greater, everything otherwise */
    len = buffer_get_rest_len(stdbuf);
    len = (len > count ? count : len);
    if (buffer_add_data(buffer, buffer_get_rest(stdbuf), len) < 0) {
      leave_function();
      return -1;
    }
    buffer_pass_bytes(stdbuf,len);
  }

  leave_function();
  return buffer_get_len(buffer);
}

/* TODO FIXME Fix the delayed close thing */
/* TODO FIXME Fix the blocking behaviours */

/**
 * @brief Reads data from a channel.
 *
 * @param channel       The channel to read from.
 *
 * @param dest          The destination buffer which will get the data.
 *
 * @param count         The count of bytes to be read.
 *
 * @param is_stderr     A boolean value to mark reading from the stderr flow.
 *
 * @return The number of bytes read, 0 on end of file or SSH_ERROR on error.
 * @warning This function may return less than count bytes of data, and won't
 *          block until count bytes have been read.
 * @warning The read function using a buffer has been renamed to
 *          channel_read_buffer().
 */
int channel_read(ssh_channel channel, void *dest, uint32_t count, int is_stderr) {
  ssh_session session = channel->session;
  ssh_buffer stdbuf = channel->stdout_buffer;
  uint32_t len;

  enter_function();

  if (count == 0) {
    leave_function();
    return 0;
  }

  if (is_stderr) {
    stdbuf=channel->stderr_buffer;
  }

  /*
   * We may have problem if the window is too small to accept as much data
   * as asked
   */
  ssh_log(session, SSH_LOG_PROTOCOL,
      "Read (%d) buffered : %d bytes. Window: %d",
      count,
      buffer_get_rest_len(stdbuf),
      channel->local_window);

  if (count > buffer_get_rest_len(stdbuf) + channel->local_window) {
    if (grow_window(session, channel,
          count - buffer_get_rest_len(stdbuf)) < 0) {
      leave_function();
      return -1;
    }
  }

  /* block reading until at least one byte is read 
  *  and ignore the trivial case count=0
  */
  while (buffer_get_rest_len(stdbuf) == 0 && count > 0) {
    if (channel->remote_eof && buffer_get_rest_len(stdbuf) == 0) {
      leave_function();
      return 0;
    }

    if (channel->remote_eof) {
      /* Return the resting bytes in buffer */
      break;
    }

    if (buffer_get_rest_len(stdbuf) >= count) {
      /* Stop reading when buffer is full enough */
      break;
    }

    if ((packet_read(session)) != SSH_OK ||
        (packet_translate(session) != SSH_OK)) {
      leave_function();
      return -1;
    }
    packet_parse(session);
  }

  if (channel->local_window < WINDOWLIMIT) {
    if (grow_window(session, channel, 0) < 0) {
      leave_function();
      return -1;
    }
  }

  len = buffer_get_rest_len(stdbuf);
  /* Read count bytes if len is greater, everything otherwise */
  len = (len > count ? count : len);
  memcpy(dest, buffer_get_rest(stdbuf), len);
  buffer_pass_bytes(stdbuf,len);

  leave_function();
  return len;
}

/**
 * @brief Do a nonblocking read on the channel.
 *
 * A nonblocking read on the specified channel. it will return <= count bytes of
 * data read atomicly.
 *
 * @param channel       The channel to read from.
 *
 * @param dest          A pointer to a destination buffer.
 *
 * @param count         The count of bytes of data to be read.
 *
 * @param is_stderr     A boolean to select the stderr stream.
 *
 * @return The number of bytes read, 0 if nothing is available or
 *         SSH_ERROR on error.
 *
 * @warning Don't forget to check for EOF as it would return 0 here.
 *
 * @see channel_is_eof()
 */
int channel_read_nonblocking(ssh_channel channel, void *dest, uint32_t count,
    int is_stderr) {
  ssh_session session = channel->session;
  uint32_t to_read;
  int rc;

  enter_function();

  to_read = channel_poll(channel, is_stderr);

  if (to_read <= 0) {
    leave_function();
    return to_read; /* may be an error code */
  }

  if (to_read > count) {
    to_read = count;
  }
  rc = channel_read(channel, dest, to_read, is_stderr);

  leave_function();
  return rc;
}

/**
 * @brief Polls a channel for data to read.
 *
 * @param channel       The channel to poll.
 *
 * @param is_stderr     A boolean to select the stderr stream.
 *
 * @return The number of bytes available for reading, 0 if nothing is available
 *         or SSH_ERROR on error.
 *
 * @warning When the channel is in EOF state, the function returns SSH_EOF.
 *
 * @see channel_is_eof()
 */
int channel_poll(ssh_channel channel, int is_stderr){
  ssh_session session = channel->session;
  ssh_buffer stdbuf = channel->stdout_buffer;

  enter_function();

  if (is_stderr) {
    stdbuf = channel->stderr_buffer;
  }

  if (buffer_get_rest_len(stdbuf) == 0 && channel->remote_eof == 0) {
    if (ssh_handle_packets(channel->session) == SSH_ERROR) {
      leave_function();
      return SSH_ERROR;
    }
  }

  if (buffer_get_rest_len(stdbuf) > 0)
    return buffer_get_rest_len(stdbuf);

  if (channel->remote_eof) {
    leave_function();
    return SSH_EOF;
  }

  leave_function();
  return buffer_get_rest_len(stdbuf);
}

/**
 * @brief Recover the session in which belongs a channel.
 *
 * @param channel       The channel to recover the session from.
 *
 * @return The session pointer.
 */
ssh_session channel_get_session(ssh_channel channel) {
  return channel->session;
}

/**
 * @brief Get the exit status of the channel (error code from the executed
 *        instruction).
 *
 * @param channel       The channel to get the status from.
 *
 * @return -1 if no exit status has been returned or eof not sent,
 *         the exit status othewise.
 */
int channel_get_exit_status(ssh_channel channel) {
  if (channel->local_eof == 0) {
    return -1;
  }

  while (channel->remote_eof == 0 || channel->exit_status == -1) {
    /* Parse every incoming packet */
    if (packet_wait(channel->session, 0, 0) != SSH_OK) {
      return -1;
    }
    if (channel->open == 0) {
      /* When a channel is closed, no exit status message can
       * come anymore */
      break;
    }
  }

  return channel->exit_status;
}

/*
 * This function acts as a meta select.
 *
 * First, channels are analyzed to seek potential can-write or can-read ones,
 * then if no channel has been elected, it goes in a loop with the posix
 * select(2).
 * This is made in two parts: protocol select and network select. The protocol
 * select does not use the network functions at all
 */
static int channel_protocol_select(ssh_channel *rchans, ssh_channel *wchans,
    ssh_channel *echans, ssh_channel *rout, ssh_channel *wout, ssh_channel *eout) {
  ssh_channel chan;
  int i;
  int j = 0;

  for (i = 0; rchans[i] != NULL; i++) {
    chan = rchans[i];

    while (chan->open && ssh_socket_data_available(chan->session->socket)) {
      ssh_handle_packets(chan->session);
    }

    if ((chan->stdout_buffer && buffer_get_len(chan->stdout_buffer) > 0) ||
        (chan->stderr_buffer && buffer_get_len(chan->stderr_buffer) > 0) ||
        chan->remote_eof) {
      rout[j] = chan;
      j++;
    }
  }
  rout[j] = NULL;

  j = 0;
  for(i = 0; wchans[i] != NULL; i++) {
    chan = wchans[i];
    /* It's not our business to seek if the file descriptor is writable */
    if (ssh_socket_data_writable(chan->session->socket) &&
        chan->open && (chan->remote_window > 0)) {
      wout[j] = chan;
      j++;
    }
  }
  wout[j] = NULL;

  j = 0;
  for (i = 0; echans[i] != NULL; i++) {
    chan = echans[i];

    if (!ssh_socket_is_open(chan->session->socket) || !chan->open) {
      eout[j] = chan;
      j++;
    }
  }
  eout[j] = NULL;

  return 0;
}

/* Just count number of pointers in the array */
static int count_ptrs(ssh_channel *ptrs) {
  int c;
  for (c = 0; ptrs[c] != NULL; c++)
    ;

  return c;
}

/**
 * @brief Act like the standard select(2) on channels.
 *
 * The list of pointers are then actualized and will only contain pointers to
 * channels that are respectively readable, writable or have an exception to
 * trap.
 *
 * @param readchans     A NULL pointer or an array of channel pointers,
 *                      terminated by a NULL.
 *
 * @param writechans    A NULL pointer or an array of channel pointers,
 *                      terminated by a NULL.
 *
 * @param exceptchans   A NULL pointer or an array of channel pointers,
 *                      terminated by a NULL.
 *
 * @param timeout       Timeout as defined by select(2).
 *
 * @return SSH_SUCCESS operation successful\n
 *         SSH_EINTR select(2) syscall was interrupted, relaunch the function
 */
int channel_select(ssh_channel *readchans, ssh_channel *writechans,
    ssh_channel *exceptchans, struct timeval * timeout) {
  ssh_channel *rchans, *wchans, *echans;
  ssh_channel dummy = NULL;
  fd_set rset;
  fd_set wset;
  fd_set eset;
  socket_t max_fd = SSH_INVALID_SOCKET;
  int rc;
  int i;

  /* don't allow NULL pointers */
  if (readchans == NULL) {
    readchans = &dummy;
  }

  if (writechans == NULL) {
    writechans = &dummy;
  }

  if (exceptchans == NULL) {
    exceptchans = &dummy;
  }

  if (readchans[0] == NULL && writechans[0] == NULL && exceptchans[0] == NULL) {
    /* No channel to poll?? Go away! */
    return 0;
  }

  /* Prepare the outgoing temporary arrays */
  rchans = malloc(sizeof(ssh_channel ) * (count_ptrs(readchans) + 1));
  if (rchans == NULL) {
    return SSH_ERROR;
  }

  wchans = malloc(sizeof(ssh_channel ) * (count_ptrs(writechans) + 1));
  if (wchans == NULL) {
    SAFE_FREE(rchans);
    return SSH_ERROR;
  }

  echans = malloc(sizeof(ssh_channel ) * (count_ptrs(exceptchans) + 1));
  if (echans == NULL) {
    SAFE_FREE(rchans);
    SAFE_FREE(wchans);
    return SSH_ERROR;
  }

  /*
   * First, try without doing network stuff then, select and redo the
   * networkless stuff
   */
  do {
    channel_protocol_select(readchans, writechans, exceptchans,
        rchans, wchans, echans);
    if (rchans[0] != NULL || wchans[0] != NULL || echans[0] != NULL) {
      /* We've got one without doing any select overwrite the begining arrays */
      memcpy(readchans, rchans, (count_ptrs(rchans) + 1) * sizeof(ssh_channel ));
      memcpy(writechans, wchans, (count_ptrs(wchans) + 1) * sizeof(ssh_channel ));
      memcpy(exceptchans, echans, (count_ptrs(echans) + 1) * sizeof(ssh_channel ));
      SAFE_FREE(rchans);
      SAFE_FREE(wchans);
      SAFE_FREE(echans);
      return 0;
    }
    /*
     * Since we verified the invalid fd cases into the networkless select,
     * we can be sure all fd are valid ones
     */
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_ZERO(&eset);

    for (i = 0; readchans[i] != NULL; i++) {
      if (!ssh_socket_fd_isset(readchans[i]->session->socket, &rset)) {
        ssh_socket_fd_set(readchans[i]->session->socket, &rset, &max_fd);
      }
    }

    for (i = 0; writechans[i] != NULL; i++) {
      if (!ssh_socket_fd_isset(writechans[i]->session->socket, &wset)) {
        ssh_socket_fd_set(writechans[i]->session->socket, &wset, &max_fd);
      }
    }

    for (i = 0; exceptchans[i] != NULL; i++) {
      if (!ssh_socket_fd_isset(exceptchans[i]->session->socket, &eset)) {
        ssh_socket_fd_set(exceptchans[i]->session->socket, &eset, &max_fd);
      }
    }

    /* Here we go */
    rc = select(max_fd, &rset, &wset, &eset, timeout);
    /* Leave if select was interrupted */
    if (rc == EINTR) {
      SAFE_FREE(rchans);
      SAFE_FREE(wchans);
      SAFE_FREE(echans);
      return SSH_EINTR;
    }

    for (i = 0; readchans[i] != NULL; i++) {
      if (ssh_socket_fd_isset(readchans[i]->session->socket, &rset)) {
        ssh_socket_set_toread(readchans[i]->session->socket);
      }
    }

    for (i = 0; writechans[i] != NULL; i++) {
      if (ssh_socket_fd_isset(writechans[i]->session->socket, &wset)) {
        ssh_socket_set_towrite(writechans[i]->session->socket);
      }
    }

    for (i = 0; exceptchans[i] != NULL; i++) {
      if (ssh_socket_fd_isset(exceptchans[i]->session->socket, &eset)) {
        ssh_socket_set_except(exceptchans[i]->session->socket);
      }
    }
  } while(1); /* Return to do loop */

  /* not reached */
  return 0;
}

/** @} */
/* vim: set ts=2 sw=2 et cindent: */
