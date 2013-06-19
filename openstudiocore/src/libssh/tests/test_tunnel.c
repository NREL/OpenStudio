/*
This file is distributed in public domain. You can do whatever you want
with its content.
*/
#include <libssh/libssh.h>
#include <stdio.h>
#include <string.h>
#include "tests.h"
#define ECHO_PORT 7
void do_connect(SSH_SESSION *session){
	int error=ssh_connect(session);
	if(error != SSH_OK){
		fprintf(stderr,"Error at connection :%s\n",ssh_get_error(session));
		return;
	}
	printf("Connected\n");
	ssh_is_server_known(session);
	// we don't care what happens here
	error=authenticate(session);
	if(error != SSH_AUTH_SUCCESS){
		fprintf(stderr,"Error at authentication :%s\n",ssh_get_error(session));
		return;
	}
	printf("Authenticated\n");
	CHANNEL *channel=channel_new(session);
	error=channel_open_forward(channel,"localhost",ECHO_PORT,"localhost",42);
	if(error!=SSH_OK){
		fprintf(stderr,"Error when opening forward:%s\n",ssh_get_error(session));
		return;
	}
	printf("Forward opened\n");
	int i=0;
	char string[20];
	char buffer[20];
	for(i=0;i<2000;++i){
		sprintf(string,"%d\n",i);
		channel_write(channel,string,strlen(string));
		do {
			error=channel_poll(channel,0);
			//if(error < strlen(string))
				//usleep(10);
		} while(error < strlen(string) && error >= 0);
		if(error>0){
			error=channel_read_nonblocking(channel,buffer,strlen(string),0);
			if(error>=0){
				if(memcmp(buffer,string,strlen(string))!=0){
					fprintf(stderr,"Problem with answer: wanted %s got %s\n",string,buffer);
				} else {
					printf(".");
					fflush(stdout);
				}
			}
				
		}
		if(error==-1){
			fprintf(stderr,"Channel reading error : %s\n",ssh_get_error(session));
			break;
		}
	}
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
