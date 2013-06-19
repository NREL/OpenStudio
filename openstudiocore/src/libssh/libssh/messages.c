/*
 * messages.c - message parsion for the server
 *
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

/** \defgroup ssh_messages SSH Messages
 * this file contains the Message parsing utilities for server programs using
 * libssh. The main loop of the program will call ssh_message_get(session) to
 * get messages as they come. they are not 1-1 with the protocol messages.
 * then, the user will know what kind of a message it is and use the appropriate
 * functions to handle it (or use the default handlers if she doesn't know what to
 * do
 * \addtogroup ssh_messages
 * @{
 */

#include <string.h>
#include <stdlib.h>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#include "libssh/libssh.h"
#include "libssh/priv.h"
#include "libssh/ssh2.h"
#include "libssh/buffer.h"
#include "libssh/packet.h"
#include "libssh/channels.h"
#include "libssh/session.h"
#include "libssh/misc.h"
#include "libssh/keys.h"
#include "libssh/dh.h"
#include "libssh/messages.h"

static ssh_message message_new(ssh_session session){
  ssh_message msg = malloc(sizeof(struct ssh_message_struct));
  if (msg == NULL) {
    return NULL;
  }
  ZERO_STRUCTP(msg);
  msg->session = session;
  return msg;
}

static ssh_message handle_service_request(ssh_session session) {
  ssh_string service = NULL;
  char *service_c = NULL;
  ssh_message msg=NULL;

  enter_function();

  service = buffer_get_ssh_string(session->in_buffer);
  if (service == NULL) {
    ssh_set_error(session, SSH_FATAL, "Invalid SSH_MSG_SERVICE_REQUEST packet");
    goto error;
  }

  service_c = string_to_char(service);
  if (service_c == NULL) {
    goto error;
  }
  ssh_log(session, SSH_LOG_PACKET,
        "Received a SERVICE_REQUEST for service %s", service_c);
  msg=message_new(session);
  if(!msg){
    SAFE_FREE(service_c);
    goto error;
  }
  msg->type=SSH_REQUEST_SERVICE;
  msg->service_request.service=service_c;
  error:
  string_free(service);
  leave_function();
  return msg;
}

static int handle_unimplemented(ssh_session session) {
  if (buffer_add_u32(session->out_buffer, htonl(session->recv_seq - 1)) < 0) {
    return -1;
  }

  if (packet_send(session) != SSH_OK) {
    return -1;
  }

  return 0;
}

static ssh_message handle_userauth_request(ssh_session session){
  ssh_string user = NULL;
  ssh_string service = NULL;
  ssh_string method = NULL;
  ssh_message msg = NULL;
  char *service_c = NULL;
  char *method_c = NULL;
  uint32_t method_size = 0;

  enter_function();

  msg = message_new(session);
  if (msg == NULL) {
    return NULL;
  }

  user = buffer_get_ssh_string(session->in_buffer);
  if (user == NULL) {
    goto error;
  }
  service = buffer_get_ssh_string(session->in_buffer);
  if (service == NULL) {
    goto error;
  }
  method = buffer_get_ssh_string(session->in_buffer);
  if (method == NULL) {
    goto error;
  }

  msg->type = SSH_REQUEST_AUTH;
  msg->auth_request.username = string_to_char(user);
  if (msg->auth_request.username == NULL) {
    goto error;
  }
  string_free(user);
  user = NULL;

  service_c = string_to_char(service);
  if (service_c == NULL) {
    goto error;
  }
  method_c = string_to_char(method);
  if (method_c == NULL) {
    goto error;
  }
  method_size = string_len(method);

  string_free(service);
  service = NULL;
  string_free(method);
  method = NULL;

  ssh_log(session, SSH_LOG_PACKET,
      "Auth request for service %s, method %s for user '%s'",
      service_c, method_c,
      msg->auth_request.username);


  if (strncmp(method_c, "none", method_size) == 0) {
    msg->auth_request.method = SSH_AUTH_METHOD_NONE;
    SAFE_FREE(service_c);
    SAFE_FREE(method_c);
    leave_function();
    return msg;
  }

  if (strncmp(method_c, "password", method_size) == 0) {
    ssh_string pass = NULL;
    uint8_t tmp;

    msg->auth_request.method = SSH_AUTH_METHOD_PASSWORD;
    SAFE_FREE(service_c);
    SAFE_FREE(method_c);
    buffer_get_u8(session->in_buffer, &tmp);
    pass = buffer_get_ssh_string(session->in_buffer);
    if (pass == NULL) {
      goto error;
    }
    msg->auth_request.password = string_to_char(pass);
    string_burn(pass);
    string_free(pass);
    pass = NULL;
    if (msg->auth_request.password == NULL) {
      goto error;
    }
    leave_function();
    return msg;
  }

  if (strncmp(method_c, "publickey", method_size) == 0) {
    ssh_string algo = NULL;
    ssh_string publickey = NULL;
    uint8_t has_sign;

    msg->auth_request.method = SSH_AUTH_METHOD_PUBLICKEY;
    SAFE_FREE(method_c);
    buffer_get_u8(session->in_buffer, &has_sign);
    algo = buffer_get_ssh_string(session->in_buffer);
    if (algo == NULL) {
      goto error;
    }
    publickey = buffer_get_ssh_string(session->in_buffer);
    if (publickey == NULL) {
      string_free(algo);
      algo = NULL;
      goto error;
    }
    msg->auth_request.public_key = publickey_from_string(session, publickey);
    string_free(algo);
    algo = NULL;
    string_free(publickey);
    publickey = NULL;
    if (msg->auth_request.public_key == NULL) {
       goto error;
    }
    msg->auth_request.signature_state = 0;
    // has a valid signature ?
    if(has_sign) {
      SIGNATURE *signature = NULL;
      ssh_public_key public_key = msg->auth_request.public_key;
      ssh_string sign = NULL;
      ssh_buffer digest = NULL;

      sign = buffer_get_ssh_string(session->in_buffer);
      if(sign == NULL) {
        ssh_log(session, SSH_LOG_PACKET, "Invalid signature packet from peer");
        msg->auth_request.signature_state = -2;
        goto error;
      }
      signature = signature_from_string(session, sign, public_key,
                                                       public_key->type);
      digest = ssh_userauth_build_digest(session, msg, service_c);
      if ((digest == NULL || signature == NULL) ||
          (digest != NULL && signature != NULL &&
          sig_verify(session, public_key, signature,
                     buffer_get(digest), buffer_get_len(digest)) < 0)) {
        ssh_log(session, SSH_LOG_PACKET, "Invalid signature from peer");

        string_free(sign);
        sign = NULL;
        buffer_free(digest);
        digest = NULL;
        signature_free(signature);
        signature = NULL;

        msg->auth_request.signature_state = -1;
        goto error;
      }         
      else
        ssh_log(session, SSH_LOG_PACKET, "Valid signature received");

      buffer_free(digest);
      digest = NULL;
      string_free(sign);
      sign = NULL;
      signature_free(signature);
      signature = NULL;

      msg->auth_request.signature_state = 1;
    }
    SAFE_FREE(service_c);
    leave_function();
    return msg;
  }

  msg->auth_request.method = SSH_AUTH_METHOD_UNKNOWN;
  SAFE_FREE(method_c);

  leave_function();
  return msg;
error:
  string_free(user);
  string_free(service);
  string_free(method);

  SAFE_FREE(method_c);
  SAFE_FREE(service_c);

  ssh_message_free(msg);

  leave_function();
  return NULL;
}

static ssh_message handle_channel_request_open(ssh_session session) {
  ssh_message msg = NULL;
  ssh_string type = NULL, originator = NULL, destination = NULL;
  char *type_c = NULL;
  uint32_t sender, window, packet, originator_port, destination_port;

  enter_function();

  msg = message_new(session);
  if (msg == NULL) {
    ssh_set_error_oom(session);
    leave_function();
    return NULL;
  }

  msg->type = SSH_REQUEST_CHANNEL_OPEN;

  type = buffer_get_ssh_string(session->in_buffer);
  if (type == NULL) {
    ssh_set_error_oom(session);
    goto error;
  }
  type_c = string_to_char(type);
  if (type_c == NULL) {
    ssh_set_error_oom(session);
    goto error;
  }

  ssh_log(session, SSH_LOG_PACKET,
      "Clients wants to open a %s channel", type_c);

  buffer_get_u32(session->in_buffer, &sender);
  buffer_get_u32(session->in_buffer, &window);
  buffer_get_u32(session->in_buffer, &packet);

  msg->channel_request_open.sender = ntohl(sender);
  msg->channel_request_open.window = ntohl(window);
  msg->channel_request_open.packet_size = ntohl(packet);

  if (strcmp(type_c,"session") == 0) {
    msg->channel_request_open.type = SSH_CHANNEL_SESSION;
    string_free(type);
    SAFE_FREE(type_c);
    leave_function();
    return msg;
  }

  if (strcmp(type_c,"direct-tcpip") == 0) {
    destination = buffer_get_ssh_string(session->in_buffer);
	if (destination == NULL) {
    ssh_set_error_oom(session);
		goto error;
	}
	msg->channel_request_open.destination = string_to_char(type);
	if (msg->channel_request_open.destination == NULL) {
    ssh_set_error_oom(session);
	  string_free(destination);
	  goto error;
	}
    string_free(destination);

    buffer_get_u32(session->in_buffer, &destination_port);
    msg->channel_request_open.destination_port = ntohl(destination_port);

    originator = buffer_get_ssh_string(session->in_buffer);
	if (originator == NULL) {
    ssh_set_error_oom(session);
	  goto error;
	}
	msg->channel_request_open.originator = string_to_char(type);
	if (msg->channel_request_open.originator == NULL) {
    ssh_set_error_oom(session);
	  string_free(originator);
	  goto error;
	}
    string_free(originator);

    buffer_get_u32(session->in_buffer, &originator_port);
    msg->channel_request_open.originator_port = ntohl(originator_port);

    msg->channel_request_open.type = SSH_CHANNEL_DIRECT_TCPIP;
    string_free(type);
    SAFE_FREE(type_c);
    leave_function();
    return msg;
  }

  if (strcmp(type_c,"forwarded-tcpip") == 0) {
    destination = buffer_get_ssh_string(session->in_buffer);
	if (destination == NULL) {
    ssh_set_error_oom(session);
		goto error;
	}
	msg->channel_request_open.destination = string_to_char(type);
	if (msg->channel_request_open.destination == NULL) {
    ssh_set_error_oom(session);
	  string_free(destination);
	  goto error;
	}
    string_free(destination);

    buffer_get_u32(session->in_buffer, &destination_port);
    msg->channel_request_open.destination_port = ntohl(destination_port);

    originator = buffer_get_ssh_string(session->in_buffer);
	if (originator == NULL) {
    ssh_set_error_oom(session);
	  goto error;
	}
	msg->channel_request_open.originator = string_to_char(type);
	if (msg->channel_request_open.originator == NULL) {
    ssh_set_error_oom(session);
	  string_free(originator);
	  goto error;
	}
    string_free(originator);

    buffer_get_u32(session->in_buffer, &originator_port);
    msg->channel_request_open.originator_port = ntohl(originator_port);

    msg->channel_request_open.type = SSH_CHANNEL_FORWARDED_TCPIP;
    string_free(type);
    SAFE_FREE(type_c);
    leave_function();
    return msg;
  }

  if (strcmp(type_c,"x11") == 0) {
    originator = buffer_get_ssh_string(session->in_buffer);
	if (originator == NULL) {
    ssh_set_error_oom(session);
	  goto error;
	}
	msg->channel_request_open.originator = string_to_char(type);
	if (msg->channel_request_open.originator == NULL) {
    ssh_set_error_oom(session);
	  string_free(originator);
	  goto error;
	}
    string_free(originator);

    buffer_get_u32(session->in_buffer, &originator_port);
    msg->channel_request_open.originator_port = ntohl(originator_port);

    msg->channel_request_open.type = SSH_CHANNEL_X11;
    string_free(type);
    SAFE_FREE(type_c);
    leave_function();
    return msg;
  }

  msg->channel_request_open.type = SSH_CHANNEL_UNKNOWN;
  string_free(type);
  SAFE_FREE(type_c);

  leave_function();
  return msg;
error:
  string_free(type);
  SAFE_FREE(type_c);
  ssh_message_free(msg);

  leave_function();
  return NULL;
}

ssh_channel ssh_message_channel_request_open_reply_accept(ssh_message msg) {
  ssh_session session = msg->session;
  ssh_channel chan = NULL;

  enter_function();

  if (msg == NULL) {
    leave_function();
    return NULL;
  }

  chan = channel_new(session);
  if (chan == NULL) {
    leave_function();
    return NULL;
  }

  chan->local_channel = ssh_channel_new_id(session);
  chan->local_maxpacket = 35000;
  chan->local_window = 32000;
  chan->remote_channel = msg->channel_request_open.sender;
  chan->remote_maxpacket = msg->channel_request_open.packet_size;
  chan->remote_window = msg->channel_request_open.window;
  chan->open = 1;

  if (buffer_add_u8(session->out_buffer, SSH2_MSG_CHANNEL_OPEN_CONFIRMATION) < 0) {
    goto error;
  }
  if (buffer_add_u32(session->out_buffer, htonl(chan->remote_channel)) < 0) {
    goto error;
  }
  if (buffer_add_u32(session->out_buffer, htonl(chan->local_channel)) < 0) {
    goto error;
  }
  if (buffer_add_u32(session->out_buffer, htonl(chan->local_window)) < 0) {
    goto error;
  }
  if (buffer_add_u32(session->out_buffer, htonl(chan->local_maxpacket)) < 0) {
    goto error;
  }

  ssh_log(session, SSH_LOG_PACKET,
      "Accepting a channel request_open for chan %d", chan->remote_channel);

  if (packet_send(session) != SSH_OK) {
    goto error;
  }

  leave_function();
  return chan;
error:
  channel_free(chan);

  leave_function();
  return NULL;
}

static ssh_message handle_channel_request(ssh_session session) {
  ssh_message msg = NULL;
  ssh_string type = NULL;
  char *type_c = NULL;
  uint32_t channel;
  uint8_t want_reply;

  enter_function();

  msg = message_new(session);
  if (msg == NULL) {
    ssh_set_error_oom(session);
    return NULL;
  }

  buffer_get_u32(session->in_buffer, &channel);
  channel = ntohl(channel);

  type = buffer_get_ssh_string(session->in_buffer);
  if (type == NULL) {
    ssh_set_error_oom(session);
    goto error;
  }
  type_c = string_to_char(type);
  if (type_c == NULL) {
    ssh_set_error_oom(session);
    goto error;
  }
  string_free(type);

  buffer_get_u8(session->in_buffer,&want_reply);

  ssh_log(session, SSH_LOG_PACKET,
      "Received a %s channel_request for channel %d (want_reply=%hhd)",
      type_c, channel, want_reply);

  msg->type = SSH_REQUEST_CHANNEL;
  msg->channel_request.channel = ssh_channel_from_local(session, channel);
  if (msg->channel_request.channel == NULL) {
    ssh_set_error(session, SSH_FATAL, "There are no channels with the id %u.",
        channel);
    goto error;
  }
  msg->channel_request.want_reply = want_reply;

  if (strcmp(type_c, "pty-req") == 0) {
    ssh_string term = NULL;
    char *term_c = NULL;
    SAFE_FREE(type_c);

    term = buffer_get_ssh_string(session->in_buffer);
    if (term == NULL) {
      ssh_set_error_oom(session);
      goto error;
    }
    term_c = string_to_char(term);
    if (term_c == NULL) {
      ssh_set_error_oom(session);
      string_free(term);
      goto error;
    }
    string_free(term);

    msg->channel_request.type = SSH_CHANNEL_REQUEST_PTY;
    msg->channel_request.TERM = term_c;

    buffer_get_u32(session->in_buffer, &msg->channel_request.width);
    buffer_get_u32(session->in_buffer, &msg->channel_request.height);
    buffer_get_u32(session->in_buffer, &msg->channel_request.pxwidth);
    buffer_get_u32(session->in_buffer, &msg->channel_request.pxheight);

    msg->channel_request.width = ntohl(msg->channel_request.width);
    msg->channel_request.height = ntohl(msg->channel_request.height);
    msg->channel_request.pxwidth = ntohl(msg->channel_request.pxwidth);
    msg->channel_request.pxheight = ntohl(msg->channel_request.pxheight);
    msg->channel_request.modes = buffer_get_ssh_string(session->in_buffer);
    if (msg->channel_request.modes == NULL) {
      SAFE_FREE(term_c);
      goto error;
    }

    leave_function();
    return msg;
  }

  if (strcmp(type_c, "window-change") == 0) {
    SAFE_FREE(type_c);

    msg->channel_request.type = SSH_CHANNEL_REQUEST_WINDOW_CHANGE;

    buffer_get_u32(session->in_buffer, &msg->channel_request.width);
    buffer_get_u32(session->in_buffer, &msg->channel_request.height);
    buffer_get_u32(session->in_buffer, &msg->channel_request.pxwidth);
    buffer_get_u32(session->in_buffer, &msg->channel_request.pxheight);

    msg->channel_request.width = ntohl(msg->channel_request.width);
    msg->channel_request.height = ntohl(msg->channel_request.height);
    msg->channel_request.pxwidth = ntohl(msg->channel_request.pxwidth);
    msg->channel_request.pxheight = ntohl(msg->channel_request.pxheight);

    leave_function();
    return msg;
  }

  if (strcmp(type_c, "subsystem") == 0) {
    ssh_string subsys = NULL;
    char *subsys_c = NULL;

    SAFE_FREE(type_c);

    subsys = buffer_get_ssh_string(session->in_buffer);
    if (subsys == NULL) {
      ssh_set_error_oom(session);
      goto error;
    }
    subsys_c = string_to_char(subsys);
    if (subsys_c == NULL) {
      ssh_set_error_oom(session);
      string_free(subsys);
      goto error;
    }
    string_free(subsys);

    msg->channel_request.type = SSH_CHANNEL_REQUEST_SUBSYSTEM;
    msg->channel_request.subsystem = subsys_c;

    leave_function();
    return msg;
  }

  if (strcmp(type_c, "shell") == 0) {
    SAFE_FREE(type_c);
    msg->channel_request.type = SSH_CHANNEL_REQUEST_SHELL;

    leave_function();
    return msg;
  }

  if (strcmp(type_c, "exec") == 0) {
    ssh_string cmd = NULL;

    SAFE_FREE(type_c);

    cmd = buffer_get_ssh_string(session->in_buffer);
    if (cmd == NULL) {
      ssh_set_error_oom(session);
      goto error;
    }

    msg->channel_request.type = SSH_CHANNEL_REQUEST_EXEC;
    msg->channel_request.command = string_to_char(cmd);
    if (msg->channel_request.command == NULL) {
      string_free(cmd);
      goto error;
    }
    string_free(cmd);

    leave_function();
    return msg;
  }

  if (strcmp(type_c, "env") == 0) {
    ssh_string name = NULL;
    ssh_string value = NULL;

    SAFE_FREE(type_c);

    name = buffer_get_ssh_string(session->in_buffer);
    if (name == NULL) {
      ssh_set_error_oom(session);
      goto error;
    }
    value = buffer_get_ssh_string(session->in_buffer);
	if (value == NULL) {
    ssh_set_error_oom(session);
		string_free(name);
	  goto error;
	}

    msg->channel_request.type = SSH_CHANNEL_REQUEST_ENV;
    msg->channel_request.var_name = string_to_char(name);
    msg->channel_request.var_value = string_to_char(value);
    if (msg->channel_request.var_name == NULL ||
		msg->channel_request.var_value == NULL) {
      string_free(name);
      string_free(value);
      goto error;
    }
    string_free(name);
    string_free(value);

    leave_function();
    return msg;
  }

  msg->channel_request.type = SSH_CHANNEL_UNKNOWN;
  SAFE_FREE(type_c);

  leave_function();
  return msg;
error:
  string_free(type);
  SAFE_FREE(type_c);
  ssh_message_free(msg);

  leave_function();
  return NULL;
}

int ssh_message_channel_request_reply_success(ssh_message msg) {
  uint32_t channel;

  if (msg == NULL) {
    return SSH_ERROR;
  }

  if (msg->channel_request.want_reply) {
    channel = msg->channel_request.channel->remote_channel;

    ssh_log(msg->session, SSH_LOG_PACKET,
        "Sending a channel_request success to channel %d", channel);

    if (buffer_add_u8(msg->session->out_buffer, SSH2_MSG_CHANNEL_SUCCESS) < 0) {
      return SSH_ERROR;
    }
    if (buffer_add_u32(msg->session->out_buffer, htonl(channel)) < 0) {
      return SSH_ERROR;
    }

    return packet_send(msg->session);
  }

  ssh_log(msg->session, SSH_LOG_PACKET,
      "The client doesn't want to know the request succeeded");

  return SSH_OK;
}

ssh_message ssh_message_retrieve(ssh_session session, uint32_t packettype){
  ssh_message msg=NULL;
  enter_function();
  switch(packettype) {
    case SSH2_MSG_SERVICE_REQUEST:
      msg=handle_service_request(session);
      break;
    case SSH2_MSG_USERAUTH_REQUEST:
      msg = handle_userauth_request(session);
      break;
    case SSH2_MSG_CHANNEL_OPEN:
      msg = handle_channel_request_open(session);
      break;
    case SSH2_MSG_CHANNEL_REQUEST:
      msg = handle_channel_request(session);
      break;
    default:
      if (handle_unimplemented(session) == 0) {
        ssh_set_error(session, SSH_FATAL,
            "Unhandled message %d\n", session->in_packet.type);
      }
  }
  leave_function();
  return msg;
}

/* \brief blocking message retrieval
 * \bug does anything that is not a message, like a channel read/write
 */
ssh_message ssh_message_get(ssh_session session) {
  ssh_message msg = NULL;
  enter_function();
  do {
    if ((packet_read(session) != SSH_OK) ||
        (packet_translate(session) != SSH_OK)) {
      leave_function();
      return NULL;
    }
  } while(session->in_packet.type==SSH2_MSG_IGNORE || session->in_packet.type==SSH2_MSG_DEBUG);
  msg=ssh_message_retrieve(session,session->in_packet.type);
  leave_function();
  return msg;
}

int ssh_message_type(ssh_message msg) {
  if (msg == NULL) {
    return -1;
  }

  return msg->type;
}

int ssh_message_subtype(ssh_message msg) {
  if (msg == NULL) {
    return -1;
  }

  switch(msg->type) {
    case SSH_REQUEST_AUTH:
      return msg->auth_request.method;
    case SSH_REQUEST_CHANNEL_OPEN:
      return msg->channel_request_open.type;
    case SSH_REQUEST_CHANNEL:
      return msg->channel_request.type;
  }

  return -1;
}

void ssh_message_free(ssh_message msg){
  if (msg == NULL) {
    return;
  }

  switch(msg->type) {
    case SSH_REQUEST_AUTH:
      SAFE_FREE(msg->auth_request.username);
      if (msg->auth_request.password) {
        memset(msg->auth_request.password, 0,
            strlen(msg->auth_request.password));
        SAFE_FREE(msg->auth_request.password);
      }
      publickey_free(msg->auth_request.public_key);
      break;
    case SSH_REQUEST_CHANNEL_OPEN:
      SAFE_FREE(msg->channel_request_open.originator);
      SAFE_FREE(msg->channel_request_open.destination);
      break;
    case SSH_REQUEST_CHANNEL:
      SAFE_FREE(msg->channel_request.TERM);
      SAFE_FREE(msg->channel_request.modes);
      SAFE_FREE(msg->channel_request.var_name);
      SAFE_FREE(msg->channel_request.var_value);
      SAFE_FREE(msg->channel_request.command);
      SAFE_FREE(msg->channel_request.subsystem);
      break;
    case SSH_REQUEST_SERVICE:
      SAFE_FREE(msg->service_request.service);
      break;
  }
  ZERO_STRUCTP(msg);
  SAFE_FREE(msg);
}

/** \internal
 * \brief handle various SSH request messages and stack them for callback
 * \param session SSH session
 * \param type packet type
 * \returns nothing
 */
void message_handle(ssh_session session, uint32_t type){
  ssh_message msg=ssh_message_retrieve(session,type);
  ssh_log(session,SSH_LOG_PROTOCOL,"Stacking message from packet type %d",type);
  if(msg){
    if(!session->ssh_message_list){
      session->ssh_message_list=ssh_list_new();
    }
    ssh_list_append(session->ssh_message_list,msg);
  }
}

/**
 * @}
 */
/* vim: set ts=2 sw=2 et cindent: */
