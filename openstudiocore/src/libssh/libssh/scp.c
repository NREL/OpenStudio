/*
 * scp - SSH scp wrapper functions
 *
 * This file is part of the SSH Library
 *
 * Copyright (c) 2009 by Aris Adamantiadis <aris@0xbadc0de.be>
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
#include <string.h>

#include "libssh/priv.h"
#include "libssh/scp.h"
/** @defgroup ssh_scp SSH-scp
 * @brief SCP protocol over SSH functions
 * @addtogroup ssh_scp
 * @{
*/

/** @brief Creates a new scp session
 * @param session the SSH session to use
 * @param mode one of SSH_SCP_WRITE or SSH_SCP_READ, depending if you need to drop files remotely or read them.
 * It is not possible to combine read and write.
 * @param location The directory in which write or read will be done. Any push or pull will be relative 
 * to this place
 * @returns NULL if the creation was impossible.
 * @returns a ssh_scp handle if it worked.
 */
ssh_scp ssh_scp_new(ssh_session session, int mode, const char *location){
  ssh_scp scp=malloc(sizeof(struct ssh_scp_struct));
  if(scp == NULL){
    ssh_set_error(session,SSH_FATAL,"Error allocating memory for ssh_scp");
    return NULL;
  }
  ZERO_STRUCTP(scp);
  if((mode&~SSH_SCP_RECURSIVE) != SSH_SCP_WRITE && (mode &~SSH_SCP_RECURSIVE) != SSH_SCP_READ){
    ssh_set_error(session,SSH_FATAL,"Invalid mode %d for ssh_scp_new()",mode);
    ssh_scp_free(scp);
    return NULL;
  }
  scp->location=strdup(location);
  if (scp->location == NULL) {
    ssh_set_error(session,SSH_FATAL,"Error allocating memory for ssh_scp");
    ssh_scp_free(scp);
    return NULL;
  }
  scp->session=session;
  scp->mode=mode & ~SSH_SCP_RECURSIVE;
  scp->recursive = (mode & SSH_SCP_RECURSIVE) != 0;
  scp->channel=NULL;
  scp->state=SSH_SCP_NEW;
  return scp;
}

int ssh_scp_init(ssh_scp scp){
  int r;
  char execbuffer[1024];
  uint8_t code;
  if(scp->state != SSH_SCP_NEW){
    ssh_set_error(scp->session,SSH_FATAL,"ssh_scp_init called under invalid state");
    return SSH_ERROR;
  }
  ssh_log(scp->session,SSH_LOG_PROTOCOL,"Initializing scp session %s %son location '%s'",
		  scp->mode==SSH_SCP_WRITE?"write":"read",
				  scp->recursive?"recursive ":"",
						  scp->location);
  scp->channel=channel_new(scp->session);
  if(scp->channel == NULL){
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  r= channel_open_session(scp->channel);
  if(r==SSH_ERROR){
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  if(scp->mode == SSH_SCP_WRITE)
    snprintf(execbuffer,sizeof(execbuffer),"scp -t %s %s",
    		scp->recursive ? "-r":"", scp->location);
  else
    snprintf(execbuffer,sizeof(execbuffer),"scp -f %s %s",
    		scp->recursive ? "-r":"", scp->location);
  if(channel_request_exec(scp->channel,execbuffer) == SSH_ERROR){
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  if(scp->mode == SSH_SCP_WRITE){
	  r=channel_read(scp->channel,&code,1,0);
	  if(code != 0){
		  ssh_set_error(scp->session,SSH_FATAL, "scp status code %ud not valid", code);
		  scp->state=SSH_SCP_ERROR;
		  return SSH_ERROR;
	  }
  } else {
	  channel_write(scp->channel,"",1);
  }
  if(scp->mode == SSH_SCP_WRITE)
    scp->state=SSH_SCP_WRITE_INITED;
  else
    scp->state=SSH_SCP_READ_INITED;
  return SSH_OK;
}

int ssh_scp_close(ssh_scp scp){
  char buffer[128];
  int err;
  if(scp->channel != NULL){
    if(channel_send_eof(scp->channel) == SSH_ERROR){
      scp->state=SSH_SCP_ERROR;
      return SSH_ERROR;
    }
    /* avoid situations where data are buffered and
     * not yet stored on disk. This can happen if the close is sent
     * before we got the EOF back
     */
    while(!channel_is_eof(scp->channel)){
      err=channel_read(scp->channel,buffer,sizeof(buffer),0);
      if(err==SSH_ERROR)
        break;
    }
    if(channel_close(scp->channel) == SSH_ERROR){
      scp->state=SSH_SCP_ERROR;
      return SSH_ERROR;
    }
    channel_free(scp->channel);
    scp->channel=NULL;
  }
  scp->state=SSH_SCP_NEW;
  return SSH_OK;
}

void ssh_scp_free(ssh_scp scp){
  if(scp->state != SSH_SCP_NEW)
    ssh_scp_close(scp);
  if(scp->channel)
    channel_free(scp->channel);
  SAFE_FREE(scp->location);
  SAFE_FREE(scp->request_name);
  SAFE_FREE(scp->warning);
  SAFE_FREE(scp);
}

/** @brief creates a directory in a scp in sink mode
 * @param scp the scp handle.
 * @param dirname Name of the directory being created.
 * @param mode  Unix permissions for the new directory, e.g. 0755.
 * @returns SSH_OK if the directory was created.
 * @returns SSH_ERROR if an error happened.
 * @see ssh_scp_leave_directory
 */
int ssh_scp_push_directory(ssh_scp scp, const char *dirname, int mode){
  char buffer[1024];
  int r;
  uint8_t code;
  char *dir;
  char *perms;
  if(scp->state != SSH_SCP_WRITE_INITED){
    ssh_set_error(scp->session,SSH_FATAL,"ssh_scp_push_directory called under invalid state");
    return SSH_ERROR;
  }
  dir=ssh_basename(dirname);
  perms=ssh_scp_string_mode(mode);
  snprintf(buffer, sizeof(buffer), "D%s 0 %s\n", perms, dir);
  SAFE_FREE(dir);
  SAFE_FREE(perms);
  r=channel_write(scp->channel,buffer,strlen(buffer));
  if(r==SSH_ERROR){
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  r=channel_read(scp->channel,&code,1,0);
  if(code != 0){
    ssh_set_error(scp->session,SSH_FATAL, "scp status code %ud not valid", code);
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  return SSH_OK;
}

/**
 * @brief Leaves a directory
 * @returns SSH_OK if the directory was created.
 * @returns SSH_ERROR if an error happened.
 * @see ssh_scp_push_directory
 */
 int ssh_scp_leave_directory(ssh_scp scp){
  char buffer[]="E\n";
  int r;
  uint8_t code;
  if(scp->state != SSH_SCP_WRITE_INITED){
    ssh_set_error(scp->session,SSH_FATAL,"ssh_scp_leave_directory called under invalid state");
    return SSH_ERROR;
  }
  r=channel_write(scp->channel,buffer,strlen(buffer));
  if(r==SSH_ERROR){
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  r=channel_read(scp->channel,&code,1,0);
  if(code != 0){
    ssh_set_error(scp->session,SSH_FATAL, "scp status code %ud not valid", code);
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  return SSH_OK;
}


/** @brief initializes the sending of a file to a scp in sink mode
 * @param scp the scp handle.
 * @param filename Name of the file being sent. It should not contain any path indicator
 * @param size Exact size in bytes of the file being sent.
 * @param mode Unix permissions for the new file, e.g. 0644
 * @returns SSH_OK if the file is ready to be sent.
 * @returns SSH_ERROR if an error happened.
 */
int ssh_scp_push_file(ssh_scp scp, const char *filename, size_t size, int mode){
  char buffer[1024];
  int r;
  uint8_t code;
  char *file;
  char *perms;
  if(scp->state != SSH_SCP_WRITE_INITED){
    ssh_set_error(scp->session,SSH_FATAL,"ssh_scp_push_file called under invalid state");
    return SSH_ERROR;
  }
  file=ssh_basename(filename);
  perms=ssh_scp_string_mode(mode);
  ssh_log(scp->session,SSH_LOG_PROTOCOL,"SCP pushing file %s, size %" PRIdS " with permissions '%s'",file,size,perms);
  snprintf(buffer, sizeof(buffer), "C%s %" PRIdS " %s\n", perms, size, file);
  SAFE_FREE(file);
  SAFE_FREE(perms);
  r=channel_write(scp->channel,buffer,strlen(buffer));
  if(r==SSH_ERROR){
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  r=channel_read(scp->channel,&code,1,0);
  if(code != 0){
    ssh_set_error(scp->session,SSH_FATAL, "scp status code %ud not valid", code);
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  scp->filelen = size;
  scp->processed = 0;
  scp->state=SSH_SCP_WRITE_WRITING;
  return SSH_OK;
}

/** @brief waits for a response of the scp server
 * @internal
 * @param response pointer where the response message must be
 * copied if any. This pointer must then be free'd.
 * @returns the return code.
 * @returns SSH_ERROR a error occured
 */
int ssh_scp_response(ssh_scp scp, char **response){
	unsigned char code;
	int r;
	char msg[128];
	r=channel_read(scp->channel,&code,1,0);
	if(r == SSH_ERROR)
		return SSH_ERROR;
	if(code == 0)
		return 0;
	if(code > 2){
		ssh_set_error(scp->session,SSH_FATAL, "SCP: invalid status code %ud received", code);
		scp->state=SSH_SCP_ERROR;
		return SSH_ERROR;
	}
	r=ssh_scp_read_string(scp,msg,sizeof(msg));
	if(r==SSH_ERROR)
		return r;
	/* Warning */
	if(code == 1){
		ssh_set_error(scp->session,SSH_REQUEST_DENIED, "SCP: Warning: status code 1 received: %s", msg);
		ssh_log(scp->session,SSH_LOG_RARE,"SCP: Warning: status code 1 received: %s", msg);
		if(response)
			*response=strdup(msg);
		return 1;
	}
	if(code == 2){
		ssh_set_error(scp->session,SSH_FATAL, "SCP: Error: status code 2 received: %s", msg);
		if(response)
			*response=strdup(msg);
		return 2;
	}
	/* Not reached */
	return SSH_ERROR;
}

/** @brief Write into a remote scp file
 * @param scp the scp handle.
 * @param buffer the buffer to write
 * @param len the number of bytes to write
 * @returns SSH_OK the write was successful
 * @returns SSH_ERROR an error happened while writing
 */
int ssh_scp_write(ssh_scp scp, const void *buffer, size_t len){
  int w;
  //int r;
  //uint8_t code;
  if(scp->state != SSH_SCP_WRITE_WRITING){
    ssh_set_error(scp->session,SSH_FATAL,"ssh_scp_write called under invalid state");
    return SSH_ERROR;
  }
  if(scp->processed + len > scp->filelen)
    len = scp->filelen - scp->processed;
  /* hack to avoid waiting for window change */
  channel_poll(scp->channel,0);
  w=channel_write(scp->channel,buffer,len);
  if(w != SSH_ERROR)
    scp->processed += w;
  else {
    scp->state=SSH_SCP_ERROR;
    //return=channel_get_exit_status(scp->channel);
    return SSH_ERROR;
  }
  /* Check if we arrived at end of file */
  if(scp->processed == scp->filelen) {
/*    r=channel_read(scp->channel,&code,1,0);
    if(r==SSH_ERROR){
      scp->state=SSH_SCP_ERROR;
      return SSH_ERROR;
    }
    if(code != 0){
      ssh_set_error(scp->session,SSH_FATAL, "scp status code %ud not valid", code);
      scp->state=SSH_SCP_ERROR;
      return SSH_ERROR;
    }
*/
    scp->processed=scp->filelen=0;
    scp->state=SSH_SCP_WRITE_INITED;
  }
  return SSH_OK;
}

/**
 * @brief reads a string on a channel, terminated by '\n'
 * @param scp the scp handle.
 * @param buffer pointer to a buffer to place the string
 * @param len size of the buffer in bytes. If the string is bigger
 * than len-1, only len-1 bytes are read and the string
 * is null-terminated.
 * @returns SSH_OK The string was read
 * @returns SSH_ERROR Error happened while reading
 */
int ssh_scp_read_string(ssh_scp scp, char *buffer, size_t len){
  size_t r=0;
  int err=SSH_OK;
  while(r<len-1){
    err=channel_read(scp->channel,&buffer[r],1,0);
    if(err==SSH_ERROR){
      break;
    }
    if(err==0){
      ssh_set_error(scp->session,SSH_FATAL,"End of file while reading string");
      err=SSH_ERROR;
      break;
    }
    r++;
    if(buffer[r-1] == '\n')
      break;
  }
  buffer[r]=0;
  return err;
}

/** @brief waits for a scp request (file, directory)
 * @returns SSH_ERROR Some error happened
 * @returns SSH_SCP_REQUEST_NEWFILE The other side is sending a file
 * @returns SSH_SCP_REQUEST_NEWDIRECTORY The other side is sending a directory
 * @returns SSH_SCP_REQUEST_END_DIRECTORY The other side has finished with the current directory
 * @see ssh_scp_read
 * @see ssh_scp_deny_request
 * @see ssh_scp_accept_request
 */
int ssh_scp_pull_request(ssh_scp scp){
  char buffer[4096];
  char *mode=NULL;
  char *p,*tmp;
  size_t size;
  char *name=NULL;
  int err;
  if(scp->state != SSH_SCP_READ_INITED){
    ssh_set_error(scp->session,SSH_FATAL,"ssh_scp_pull_request called under invalid state");
    return SSH_ERROR;
  }
  err=ssh_scp_read_string(scp,buffer,sizeof(buffer));
  if(err==SSH_ERROR){
	if(channel_is_eof(scp->channel)){
		scp->state=SSH_SCP_TERMINATED;
		return SSH_SCP_REQUEST_EOF;
	}
    return err;
  }
  p=strchr(buffer,'\n');
  if(p!=NULL)
	  *p='\0';
  ssh_log(scp->session,SSH_LOG_PROTOCOL,"Received SCP request: '%s'",buffer);
  switch(buffer[0]){
    case 'C':
      /* File */
    case 'D':
      /* Directory */
      p=strchr(buffer,' ');
      if(p==NULL)
        goto error;
      *p='\0';
      p++;
      //mode=strdup(&buffer[1]);
      scp->request_mode=ssh_scp_integer_mode(&buffer[1]);
      tmp=p;
      p=strchr(p,' ');
      if(p==NULL)
        goto error;
      *p=0;
      size=strtoull(tmp,NULL,10);
      p++;
      name=strdup(p);
      SAFE_FREE(scp->request_name);
      scp->request_name=name;
      if(buffer[0]=='C'){
        scp->filelen=size;
        scp->request_type=SSH_SCP_REQUEST_NEWFILE;
      } else {
        scp->filelen='0';
        scp->request_type=SSH_SCP_REQUEST_NEWDIR;
      }
      scp->state=SSH_SCP_READ_REQUESTED;
      scp->processed = 0;
      return scp->request_type;
      break;
    case 'E':
    	scp->request_type=SSH_SCP_REQUEST_ENDDIR;
    	channel_write(scp->channel,"",1);
    	return scp->request_type;
    case 0x1:
    	ssh_set_error(scp->session,SSH_REQUEST_DENIED,"SCP: Warning: %s",&buffer[1]);
    	scp->request_type=SSH_SCP_REQUEST_WARNING;
    	SAFE_FREE(scp->warning);
    	scp->warning=strdup(&buffer[1]);
    	return scp->request_type;
    case 0x2:
    	ssh_set_error(scp->session,SSH_FATAL,"SCP: Error: %s",&buffer[1]);
    	return SSH_ERROR;
    case 'T':
      /* Timestamp */
    default:
      ssh_set_error(scp->session,SSH_FATAL,"Unhandled message: (%d)%s",buffer[0],buffer);
      return SSH_ERROR;
  }

  /* a parsing error occured */
  error:
  SAFE_FREE(name);
  SAFE_FREE(mode);
  ssh_set_error(scp->session,SSH_FATAL,"Parsing error while parsing message: %s",buffer);
  return SSH_ERROR;
}

/**
 * @brief denies the transfer of a file or creation of a directory
 *  coming from the remote party
 *  @param scp the scp handle.
 *  @param reason nul-terminated string with a human-readable explanation
 *  of the deny
 *  @returns SSH_OK the message was sent
 *  @returns SSH_ERROR Error sending the message, or sending it in a bad state
 */
int ssh_scp_deny_request(ssh_scp scp, const char *reason){
  char buffer[4096];
  int err;
  if(scp->state != SSH_SCP_READ_REQUESTED){
    ssh_set_error(scp->session,SSH_FATAL,"ssh_scp_deny_request called under invalid state");
    return SSH_ERROR;
  }
  snprintf(buffer,sizeof(buffer),"%c%s\n",2,reason);
  err=channel_write(scp->channel,buffer,strlen(buffer));
  if(err==SSH_ERROR) {
    return SSH_ERROR;
  }
  else {
    scp->state=SSH_SCP_READ_INITED;
    return SSH_OK;
  }
}

/**
 * @brief accepts transfer of a file or creation of a directory
 *  coming from the remote party
 *  @param scp the scp handle.
 *  @returns SSH_OK the message was sent
 *  @returns SSH_ERROR Error sending the message, or sending it in a bad state
 */
int ssh_scp_accept_request(ssh_scp scp){
  char buffer[]={0x00};
  int err;
  if(scp->state != SSH_SCP_READ_REQUESTED){
    ssh_set_error(scp->session,SSH_FATAL,"ssh_scp_deny_request called under invalid state");
    return SSH_ERROR;
  }
  err=channel_write(scp->channel,buffer,1);
  if(err==SSH_ERROR) {
    return SSH_ERROR;
  }
  if(scp->request_type==SSH_SCP_REQUEST_NEWFILE)
    scp->state=SSH_SCP_READ_READING;
  else
    scp->state=SSH_SCP_READ_INITED;
  return SSH_OK;
}

/** @brief Read from a remote scp file
 * @param scp the scp handle.
 * @param buffer Destination buffer
 * @param size Size of the buffer
 * @returns Number of bytes read
 * @returns SSH_ERROR An error happened while reading
 */
int ssh_scp_read(ssh_scp scp, void *buffer, size_t size){
  int r;
  int code;
  if(scp->state == SSH_SCP_READ_REQUESTED && scp->request_type == SSH_SCP_REQUEST_NEWFILE){
    r=ssh_scp_accept_request(scp);
    if(r==SSH_ERROR)
      return r;
  }
  if(scp->state != SSH_SCP_READ_READING){
    ssh_set_error(scp->session,SSH_FATAL,"ssh_scp_read called under invalid state");
    return SSH_ERROR;
  }
  if(scp->processed + size > scp->filelen)
    size = scp->filelen - scp->processed;
  if(size > 65536)
    size=65536; /* avoid too large reads */
  r=channel_read(scp->channel,buffer,size,0);
  if(r != SSH_ERROR)
    scp->processed += r;
  else {
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  /* Check if we arrived at end of file */
  if(scp->processed == scp->filelen) {
    scp->processed=scp->filelen=0;
    channel_write(scp->channel,"",1);
    code=ssh_scp_response(scp,NULL);
    if(code == 0){
    	scp->state=SSH_SCP_READ_INITED;
    	return r;
    }
    if(code==1){
    	scp->state=SSH_SCP_READ_INITED;
    	return SSH_ERROR;
    }
    scp->state=SSH_SCP_ERROR;
    return SSH_ERROR;
  }
  return r;
}

/** @brief Gets the name of the directory or file being
 * pushed from the other party
 * @returns file name. Should not be freed.
 */
const char *ssh_scp_request_get_filename(ssh_scp scp){
  return scp->request_name;
}

/**@brief Gets the permissions of the directory or file being
 * pushed from the other party
 * @returns Unix permission, e.g 0644.
 */
int ssh_scp_request_get_permissions(ssh_scp scp){
  return scp->request_mode;
}

/** @brief Gets the size of the file being pushed
 * from the other party
 * @returns Numeric size of the file being read.
 */
size_t ssh_scp_request_get_size(ssh_scp scp){
  return scp->filelen;
}

/** @brief Converts a scp text mode to an integer one
 * @param mode mode to convert, e.g. "0644"
 * @returns integer value, e.g. 420 for "0644"
 */
int ssh_scp_integer_mode(const char *mode){
	int value=strtoul(mode,NULL,8) & 0xffff;
	return value;
}

/** @brief Converts a unix mode into a scp string one.
 * @param mode mode to convert, e.g. 420 or 0644
 * @returns pointer to a malloc'ed string containing the scp mode,
 * e.g. "0644".
 */
char *ssh_scp_string_mode(int mode){
	char buffer[16];
	snprintf(buffer,sizeof(buffer),"%.4o",mode);
	return strdup(buffer);
}

/** @brief Gets the warning string
 * @returns Warning string. Should not be freed.
 */

const char *ssh_scp_request_get_warning(ssh_scp scp){
	return scp->warning;
}

/** @} */

