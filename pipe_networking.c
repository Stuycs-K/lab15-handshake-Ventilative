#include "pipe_networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  mkfifo("serverPipe", 0666);
  int fd = open("serverPipe", O_RDONLY);
  int from_client;
  while(1){
    int bytesRead = read(fd, from_client, 4);
    if (bytesRead > 0){
      remove("serverPipe");
      break;
    }
  }
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client = server_setup();
  to_client = open(from_client, O_WRONLY);
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  mkfifo(68743496, 0666);
  int fd = open("serverPipe", O_WRONLY);
  write(68743496, fd, 4);
  int pfd = open(68743496, O_RDONLY);
  int from_server;
  to_server = server_handshake(from_server);
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
