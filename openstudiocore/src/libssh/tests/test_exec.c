/*
This file is distributed in public domain. You can do whatever you want
with its content.
*/
#include <libssh/libssh.h>
#include <stdio.h>
#include <string.h>
#include "tests.h"

void do_connect(SSH_SESSION *session) {
  char buf[4096] = {0};
  CHANNEL *channel;

  int error = ssh_connect(session);
  if (error != SSH_OK) {
    fprintf(stderr,"Error at connection: %s\n", ssh_get_error(session));
    return;
  }
  printf("Connected\n");

  ssh_is_server_known(session);

  error = authenticate(session);
  if(error != SSH_AUTH_SUCCESS) {
    fprintf(stderr,"Error at authentication: %s\n", ssh_get_error(session));
    return;
  }
  printf("Authenticated\n");
  channel = channel_new(session);
  channel_open_session(channel);
  printf("Execute 'ls' on the channel\n");
  error = channel_request_exec(channel, "ls");
  if(error != SSH_OK){
    fprintf(stderr, "Error executing command: %s\n", ssh_get_error(session));
    return;
  }
  printf("--------------------output----------------------\n");
  while (channel_read(channel, buf, sizeof(buf), 0)) {
    printf("%s", buf);
  }
  printf("\n");
  printf("---------------------end------------------------\n");
  channel_send_eof(channel);
  fprintf(stderr, "Exit status: %d\n", channel_get_exit_status(channel));

  printf("\nChannel test finished\n");
  channel_close(channel);
  channel_free(channel);
}

int main(int argc, char **argv){
	SSH_OPTIONS *options=set_opts(argc, argv);
	SSH_SESSION *session=ssh_new();
	if(options==NULL){
		return 1;
	}
	ssh_set_options(session,options);
	do_connect(session);
	ssh_disconnect(session);
	ssh_finalize();
	return 0;
}
