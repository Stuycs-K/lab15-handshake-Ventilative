#include "pipe_networking.h"

int main() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, plumber);
  int to_client;
  int from_client;
  while(1){
    server_handshake_half(&to_client, from_client);
  }
}
