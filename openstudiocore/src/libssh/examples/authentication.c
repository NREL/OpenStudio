/*
 * authentication.c
 * This file contains an example of how to do an authentication to a
 * SSH server using libssh
 */

/*
Copyright 2003-2009 Aris Adamantiadis

This file is part of the SSH Library

You are free to copy this file, modify it in any way, consider it being public
domain. This does not apply to the rest of the library though, but it is
allowed to cut-and-paste working code from this file to any license of
program.
The goal is to show the API in action. It's not a reference on how terminal
clients must be made or how a client should react.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <libssh/libssh.h>
#include "examples_common.h"

int authenticate_kbdint(ssh_session session){
  int err=ssh_userauth_kbdint(session,NULL,NULL);
  const char *name, *instruction, *prompt;
  char *ptr;
  char buffer[128];
  int i,n;
  char echo;
  while (err==SSH_AUTH_INFO){
    name=ssh_userauth_kbdint_getname(session);
    instruction=ssh_userauth_kbdint_getinstruction(session);
    n=ssh_userauth_kbdint_getnprompts(session);
    if(strlen(name)>0)
      printf("%s\n",name);
    if(strlen(instruction)>0)
      printf("%s\n",instruction);
    for(i=0;i<n;++i){
      prompt=ssh_userauth_kbdint_getprompt(session,i,&echo);
      if(echo){
        printf("%s",prompt);
        fgets(buffer,sizeof(buffer),stdin);
        buffer[sizeof(buffer)-1]=0;
        if((ptr=strchr(buffer,'\n')))
          *ptr=0;
        if (ssh_userauth_kbdint_setanswer(session,i,buffer) < 0) {
          return SSH_AUTH_ERROR;
        }
        memset(buffer,0,strlen(buffer));
      } else {
        ptr=getpass(prompt);
        if (ssh_userauth_kbdint_setanswer(session,i,ptr) < 0) {
          return SSH_AUTH_ERROR;
        }
      }
    }
    err=ssh_userauth_kbdint(session,NULL,NULL);
  }
  return err;
}

int authenticate_console(ssh_session session){
  int rc;
  int method;
  char *password;
  char *banner;

  // Try to authenticate
  rc = ssh_userauth_none(session, NULL);
  if (rc == SSH_AUTH_ERROR) {
    perror("Authentication failed.");
    return rc;
  }

  method = ssh_auth_list(session);
  while (rc != SSH_AUTH_SUCCESS) {

    // Try to authenticate with public key first
    if (method & SSH_AUTH_METHOD_PUBLICKEY) {
      rc = ssh_userauth_autopubkey(session, NULL);
      if (rc == SSH_AUTH_ERROR) {
        perror("Authentication failed.");
        return rc;
      } else if (rc == SSH_AUTH_SUCCESS) {
        break;
      }
    }

    // Try to authenticate with keyboard interactive";
    if (method & SSH_AUTH_METHOD_INTERACTIVE) {
      rc = authenticate_kbdint(session);
      if (rc == SSH_AUTH_ERROR) {
        perror("Authentication failed.");
        return rc;
      } else if (rc == SSH_AUTH_SUCCESS) {
        break;
      }
    }

    password=getpass("Password: ");
    // Try to authenticate with password
    if (method & SSH_AUTH_METHOD_PASSWORD) {
      rc = ssh_userauth_password(session, NULL, password);
      if (rc == SSH_AUTH_ERROR) {
        perror("Authentication failed.");
        return rc;
      } else if (rc == SSH_AUTH_SUCCESS) {
        break;
      }
    }
  }

  banner = ssh_get_issue_banner(session);
  if (banner) {
    printf("%s\n",banner);
    free(banner);
  }

  return rc;
}
