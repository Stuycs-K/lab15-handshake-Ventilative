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
  int from_client = open("serverPipe", O_RDONLY);
  remove("serverPipe");
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
  if (read(from_client, to_client, 4) < 0) printf("Error line 41\n");
  char str[512];
  sprintf(str, "%d", *to_client);
  printf("%s\n", str);
  int downstream = open(str, O_WRONLY);
  printf("DOWNSTREAM OPENED\n");
  int ack = *to_client + 1;
  write(downstream, &ack, 4);
  //
  //
  // int * finalAck;
  // read(from_client, finalAck, 4);
  // if (*finalAck == ack - 2){
  //   printf("Done\n");
  // }
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

  char str[512];
  int pid = getpid();
  sprintf(str, "%d", pid);
  printf("My pid is %s\n", str);
  mkfifo(str, 0666);
  int fd = open("serverPipe", O_WRONLY);
  write(fd, &pid, 4);
  int from_server = open(str, O_RDONLY);
  printf("FROM SERVER CONNECTED\n");
  remove(str);
  int * ack;
  if (read(from_server, ack, 4) < 0) printf("%s\n", strerror(errno));
  printf("received ack %d\n", *ack);
  // if (*ack == pid + 1){
  //   write(fd, pid-1, 4);
  // }
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
