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

int rand(){
  int heresAnInt = -1;
  int rand = open("/dev/random", O_RDONLY);
  read(rand, &heresAnInt, 4);
  if (heresAnInt < 0) heresAnInt *= -1;
  heresAnInt %= 101;
  return heresAnInt;
}

void sighandler(int signo){
    remove("serverPipe");
    exit(0);
}

void plumber(int signo){
    printf("client left.\n");
}

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
  read(from_client, to_client, 4);
  char str[512];
  sprintf(str, "%d", *to_client);
  //printf("received %s\n", str);
  int ack = *to_client + 1;
  *to_client = open(str, O_WRONLY);
  //printf("DOWNSTREAM OPENED\n");
  write(*to_client, &ack, 4);
  int finalAck;
  read(from_client, &finalAck, 4);
  if (finalAck == ack - 2){
    //printf("Connection established.\n");
  }
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
  //printf("My pid is %s\n", str);
  mkfifo(str, 0666);
  *to_server = open("serverPipe", O_WRONLY);
  write(*to_server, &pid, 4);
  int from_server = open(str, O_RDONLY);
  //printf("FROM SERVER CONNECTED\n");
  remove(str);
  int ack;
  if (read(from_server, &ack, 4) < 0) printf("%s\n", strerror(errno));
  //printf("received ack %d\n", ack);
  if (ack == pid + 1){
    //printf("Ack Passed\n");
    pid -= 1;
    write(*to_server, &pid, 4);
  }
  return from_server;
}

void server_handshake_half(int *to_client, int from_client){
  from_client = server_setup();
  int childID = fork();
  if (childID == 0){
    signal(SIGPIPE, plumber);
    read(from_client, to_client, 4);
    char str[512];
    sprintf(str, "%d", *to_client);
    printf("%s received\n", str);
    int ack = *to_client + 1;
    *to_client = open(str, O_WRONLY);
    write(*to_client, &ack, 4);
    int finalAck;
    read(from_client, &finalAck, 4);
    if (finalAck != ack - 2){
      printf("Ack incorrect.");
      exit(0);
    }
    else{
      int heresAnInt = -1;
      while(1){
          heresAnInt = rand();
          int bytes = write(*to_client, &heresAnInt, 4);
          sleep(1);
          if (bytes == -1) break;
      }
      close(*to_client);
      close(from_client);
    }
  }
}
