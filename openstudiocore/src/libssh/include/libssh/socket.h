/*
 * This file is part of the SSH Library
 *
 * Copyright (c) 2009 by Aris Adamantiadis
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

#ifndef SOCKET_H_
#define SOCKET_H_

/* socket.c */

char **get_environ(void); // Necessary to support the case where libssh is used in a dylib on macos
struct socket;
int ssh_socket_init(void);
struct socket *ssh_socket_new(ssh_session session);
void ssh_socket_free(struct socket *s);
void ssh_socket_set_fd(struct socket *s, socket_t fd);
socket_t ssh_socket_get_fd(struct socket *s);
#ifndef _WIN32
int ssh_socket_unix(struct socket *s, const char *path);
#endif
void ssh_socket_close(struct socket *s);
int ssh_socket_read(struct socket *s, void *buffer, int len);
int ssh_socket_write(struct socket *s,const void *buffer, int len);
int ssh_socket_is_open(struct socket *s);
int ssh_socket_fd_isset(struct socket *s, fd_set *set);
void ssh_socket_fd_set(struct socket *s, fd_set *set, socket_t *max_fd);
int ssh_socket_completeread(struct socket *s, void *buffer, uint32_t len);
int ssh_socket_completewrite(struct socket *s, const void *buffer, uint32_t len);
int ssh_socket_wait_for_data(struct socket *s, ssh_session session, uint32_t len);
int ssh_socket_nonblocking_flush(struct socket *s);
int ssh_socket_blocking_flush(struct socket *s);
int ssh_socket_poll(struct socket *s, int *writeable, int *except);
void ssh_socket_set_towrite(struct socket *s);
void ssh_socket_set_toread(struct socket *s);
void ssh_socket_set_except(struct socket *s);
int ssh_socket_get_status(struct socket *s);
int ssh_socket_data_available(struct socket *s);
int ssh_socket_data_writable(struct socket *s);

#ifndef _WIN32
void ssh_execute_command(const char *command, socket_t in, socket_t out);
socket_t ssh_socket_connect_proxycommand(ssh_session session,
    const char *command);
#endif

#endif /* SOCKET_H_ */
