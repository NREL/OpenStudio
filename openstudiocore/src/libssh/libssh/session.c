/*
 * session.c - non-networking functions
 *
 * This file is part of the SSH Library
 *
 * Copyright (c) 2005-2008 by Aris Adamantiadis
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
#include <string.h>
#include <stdlib.h>
#include "libssh/libssh.h"
#include "libssh/priv.h"
#include "libssh/server.h"
#include "libssh/socket.h"
#include "libssh/agent.h"
#include "libssh/packet.h"
#include "libssh/session.h"
#include "libssh/misc.h"
#include "libssh/ssh2.h"
#include "libssh/buffer.h"
#define FIRST_CHANNEL 42 // why not ? it helps to find bugs.

/** \defgroup ssh_session SSH Session
 * \brief functions that manage a session
 */
/** \addtogroup ssh_session
 * @{ */

/** \brief creates a new ssh session
 * \returns new ssh_session pointer
 */
ssh_session ssh_new(void) {
  ssh_session session;
  char *id;
  int rc;

  session = malloc(sizeof (struct ssh_session_struct));
  if (session == NULL) {
    return NULL;
  }
  ZERO_STRUCTP(session);

  session->next_crypto = crypto_new();
  if (session->next_crypto == NULL) {
    goto err;
  }

  session->socket = ssh_socket_new(session);
  if (session->socket == NULL) {
    goto err;
  }

  session->out_buffer = buffer_new();
  if (session->out_buffer == NULL) {
    goto err;
  }

  session->in_buffer=buffer_new();
  if (session->in_buffer == NULL) {
    goto err;
  }

  session->alive = 0;
  session->auth_methods = 0;
  session->blocking = 1;
  session->log_indent = 0;
  session->maxchannel = FIRST_CHANNEL;

  /* options */
  session->port = 22;
  session->fd = -1;
  session->ssh2 = 1;
#ifdef WITH_SSH1
  session->ssh1 = 1;
#else
  session->ssh1 = 0;
#endif

#ifndef _WIN32
    session->agent = agent_new(session);
    if (session->agent == NULL) {
      goto err;
    }
#endif /* _WIN32 */

    session->identity = ssh_list_new();
    if (session->identity == NULL) {
      goto err;
    }

    id = strdup("%d/id_rsa");
    if (id == NULL) {
      goto err;
    }
    rc = ssh_list_append(session->identity, id);
    if (rc == SSH_ERROR) {
      goto err;
    }

    id = strdup("%d/id_dsa");
    if (id == NULL) {
      goto err;
    }
    rc = ssh_list_append(session->identity, id);
    if (rc == SSH_ERROR) {
      goto err;
    }

    id = strdup("%d/identity");
    if (id == NULL) {
      goto err;
    }
    rc = ssh_list_append(session->identity, id);
    if (rc == SSH_ERROR) {
      goto err;
    }

    return session;

err:
    ssh_free(session);
    return NULL;
}

/**
 * @brief deallocate a session handle
 * @see ssh_disconnect()
 * @see ssh_new()
 */
void ssh_free(ssh_session session) {
  int i;
  enter_function();

  if (session == NULL) {
    return;
  }

  SAFE_FREE(session->serverbanner);
  SAFE_FREE(session->clientbanner);
  SAFE_FREE(session->banner);
#ifdef WITH_PCAP
  if(session->pcap_ctx){
  	ssh_pcap_context_free(session->pcap_ctx);
  	session->pcap_ctx=NULL;
  }
#endif
  buffer_free(session->in_buffer);
  buffer_free(session->out_buffer);
  session->in_buffer=session->out_buffer=NULL;
  crypto_free(session->current_crypto);
  crypto_free(session->next_crypto);
  ssh_socket_free(session->socket);
  /* delete all channels */
  while (session->channels) {
    channel_free(session->channels);
  }
#ifndef _WIN32
  agent_free(session->agent);
#endif /* _WIN32 */
  if (session->client_kex.methods) {
    for (i = 0; i < 10; i++) {
      SAFE_FREE(session->client_kex.methods[i]);
    }
  }

  if (session->server_kex.methods) {
    for (i = 0; i < 10; i++) {
      SAFE_FREE(session->server_kex.methods[i]);
    }
  }
  SAFE_FREE(session->client_kex.methods);
  SAFE_FREE(session->server_kex.methods);

  privatekey_free(session->dsa_key);
  privatekey_free(session->rsa_key);
  if(session->ssh_message_list){
    ssh_message msg;
    while((msg=ssh_list_pop_head(ssh_message ,session->ssh_message_list))
        != NULL){
      ssh_message_free(msg);
    }
    ssh_list_free(session->ssh_message_list);
  }

  if (session->identity) {
    char *id;

    for (id = ssh_list_pop_head(char *, session->identity);
         id != NULL;
         id = ssh_list_pop_head(char *, session->identity)) {
      SAFE_FREE(id);
    }
    ssh_list_free(session->identity);
  }

  /* options */
  SAFE_FREE(session->username);
  SAFE_FREE(session->host);
  SAFE_FREE(session->sshdir);
  SAFE_FREE(session->knownhosts);
  SAFE_FREE(session->ProxyCommand);

  for (i = 0; i < 10; i++) {
    if (session->wanted_methods[i]) {
      SAFE_FREE(session->wanted_methods[i]);
    }
  }

  /* burn connection, it could hang sensitive datas */
  ZERO_STRUCTP(session);
  SAFE_FREE(session);
}

/** \brief disconnect impolitely from remote host by closing the socket.
 * Suitable if you forked and want to destroy this session.
 * \param session current ssh session
 */
void ssh_silent_disconnect(ssh_session session) {
  enter_function();

  if (session == NULL) {
    return;
  }

  ssh_socket_close(session->socket);
  session->alive = 0;
  ssh_disconnect(session);
  leave_function();
}

/** \brief set the session in blocking/nonblocking mode
 * \param session ssh session
 * \param blocking zero for nonblocking mode
 * \bug nonblocking code is in development and won't work as expected
 */
void ssh_set_blocking(ssh_session session, int blocking) {
  if (session == NULL) {
    return;
  }

  session->blocking = blocking ? 1 : 0;
}

/** In case you'd need the file descriptor of the connection
 * to the server/client
 * \brief recover the fd of connection
 * \param session ssh session
 * \return file descriptor of the connection, or -1 if it is
 * not connected
 */
socket_t ssh_get_fd(ssh_session session) {
  if (session == NULL) {
    return -1;
  }

  return ssh_socket_get_fd(session->socket);
}

/** \brief say to the session it has data to read on the file descriptor without blocking
 * \param session ssh session
 */
void ssh_set_fd_toread(ssh_session session) {
  if (session == NULL) {
    return;
  }

  ssh_socket_set_toread(session->socket);
}

/** \brief say the session it may write to the file descriptor without blocking
 * \param session ssh session
 */
void ssh_set_fd_towrite(ssh_session session) {
  if (session == NULL) {
    return;
  }

  ssh_socket_set_towrite(session->socket);
}

/** \brief say the session it has an exception to catch on the file descriptor
 * \param session ssh session
 */
void ssh_set_fd_except(ssh_session session) {
  if (session == NULL) {
    return;
  }

  ssh_socket_set_except(session->socket);
}

/** \warning I don't remember if this should be internal or not
 */
/* looks if there is data to read on the socket and parse it. */
int ssh_handle_packets(ssh_session session) {
  int w = 0;
  int e = 0;
  int rc = -1;

  enter_function();

  do {
    rc = ssh_socket_poll(session->socket, &w, &e);
    if (rc <= 0) {
      /* error or no data available */
      leave_function();
      return rc;
    }

    /* if an exception happened, it will be trapped by packet_read() */
    if ((packet_read(session) != SSH_OK) ||
        (packet_translate(session) != SSH_OK)) {
      leave_function();
      return -1;
    }

    packet_parse(session);
  } while(rc > 0);

  leave_function();
  return rc;
}

/**
 * @brief Get session status
 *
 * @param session       The ssh session to use.
 *
 * @returns A bitmask including SSH_CLOSED, SSH_READ_PENDING or SSH_CLOSED_ERROR
 *          which respectively means the session is closed, has data to read on
 *          the connection socket and session was closed due to an error.
 */
int ssh_get_status(ssh_session session) {
  int socketstate;
  int r = 0;

  if (session == NULL) {
    return 0;
  }

  socketstate = ssh_socket_get_status(session->socket);

  if (session->closed) {
    r |= SSH_CLOSED;
  }
  if (socketstate & SSH_READ_PENDING) {
    r |= SSH_READ_PENDING;
  }
  if (session->closed && (socketstate & SSH_CLOSED_ERROR)) {
    r |= SSH_CLOSED_ERROR;
  }

  return r;
}

/** \brief get the disconnect message from the server
 * \param session ssh session
 * \return message sent by the server along with the disconnect, or NULL in which case the reason of the disconnect may be found with ssh_get_error.
 * \see ssh_get_error()
 */
const char *ssh_get_disconnect_message(ssh_session session) {
  if (session == NULL) {
    return NULL;
  }

  if (!session->closed) {
    ssh_set_error(session, SSH_REQUEST_DENIED,
        "Connection not closed yet");
  } else if(session->closed_by_except) {
    ssh_set_error(session, SSH_REQUEST_DENIED,
        "Connection closed by socket error");
  } else if(!session->discon_msg) {
    ssh_set_error(session, SSH_FATAL,
        "Connection correctly closed but no disconnect message");
  } else {
    return session->discon_msg;
  }

  return NULL;
}

/**
 * @brief Get the protocol version of the session.
 *
 * @param session       The ssh session to use.
 *
 * @return 1 or 2, for ssh1 or ssh2, < 0 on error.
 */
int ssh_get_version(ssh_session session) {
  if (session == NULL) {
    return -1;
  }

  return session->version;
}


/**
 * @internal
 * @handle a SSH_MSG_GLOBAL_REQUEST packet
 * @param session the SSH session
 */
void ssh_global_request_handle(ssh_session session){
  ssh_string type;
  char *type_c;
  uint32_t needreply;
  type=buffer_get_ssh_string(session->in_buffer);
  buffer_get_u32(session->in_buffer,&needreply);
  if(type==NULL)
    return;
  type_c=string_to_char(type);
  if(!type_c)
    return;
  ssh_log(session, SSH_LOG_PROTOCOL,
      "Received SSH_GLOBAL_REQUEST %s (wantreply=%d)",type_c,needreply);
  SAFE_FREE(type_c);
  string_free(type);
  if(needreply != 0){
    buffer_add_u8(session->out_buffer,SSH2_MSG_REQUEST_FAILURE);
    packet_send(session);
  }
}

/** @} */
/* vim: set ts=2 sw=2 et cindent: */
