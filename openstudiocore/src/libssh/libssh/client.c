/*
 * client.c - SSH client functions
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#include "libssh/priv.h"
#include "libssh/ssh2.h"
#include "libssh/buffer.h"
#include "libssh/packet.h"
#include "libssh/socket.h"
#include "libssh/session.h"
#include "libssh/dh.h"

#define set_status(session, status) do {\
        if (session->callbacks && session->callbacks->connect_status_function) \
            session->callbacks->connect_status_function(session->callbacks->userdata, status); \
    } while (0)

/**
 * @internal
 *
 * @brief Get a banner from a socket.
 *
 * The caller has to free memroy.
 *
 * @param  session      The session to get the banner from.
 *
 * @return A newly allocated string with the banner or NULL on error.
 */
char *ssh_get_banner(ssh_session session) {
  char buffer[128] = {0};
  char *str = NULL;
  int i;

  enter_function();

  for (i = 0; i < 127; i++) {
    if (ssh_socket_read(session->socket, &buffer[i], 1) != SSH_OK) {
      ssh_set_error(session, SSH_FATAL, "Remote host closed connection");
      leave_function();
      return NULL;
    }
#ifdef WITH_PCAP
    if(session->pcap_ctx && buffer[i] == '\n'){
    	ssh_pcap_context_write(session->pcap_ctx,SSH_PCAP_DIR_IN,buffer,i+1,i+1);
    }
#endif
    if (buffer[i] == '\r') {
      buffer[i] = '\0';
    }
    if (buffer[i] == '\n') {
      buffer[i] = '\0';
      str = strdup(buffer);
      if (str == NULL) {
        leave_function();
        return NULL;
      }
      leave_function();
      return str;
    }
  }

  ssh_set_error(session, SSH_FATAL, "Too large banner");

  leave_function();
  return NULL;
}

/**
 * @internal
 *
 * @brief Analyze the SSH banner to find out if we have a SSHv1 or SSHv2
 * server.
 *
 * @param  session      The session to analyze the banner from.
 * @param  ssh1         The variable which is set if it is a SSHv1 server.
 * @param  ssh2         The variable which is set if it is a SSHv2 server.
 *
 * @return 0 on success, < 0 on error.
 *
 * @see ssh_get_banner()
 */
static int ssh_analyze_banner(ssh_session session, int *ssh1, int *ssh2) {
  const char *banner = session->serverbanner;
  const char *openssh;

  ssh_log(session, SSH_LOG_RARE, "Analyzing banner: %s", banner);

  if (strncmp(banner, "SSH-", 4) != 0) {
    ssh_set_error(session, SSH_FATAL, "Protocol mismatch: %s", banner);
    return -1;
  }

  /*
   * Typical banners e.g. are:
   * SSH-1.5-blah
   * SSH-1.99-blah
   * SSH-2.0-blah
   */
  switch(banner[4]) {
    case '1':
      *ssh1 = 1;
      if (banner[6] == '9') {
        *ssh2 = 1;
      } else {
        *ssh2 = 0;
      }
      break;
    case '2':
      *ssh1 = 0;
      *ssh2 = 1;
      break;
    default:
      ssh_set_error(session, SSH_FATAL, "Protocol mismatch: %s", banner);
      return -1;
  }

  openssh = strstr(banner, "OpenSSH");
  if (openssh != NULL) {
    int major, minor;
    major = strtol(openssh + 8, (char **) NULL, 10);
    minor = strtol(openssh + 10, (char **) NULL, 10);
    session->openssh = SSH_VERSION_INT(major, minor, 0);
    ssh_log(session, SSH_LOG_RARE,
        "We are talking to an OpenSSH server version: %d.%d (%x)",
        major, minor, session->openssh);
  }

  return 0;
}

/** @internal
 * @brief Sends a SSH banner to the server.
 *
 * @param session      The SSH session to use.
 *
 * @param server       Send client or server banner.
 *
 * @return 0 on success, < 0 on error.
 */
int ssh_send_banner(ssh_session session, int server) {
  const char *banner = NULL;
  char buffer[128] = {0};

  enter_function();

  banner = session->version == 1 ? CLIENTBANNER1 : CLIENTBANNER2;

  if (session->xbanner) {
    banner = session->xbanner;
  }

  if (server) {
    session->serverbanner = strdup(banner);
    if (session->serverbanner == NULL) {
      leave_function();
      return -1;
    }
  } else {
    session->clientbanner = strdup(banner);
    if (session->clientbanner == NULL) {
      leave_function();
      return -1;
    }
  }

  snprintf(buffer, 128, "%s\r\n", banner);

  if (ssh_socket_write(session->socket, buffer, strlen(buffer)) == SSH_ERROR) {
    leave_function();
    return -1;
  }

  if (ssh_socket_blocking_flush(session->socket) != SSH_OK) {
    leave_function();
    return -1;
  }
#ifdef WITH_PCAP
  if(session->pcap_ctx)
  	ssh_pcap_context_write(session->pcap_ctx,SSH_PCAP_DIR_OUT,buffer,strlen(buffer),strlen(buffer));
#endif
  leave_function();
  return 0;
}

#define DH_STATE_INIT 0
#define DH_STATE_INIT_TO_SEND 1
#define DH_STATE_INIT_SENT 2
#define DH_STATE_NEWKEYS_TO_SEND 3
#define DH_STATE_NEWKEYS_SENT 4
#define DH_STATE_FINISHED 5
static int dh_handshake(ssh_session session) {
  ssh_string e = NULL;
  ssh_string f = NULL;
  ssh_string pubkey = NULL;
  ssh_string signature = NULL;
  int rc = SSH_ERROR;

  enter_function();

  switch (session->dh_handshake_state) {
    case DH_STATE_INIT:
      if (buffer_add_u8(session->out_buffer, SSH2_MSG_KEXDH_INIT) < 0) {
        goto error;
      }

      if (dh_generate_x(session) < 0) {
        goto error;
      }
      if (dh_generate_e(session) < 0) {
        goto error;
      }

      e = dh_get_e(session);
      if (e == NULL) {
        goto error;
      }

      if (buffer_add_ssh_string(session->out_buffer, e) < 0) {
        goto error;
      }
      string_burn(e);
      string_free(e);
      e=NULL;

      rc = packet_send(session);
      if (rc == SSH_ERROR) {
        goto error;
      }

      session->dh_handshake_state = DH_STATE_INIT_TO_SEND;
    case DH_STATE_INIT_TO_SEND:
      rc = packet_flush(session, 0);
      if (rc != SSH_OK) {
        goto error;
      }
      session->dh_handshake_state = DH_STATE_INIT_SENT;
    case DH_STATE_INIT_SENT:
      rc = packet_wait(session, SSH2_MSG_KEXDH_REPLY, 1);
      if (rc != SSH_OK) {
        goto error;
      }

      pubkey = buffer_get_ssh_string(session->in_buffer);
      if (pubkey == NULL){
        ssh_set_error(session,SSH_FATAL, "No public key in packet");
        rc = SSH_ERROR;
        goto error;
      }
      dh_import_pubkey(session, pubkey);

      f = buffer_get_ssh_string(session->in_buffer);
      if (f == NULL) {
        ssh_set_error(session,SSH_FATAL, "No F number in packet");
        rc = SSH_ERROR;
        goto error;
      }
      if (dh_import_f(session, f) < 0) {
        ssh_set_error(session, SSH_FATAL, "Cannot import f number");
        rc = SSH_ERROR;
        goto error;
      }
      string_burn(f);
      string_free(f);
      f=NULL;
      signature = buffer_get_ssh_string(session->in_buffer);
      if (signature == NULL) {
        ssh_set_error(session, SSH_FATAL, "No signature in packet");
        rc = SSH_ERROR;
        goto error;
      }
      session->dh_server_signature = signature;
      signature=NULL; /* ownership changed */
      if (dh_build_k(session) < 0) {
        ssh_set_error(session, SSH_FATAL, "Cannot build k number");
        rc = SSH_ERROR;
        goto error;
      }

      /* Send the MSG_NEWKEYS */
      if (buffer_add_u8(session->out_buffer, SSH2_MSG_NEWKEYS) < 0) {
        rc = SSH_ERROR;
        goto error;
      }

      rc = packet_send(session);
      if (rc == SSH_ERROR) {
        goto error;
      }

      session->dh_handshake_state = DH_STATE_NEWKEYS_TO_SEND;
    case DH_STATE_NEWKEYS_TO_SEND:
      rc = packet_flush(session, 0);
      if (rc != SSH_OK) {
        goto error;
      }
      ssh_log(session, SSH_LOG_RARE, "SSH_MSG_NEWKEYS sent\n");

      session->dh_handshake_state = DH_STATE_NEWKEYS_SENT;
    case DH_STATE_NEWKEYS_SENT:
      rc = packet_wait(session, SSH2_MSG_NEWKEYS, 1);
      if (rc != SSH_OK) {
        goto error;
      }
      ssh_log(session, SSH_LOG_RARE, "Got SSH_MSG_NEWKEYS\n");

      rc = make_sessionid(session);
      if (rc != SSH_OK) {
        goto error;
      }

      /*
       * Set the cryptographic functions for the next crypto
       * (it is needed for generate_session_keys for key lenghts)
       */
      if (crypt_set_algorithms(session)) {
        rc = SSH_ERROR;
        goto error;
      }

      if (generate_session_keys(session) < 0) {
        rc = SSH_ERROR;
        goto error;
      }

      /* Verify the host's signature. FIXME do it sooner */
      signature = session->dh_server_signature;
      session->dh_server_signature = NULL;
      if (signature_verify(session, signature)) {
        rc = SSH_ERROR;
        goto error;
      }

      /* forget it for now ... */
      string_burn(signature);
      string_free(signature);
      signature=NULL;
      /*
       * Once we got SSH2_MSG_NEWKEYS we can switch next_crypto and
       * current_crypto
       */
      if (session->current_crypto) {
        crypto_free(session->current_crypto);
        session->current_crypto=NULL;
      }

      /* FIXME later, include a function to change keys */
      session->current_crypto = session->next_crypto;

      session->next_crypto = crypto_new();
      if (session->next_crypto == NULL) {
        rc = SSH_ERROR;
        goto error;
      }

      session->dh_handshake_state = DH_STATE_FINISHED;

      leave_function();
      return SSH_OK;
    default:
      ssh_set_error(session, SSH_FATAL, "Invalid state in dh_handshake(): %d",
          session->dh_handshake_state);

      leave_function();
      return SSH_ERROR;
  }

  /* not reached */
error:
  if(e != NULL){
    string_burn(e);
    string_free(e);
  }
  if(f != NULL){
    string_burn(f);
    string_free(f);
  }
  if(signature != NULL){
    string_burn(signature);
    string_free(signature);
  }

  leave_function();
  return rc;
}

/**
 * @internal
 *
 * @brief Request a service from the SSH server.
 *
 * Service requests are for example: ssh-userauth, ssh-connection, etc.
 *
 * @param  session      The session to use to ask for a service request.
 * @param  service      The service request.
 *
 * @return 0 on success, < 0 on error.
 */
int ssh_service_request(ssh_session session, const char *service) {
  ssh_string service_s = NULL;

  enter_function();

  if (buffer_add_u8(session->out_buffer, SSH2_MSG_SERVICE_REQUEST) < 0) {
    leave_function();
    return -1;
  }

  service_s = string_from_char(service);
  if (service_s == NULL) {
    leave_function();
    return -1;
  }

  if (buffer_add_ssh_string(session->out_buffer,service_s) < 0) {
    string_free(service_s);
    leave_function();
    return -1;
  }
  string_free(service_s);

  if (packet_send(session) != SSH_OK) {
    ssh_set_error(session, SSH_FATAL,
        "Sending SSH2_MSG_SERVICE_REQUEST failed.");
    leave_function();
    return -1;
  }

  ssh_log(session, SSH_LOG_PACKET,
      "Sent SSH_MSG_SERVICE_REQUEST (service %s)", service);

  if (packet_wait(session,SSH2_MSG_SERVICE_ACCEPT,1) != SSH_OK) {
    ssh_set_error(session, SSH_FATAL, "Did not receive SERVICE_ACCEPT");
    leave_function();
    return -1;
  }

  ssh_log(session, SSH_LOG_PACKET,
      "Received SSH_MSG_SERVICE_ACCEPT (service %s)", service);

  leave_function();
  return 0;
}

/** \addtogroup ssh_session
 * @{
 */

/** \brief connect to the ssh server
 * \param session ssh session
 * \return SSH_OK on success, SSH_ERROR on error
 * \see ssh_new()
 * \see ssh_disconnect()
 */
int ssh_connect(ssh_session session) {
  int ssh1 = 0;
  int ssh2 = 0;
  socket_t fd = SSH_INVALID_SOCKET;
  int ret;

  if (session == NULL) {
    ssh_set_error(session, SSH_FATAL, "Invalid session pointer");
    return SSH_ERROR;
  }

  enter_function();

  session->alive = 0;
  session->client = 1;

  if (ssh_init() < 0) {
    leave_function();
    return SSH_ERROR;
  }
  if (session->fd == SSH_INVALID_SOCKET && session->host == NULL &&
      session->ProxyCommand == NULL) {
    ssh_set_error(session, SSH_FATAL, "Hostname required");
    leave_function();
    return SSH_ERROR;
  }

  ret = ssh_options_apply(session);
  if (ret < 0) {
      ssh_set_error(session, SSH_FATAL, "Couldn't apply options");
      leave_function();
      return SSH_ERROR;
  }

  if (session->fd != SSH_INVALID_SOCKET) {
    fd = session->fd;
#ifndef _WIN32
  } else if (session->ProxyCommand != NULL) {
    fd=ssh_socket_connect_proxycommand(session, session->ProxyCommand);
#endif
  } else {
    fd = ssh_connect_host(session, session->host, session->bindaddr,
        session->port, session->timeout, session->timeout_usec);
  }
  if (fd == SSH_INVALID_SOCKET) {
    leave_function();
    return SSH_ERROR;
  }
  set_status(session, 0.2);

  ssh_socket_set_fd(session->socket, fd);

  session->alive = 1;
  session->serverbanner = ssh_get_banner(session);
  if (session->serverbanner == NULL) {
    ssh_socket_close(session->socket);
    session->alive = 0;
    leave_function();
    return SSH_ERROR;
  }
  set_status(session, 0.4);

  ssh_log(session, SSH_LOG_RARE,
      "SSH server banner: %s", session->serverbanner);

  /* Here we analyse the different protocols the server allows. */
  if (ssh_analyze_banner(session, &ssh1, &ssh2) < 0) {
    ssh_socket_close(session->socket);
    session->alive = 0;
    leave_function();
    return SSH_ERROR;
  }

  /* Here we decide which version of the protocol to use. */
  if (ssh2 && session->ssh2) {
    session->version = 2;
  } else if(ssh1 && session->ssh1) {
    session->version = 1;
  } else {
    ssh_set_error(session, SSH_FATAL,
        "No version of SSH protocol usable (banner: %s)",
        session->serverbanner);
    ssh_socket_close(session->socket);
    session->alive = 0;
    leave_function();
    return SSH_ERROR;
  }

  if (ssh_send_banner(session, 0) < 0) {
    ssh_set_error(session, SSH_FATAL, "Sending the banner failed");
    ssh_socket_close(session->socket);
    session->alive = 0;
    leave_function();
    return SSH_ERROR;
  }
  set_status(session, 0.5);

  switch (session->version) {
    case 2:
      if (ssh_get_kex(session,0) < 0) {
        ssh_socket_close(session->socket);
        session->alive = 0;
        leave_function();
        return SSH_ERROR;
      }
      set_status(session,0.6);

      ssh_list_kex(session, &session->server_kex);
      if (set_kex(session) < 0) {
        ssh_socket_close(session->socket);
        session->alive = 0;
        leave_function();
        return SSH_ERROR;
      }
      if (ssh_send_kex(session, 0) < 0) {
        ssh_socket_close(session->socket);
        session->alive = 0;
        leave_function();
        return SSH_ERROR;
      }
      set_status(session,0.8);

      if (dh_handshake(session) < 0) {
        ssh_socket_close(session->socket);
        session->alive = 0;
        leave_function();
        return SSH_ERROR;
      }
      set_status(session,1.0);

      session->connected = 1;
      break;
    case 1:
      if (ssh_get_kex1(session) < 0) {
        ssh_socket_close(session->socket);
        session->alive = 0;
        leave_function();
        return SSH_ERROR;
      }
      set_status(session,0.6);

      session->connected = 1;
      break;
  }

  leave_function();
  return 0;
}

/**
 * @brief Get the issue banner from the server.
 *
 * This is the banner showing a disclaimer to users who log in,
 * typically their right or the fact that they will be monitored.
 *
 * @param session       The SSH session to use.
 *
 * @return A newly allocated string with the banner, NULL on error.
 */
char *ssh_get_issue_banner(ssh_session session) {
  if (session == NULL || session->banner == NULL) {
    return NULL;
  }

  return string_to_char(session->banner);
}

/**
 * @brief Get the version of the OpenSSH server, if it is not an OpenSSH server
 * then 0 will be returned.
 *
 * You can use the SSH_VERSION_INT macro to compare version numbers.
 *
 * @param  session      The SSH session to use.
 *
 * @return The version number if available, 0 otherwise.
 */
int ssh_get_openssh_version(ssh_session session) {
  if (session == NULL) {
    return 0;
  }

  return session->openssh;
}

/**
 * @brief Disconnect from a session (client or server).
 * The session can then be reused to open a new session.
 *
 * @param session       The SSH session to disconnect.
 */
void ssh_disconnect(ssh_session session) {
  ssh_string str = NULL;

  if (session == NULL) {
    return;
  }

  enter_function();

  if (ssh_socket_is_open(session->socket)) {
    if (buffer_add_u8(session->out_buffer, SSH2_MSG_DISCONNECT) < 0) {
      goto error;
    }
    if (buffer_add_u32(session->out_buffer,
          htonl(SSH2_DISCONNECT_BY_APPLICATION)) < 0) {
      goto error;
    }

    str = string_from_char("Bye Bye");
    if (str == NULL) {
      goto error;
    }

    if (buffer_add_ssh_string(session->out_buffer,str) < 0) {
      string_free(str);
      goto error;
    }
    string_free(str);

    packet_send(session);
    ssh_socket_close(session->socket);
  }
  session->alive = 0;

error:
  leave_function();
}

const char *ssh_copyright(void) {
    return SSH_STRINGIFY(LIBSSH_VERSION) " (c) 2003-2008 Aris Adamantiadis "
    "(aris@0xbadc0de.be) Distributed under the LGPL, please refer to COPYING"
    "file for information about your rights";
}
/** @} */
/* vim: set ts=2 sw=2 et cindent: */
