#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  // int heresAnInt = 1234543;
  // write(to_server, &heresAnInt, 4);
  while(1){
    int buff = -1;
    read(from_server, &buff, 4);
    printf("%d\n", buff);
  }
}
